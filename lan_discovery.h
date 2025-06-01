#ifndef LAN_DISCOVERY_H
#define LAN_DISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QSet>
#include <QMap>
#include <QDateTime>

struct UserInfo {
    QString name;
    QString ip;
    bool online = false;
};

class LanDiscovery : public QObject {
    Q_OBJECT
public:
    explicit LanDiscovery(QObject *parent = nullptr);
    void start();
    void setAppPresence(bool isOnline);

signals:
    void userDiscovered(const UserInfo &user);
    void userExpired(const QString &ip);

private slots:
    void broadcastPresence();
    void processPendingDatagrams();
    void checkForExpiredUsers();

private:
    QUdpSocket *udpSocket;
    QSet<QString> selfIps;

    QTimer *broadcastTimer;
    QTimer *cleanupTimer;
    QSet<QString> knownIps;
    QMap<QString, QDateTime> lastSeenMap;
    bool appPresence = true;
};

#endif // LAN_DISCOVERY_H
