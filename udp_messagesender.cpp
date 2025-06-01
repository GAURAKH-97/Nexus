#include "udp_messagesender.h"
#include <QHostAddress>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>

UdpMessageSender::UdpMessageSender(QObject *parent)
    : QObject(parent) {
    udpSocket = new QUdpSocket(this);
}


void UdpMessageSender::sendMessage(const QString &ip, const QString &message, const QString &messageId) {
    QJsonObject json;
    json["type"] = "msg";
    json["id"] = messageId;
    json["message"] = message;

    QByteArray data = QJsonDocument(json).toJson();
    qDebug() << "[SEND]" << ip << ":" << 45455 << "=>" << message;

    bool sent = udpSocket->writeDatagram(data, QHostAddress(ip), 45455);
    udpSocket->flush();  // Ensure message is pushed to socket
    qDebug() << "[SEND] writeDatagram returned:" << sent;

    if (!sent)
        qDebug() << "[SEND ERROR]" << udpSocket->errorString();
}
void UdpMessageSender::sendAck(const QString &ip, const QString &messageId) {
    QJsonObject json;
    json["type"] = "ack";
    json["id"] = messageId;

    QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    qDebug() << "[SEND ACK]" << ip << ":" << 45455 << "=> for ID:" << messageId;

    bool sent = udpSocket->writeDatagram(data, QHostAddress(ip), 45455);
    if (!sent)
        qDebug() << "[ACK ERROR]" << udpSocket->errorString();
}

