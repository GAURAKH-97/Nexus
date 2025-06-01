#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>

class FileTransferManager : public QObject {
    Q_OBJECT

public:
    explicit FileTransferManager(QObject *parent = nullptr);

    void sendFile(const QString &receiverIp, const QString &filePath, const QString &messageId);
    void setupReceiver();

signals:

    void fileReceived(const QString &fileName, const QString &senderIp, const QString &messageId);

    void fileReceiveStarted(const QString &fileName, const QString &senderIp, const QString &messageId);
    void fileReceiveFailed(const QString &senderIp, const QString &reason);
    void progressUpdate(const QString &messageId, int percent);
    void fileTransferStarted(const QString &messageId);
    void fileTransferCompleted(const QString &messageId);
    void fileTransferFailed(const QString &messageId, const QString &reason);

private slots:
    void handleNewConnection();
    void handleReadyRead();
    // void handleBytesWritten(qint64 bytes);

private:
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    QFile currentFile;
    QString currentMessageId;
    qint64 bytesSent;
    qint64 totalBytes;

    QByteArray buffer;
    bool headerParsed = false;
    QString actualFileName;
    qint64 bytesReceived = 0;
    qint64 expectedBytes = 0;
    QString incomingMessageId;



};

#endif // FILETRANSFERMANAGER_H
