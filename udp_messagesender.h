#ifndef UDPMESSAGESENDER_H
#define UDPMESSAGESENDER_H

#include <QObject>
#include <QUdpSocket>

class UdpMessageSender : public QObject {
    Q_OBJECT
public:
    explicit UdpMessageSender(QObject *parent = nullptr);
    void sendMessage(const QString &ip, const QString &message, const QString &messageId = QString());
    void sendAck(const QString &ip, const QString &messageId);

private:
    QUdpSocket *udpSocket;
};

#endif // UDPMESSAGESENDER_H
