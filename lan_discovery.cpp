#include "lan_discovery.h"
#include <QHostInfo>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QDebug>

LanDiscovery::LanDiscovery(QObject *parent)
    : QObject(parent) {

    udpSocket = new QUdpSocket(this);
    broadcastTimer = new QTimer(this);
    cleanupTimer = new QTimer(this);

    connect(broadcastTimer, &QTimer::timeout, this, &LanDiscovery::broadcastPresence);
    connect(udpSocket, &QUdpSocket::readyRead, this, &LanDiscovery::processPendingDatagrams);
    connect(cleanupTimer, &QTimer::timeout, this, &LanDiscovery::checkForExpiredUsers);


    // ✅ Store all local IPs for skipping
    const QList<QHostAddress> &addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &addr : addresses) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
            selfIps.insert(addr.toString());
        }
    }
}

void LanDiscovery::start() {
    udpSocket->bind(45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    broadcastTimer->start(2000);  // broadcast every 2 seconds
    cleanupTimer->start(2000);    // check for inactive users
}

void LanDiscovery::setAppPresence(bool isOnline) {
    appPresence = isOnline;
}

void LanDiscovery::broadcastPresence() {
    QString name = QHostInfo::localHostName();
    QString status = appPresence ? "ONLINE" : "OFFLINE";
    QByteArray datagram = QString("PRESENCE: %1, %2").arg(name, status).toUtf8();

    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
            QHostAddress broadcast = entry.broadcast();
            if (broadcast != QHostAddress::Null &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                udpSocket->writeDatagram(datagram, broadcast, 45454);
            }
        }
    }
}

void LanDiscovery::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString message = QString::fromUtf8(datagram.data());
        QString ip = datagram.senderAddress().toString();

        if (ip.startsWith("::ffff:"))
            ip = ip.section("::ffff:", 1, 1);

        // qDebug() << "[Discovery] Got:" << ip << "vs self:" << selfIps;

        // ✅ Ignore all local IPs
        if (selfIps.contains(ip))
            continue;

        QStringList parts = message.split(", ");
        if (parts.size() == 2) {
            QString name = parts[0].section(":", 1).trimmed();
            QString status = parts[1].trimmed();

            lastSeenMap[ip] = QDateTime::currentDateTime();

            if (!knownIps.contains(ip)) {
                knownIps.insert(ip);

                UserInfo user;
                user.name = name;
                user.ip = ip;
                user.online = (status == "ONLINE");

                emit userDiscovered(user);
            }
        }
    }
}

void LanDiscovery::checkForExpiredUsers() {
    QDateTime now = QDateTime::currentDateTime();
    for (auto it = lastSeenMap.begin(); it != lastSeenMap.end(); ) {
        if (it.value().msecsTo(now) > 6000) {
            QString expiredIp = it.key();
            it = lastSeenMap.erase(it);
            knownIps.remove(expiredIp);
            emit userExpired(expiredIp);
        } else {
            ++it;
        }
    }
}
