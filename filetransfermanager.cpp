#include "filetransfermanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTcpSocket>

FileTransferManager::FileTransferManager(QObject *parent)
    : QObject(parent), tcpServer(nullptr), clientSocket(nullptr), bytesSent(0), totalBytes(0), headerParsed(false), expectedBytes(0), bytesReceived(0) {

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &FileTransferManager::handleNewConnection);

    if (!tcpServer->listen(QHostAddress::AnyIPv4, 45456)) {
        qDebug() << "[FileTransfer] Failed to start TCP server on port 45456";
    } else {
        qDebug() << "[FileTransfer] TCP server started on port 45456";
    }
}

void FileTransferManager::sendFile(const QString &receiverIp, const QString &filePath, const QString &messageId) {
    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        emit fileReceiveFailed(receiverIp, "Cannot open file: " + filePath);
        return;
    }

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(receiverIp, 45456);

    connect(socket, &QTcpSocket::connected, this, [=]() {
        QFileInfo info(*file);
        QString fileName = info.fileName();
        qint64 fileSize = file->size();

        QByteArray header = QString("%1|%2|%3\n").arg(fileName).arg(fileSize).arg(messageId).toUtf8();
        socket->write(header);
        socket->flush();

        while (!file->atEnd()) {
            socket->write(file->read(8192));
            socket->flush();
        }

        file->close();
        socket->disconnectFromHost();
        qDebug() << "[FileSender] File sent:" << fileName;
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError) {
        emit fileReceiveFailed(receiverIp, socket->errorString());
    });
}

void FileTransferManager::setupReceiver() {
    connect(tcpServer, &QTcpServer::newConnection, this, &FileTransferManager::handleNewConnection);
}

void FileTransferManager::handleNewConnection() {
    QTcpSocket *incoming = tcpServer->nextPendingConnection();
    qDebug() <<"hello"<< incoming;
    if (!incoming) {
        qDebug() << "[FileTransfer] ERROR: No pending connection!";
        return;
    }

    QString senderIp = incoming->peerAddress().toString();
    qDebug() << "[FileTransfer] Connection from IP:" << senderIp;

    connect(incoming, &QTcpSocket::readyRead, this, [=]() mutable {
        buffer.append(incoming->readAll());

        if (!headerParsed) {
            int index = buffer.indexOf('\n');
            if (index != -1) {
                QByteArray header = buffer.left(index);
                buffer.remove(0, index + 1);

                QList<QByteArray> parts = header.split('|');
                if (parts.size() == 3) {
                    actualFileName = QString::fromUtf8(parts[0]);
                    expectedBytes = parts[1].toLongLong();
                    currentMessageId = QString::fromUtf8(parts[2]);
                    this->incomingMessageId = currentMessageId;


                    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Nexus";
                    QDir().mkpath(saveDir);
                    QString filePath = saveDir + "/" + actualFileName;

                    currentFile.setFileName(filePath);
                    if (!currentFile.open(QIODevice::WriteOnly)) {
                        emit fileReceiveFailed(senderIp, "Cannot open file: " + filePath);
                        return;
                    }

                    headerParsed = true;
                    bytesReceived = 0;

                    emit fileReceiveStarted(actualFileName, senderIp, incomingMessageId);
                    qDebug() << "[HeaderParsed] File:" << actualFileName << "| Size:" << expectedBytes << "| ID:" << currentMessageId;
                } else {
                    qDebug() << "[HeaderError] Invalid header format";
                    return;
                }
            } else {
                return;
            }
        }

        if (headerParsed && currentFile.isOpen()) {
            currentFile.write(buffer);
            qDebug() << "[FileTransfer] Wrote chunk of" << buffer.size() << "bytes.";
            bytesReceived += buffer.size();
            buffer.clear();

            if (bytesReceived >= expectedBytes) {
                currentFile.close();
                emit fileReceived(actualFileName, senderIp, incomingMessageId);

                incoming->disconnectFromHost();
                incoming->deleteLater();
                qDebug() << "[FileReceived ✅]" << actualFileName;
            }
        }
    });

    connect(incoming, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, [=](QAbstractSocket::SocketError) {
                qDebug() << "[FileTransfer] Socket error:" << incoming->errorString();
            });
}



void FileTransferManager::handleReadyRead() {
    if (!clientSocket || !currentFile.isOpen()) {
        qDebug() << "[FileTransfer] Cannot read: socket or file is invalid";
        return;
    }

    QByteArray data = clientSocket->readAll();
    if (data.isEmpty()) return;

    qint64 written = currentFile.write(data);
    if (written == -1) {
        qDebug() << "[FileTransfer] ERROR: Failed to write to file.";
    } else {
        qDebug() << "[FileTransfer] Received" << written << "bytes";
    }
}
