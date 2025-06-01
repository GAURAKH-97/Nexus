#ifndef CLIPBOARDVIEW_H
#define CLIPBOARDVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QStringList>
#include "clipboardstorage.h"




class ClipboardView : public QWidget {
    Q_OBJECT
public:
    explicit ClipboardView(QWidget *parent = nullptr);
    void addEntry(const QString &text, const QString &user);
    void clearEntries();

private:
    QVBoxLayout *entryLayout;
    QWidget *entryContainer;
private slots:
    void handlesettingClicked();



};

#endif // CLIPBOARDVIEW_H
