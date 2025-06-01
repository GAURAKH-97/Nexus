#include "udp_messagereceiver.h"
#include <QNetworkDatagram>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "udp_messagesender.h"

extern UdpMessageSender *globalSender;

UdpMessageReceiver::UdpMessageReceiver(QObject *parent)
    : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    bool ok = udpSocket->bind(QHostAddress::AnyIPv4, 45455, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    if (!ok) {
        qDebug() << "[Udp Receiver] Failed to bind to port 45455";
    } else {
        qDebug() << "[Udp Receiver] Successfully bound to port 45455";
    }

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &UdpMessageReceiver::processPendingDatagrams);

    qDebug() << "[Udp Receiver] Constructor complete, waiting for datagrams...";
}

void UdpMessageReceiver::processPendingDatagrams() {

    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString senderIp = datagram.senderAddress().toString();
        if (senderIp.startsWith("::ffff:"))
            senderIp = senderIp.section("::ffff:", 1, 1);

        QByteArray data = datagram.data();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        qDebug() << "[Raw Data]" << QString::fromUtf8(data);


        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qDebug() << "[Udp Receiver] Invalid JSON from" << senderIp << "→" << QString::fromUtf8(data);
            continue;
        }

        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();
        QString message = obj.value("message").toString();
        QString messageId = obj.value("id").toString();

        if (type == "msg") {
            qDebug() << "[RECEIVED]" << senderIp << "=> [MSG]" << message;
            emit messageReceived(senderIp, message, messageId);
            if (!messageId.isEmpty() && globalSender) {
                qDebug() << "sending ack";
                globalSender->sendAck(senderIp, messageId);
            } else {
                qDebug() << "[Receiver] Not sending ACK — empty ID or sender missing.";
            }


        } else if (type == "ack") {
            qDebug() << "[ACK RECEIVED] from" << senderIp << "for ID:" << messageId;
            emit messageAcknowledged(senderIp, messageId);
        }
    }
}
