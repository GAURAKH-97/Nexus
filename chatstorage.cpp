#include "chatstorage.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

ChatStorage::ChatStorage() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/nexus_chat.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open()) {
        qWarning() << "Failed to open chat DB:" << db.lastError().text();
    } else {
        initDB();
    }
}

ChatStorage::~ChatStorage() {
    if (db.isOpen())
        db.close();
}

void ChatStorage::initDB() {
    QSqlQuery query;
    QString createTable = R"(
    CREATE TABLE IF NOT EXISTS messages (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ip TEXT NOT NULL,
        message TEXT NOT NULL,
        direction TEXT CHECK(direction IN ('sent', 'received')) NOT NULL,
        timestamp INTEGER NOT NULL,
        messageId TEXT,
        status TEXT
    )
)";

    if (!query.exec(createTable)) {
        qWarning() << "Create table failed:" << query.lastError().text();
    }
}


void ChatStorage::saveMessage(const QString &ip, const QString &message, const QString &direction, const QString &messageId, const QString &status) {
    QSqlQuery query;
    query.prepare("INSERT INTO messages (ip, message, direction, timestamp, messageId, status) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(ip);
    query.addBindValue(message);
    query.addBindValue(direction);
    query.addBindValue(QDateTime::currentSecsSinceEpoch());
    query.addBindValue(messageId);
    query.addBindValue(status);

    if (!query.exec()) {
        qWarning() << "Insert message failed:" << query.lastError().text();
    }
}



QList<ChatMessage> ChatStorage::loadMessages(const QString &ip) {
    QList<ChatMessage> messages;
    QSqlQuery query;
    query.prepare("SELECT message, direction, timestamp, messageId, status FROM messages WHERE ip = ? ORDER BY timestamp ASC");


    query.addBindValue(ip);

    if (query.exec()) {
        while (query.next()) {
            ChatMessage msg;
            msg.ip = ip;
            msg.message = query.value(0).toString();
            msg.direction = query.value(1).toString();
            msg.timestamp = query.value(2).toLongLong();
            msg.messageId = query.value(3).toString();
            msg.status = query.value(4).toString();
            qDebug() <<"[loadmessages] "<<query.value(0).toString() ;
            qDebug() <<"[loadmessages] "<<query.value(3).toString() ;
            qDebug() <<"[loadmessages] "<<query.value(4).toString() ;
            messages.append(msg);
        }
    }
    return messages;
}

void ChatStorage::updateMessageStatus(const QString &messageId, const QString &newStatus) {
    QSqlQuery query;
    query.prepare("UPDATE messages SET status = ? WHERE messageId = ?");
    query.addBindValue(newStatus);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qWarning() << "Failed to update message status:" << query.lastError().text();
    }
}

void ChatStorage::updateMessageText(const QString &messageId, const QString &newText) {
    QSqlQuery query;
    query.prepare("UPDATE messages SET message = ? WHERE messageId = ?");
    query.addBindValue(newText);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qWarning() << "Failed to update message text:" << query.lastError().text();
    }
}
