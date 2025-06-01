#ifndef UDP_MESSAGERECEIVER_H
#define UDP_MESSAGERECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpMessageReceiver : public QObject {
    Q_OBJECT
public:
    explicit UdpMessageReceiver(QObject *parent = nullptr);

signals:
    void messageAcknowledged(const QString &senderIp, const QString &messageId);
    void messageReceived(const QString &senderIp, const QString &message, const QString &messageId);  // UPDATED



private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // UDP_MESSAGERECEIVER_H
