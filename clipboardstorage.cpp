#include "clipboardstorage.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QStack>



ClipboardStorage::ClipboardStorage() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/nexus_clipboard.db";
    db = QSqlDatabase::addDatabase("QSQLITE", "clipboard_connection");
    db.setDatabaseName(path);

    if (!db.open()) {
        qWarning() << "[ClipboardStorage] Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "[ClipboardStorage] Database opened at" << path;
        QSqlQuery query(db);
        query.exec("CREATE TABLE IF NOT EXISTS clipboard ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "text TEXT NOT NULL, "
                   "sender TEXT NOT NULL, "
                   "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");
    }
}

ClipboardStorage::~ClipboardStorage() {
    db.close();
    QSqlDatabase::removeDatabase("clipboard_connection");
}

void ClipboardStorage::addEntry(const QString &text, const QString &sender) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO clipboard (text, sender) VALUES (:text, :sender)");
    query.bindValue(":text", text);
    query.bindValue(":sender", sender);
    if (!query.exec()) {
        qWarning() << "[ClipboardStorage] Failed to insert:" << query.lastError().text();
    }
}

QList<ClipboardEntry> ClipboardStorage::getAllEntries() {
    QList<ClipboardEntry> entries;
    QSqlQuery query(db);

    // Fetch latest 100 in reverse order (newest first)
    query.exec("SELECT text, sender, timestamp FROM clipboard ORDER BY id DESC LIMIT 100");

    while (query.next()) {
        ClipboardEntry entry;
        entry.text = query.value(0).toString();
        entry.sender = query.value(1).toString();
        entry.timestamp = query.value(2).toString();
        entries.append(entry);
    }

    return entries;
}


QString ClipboardStorage::getLatestEntryText() {
    QSqlQuery query(db);
    query.exec("SELECT text FROM clipboard ORDER BY id DESC LIMIT 1");
    if (query.next()) {
        return query.value(0).toString();
    }
    return QString();
}

void ClipboardStorage::clear() {
    QSqlQuery query(db);
    query.exec("DELETE FROM clipboard");
}
