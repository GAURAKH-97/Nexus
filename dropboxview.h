// dropboxview.h
#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <avatarwidget.h>

class DropboxView : public QWidget
{
    Q_OBJECT
public:
    explicit DropboxView(QWidget *parent = nullptr);

    // Populate the lists
    void setMyFiles(const QStringList &files);
    void setOthersFiles(const QList<QPair<QString, QString>> &filesWithOwners);

signals:
    // Emitted when user clicks “Deselect”
    void deselectFile(const QString &fileName);
    // Emitted when user wants to open someone else’s file
    void openFile(const QString &fileName, const QString &owner);

private slots:
    void handleDeselectClicked();
    void handleOthersItemActivated(QListWidgetItem *item);

private:

    AvatarWidget *avatar = nullptr;
    QLabel *nameLabel;
    QListWidget *myFilesList;
    QListWidget *othersFilesList;

    QWidget* createMyFileItem(const QString &fileName);
};
