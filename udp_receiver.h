#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpMessageReceiver : public QObject {
    Q_OBJECT
public:
    explicit UdpMessageReceiver(QObject *parent = nullptr);
    void startListening();

signals:
    void messageReceived(const QString &senderIp, const QString &message);

private slots:
    void onReadyRead();

private:
    QUdpSocket *udpSocket;
};

#endif // UDP_RECEIVER_H
