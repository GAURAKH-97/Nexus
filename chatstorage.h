#ifndef CHATSTORAGE_H
#define CHATSTORAGE_H

#include <QString>
#include <QSqlDatabase>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>


struct ChatMessage {
    QString ip;
    QString message;
    QString direction;
    QString messageId;
    QString status;
    qint64 timestamp;
};


class ChatStorage {
public:
    ChatStorage();
    ~ChatStorage();
    void updateMessageStatus(const QString &messageId, const QString &newStatus);
    void updateMessageText(const QString &messageId, const QString &newText);
    void saveMessage(const QString &ip, const QString &message, const QString &direction, const QString &messageId, const QString &status);

    QList<ChatMessage> loadMessages(const QString &ip);

private:
    QSqlDatabase db;
    void initDB();
};

#endif // CHATSTORAGE_H
