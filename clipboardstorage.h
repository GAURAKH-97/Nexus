#ifndef CLIPBOARDSTORAGE_H
#define CLIPBOARDSTORAGE_H

#include <QString>
#include <QList>
#include <QSqlDatabase>

struct ClipboardEntry {
    QString text;
    QString sender;
    QString timestamp;
};

class ClipboardStorage {
public:
    ClipboardStorage();
    ~ClipboardStorage();

    void addEntry(const QString &text, const QString &sender);
    QList<ClipboardEntry> getAllEntries();
    QString getLatestEntryText();
    void clear();

private:
    QSqlDatabase db;
};

#endif // CLIPBOARDSTORAGE_H
