#include "udp_receiver.h"
#include <QNetworkDatagram>
#include <QDebug>

UdpMessageReceiver::UdpMessageReceiver(QObject *parent)
    : QObject(parent), udpSocket(new QUdpSocket(this)) {}

void UdpMessageReceiver::startListening() {
    udpSocket->bind(45455, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);  // Different port from discovery
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpMessageReceiver::onReadyRead);
}

void UdpMessageReceiver::onReadyRead() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString message = QString::fromUtf8(datagram.data());
        QString senderIp = datagram.senderAddress().toString();

        if (senderIp.startsWith("::ffff:"))
            senderIp = senderIp.section("::ffff:", 1, 1);

        emit messageReceived(senderIp, message);
    }
}
