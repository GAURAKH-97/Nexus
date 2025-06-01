#ifndef MAINSCREEN_H
#define MAINSCREEN_H


#include <QHBoxLayout>
#include<QListWidgetItem>
#include "udp_messagereceiver.h"
#include "chatstorage.h"
#include "floatingmenu.h"
#include "settingspanel.h"
#include "clipboardview.h"
#include "clipboardhotkeyhandler.h"
#include "clipboardstorage.h"
#include "filetransfermanager.h"
#include "dropboxview.h"

#include <QWidget>
#include <QPushButton>


class UserListPanel;
class RightPanel;
class LanDiscovery;

class MainScreen : public QWidget {
    Q_OBJECT
public:
    explicit MainScreen(QWidget *parent = nullptr);
    void toggleFloatingMenu();


private:
    FileTransferManager *fileTransferManager;


    QHBoxLayout    *mainLayout;
    UserListPanel  *userListPanel;
    RightPanel     *rightPanel;
    LanDiscovery   *lanDiscovery;
    UdpMessageReceiver *receiver;
    ChatStorage storage;  // you can keep this per-user too
    FloatingMenu *floatingMenu;
    Settings *settingsWindow;
    DropboxView *dbView;
    QPushButton *menuButton;        // ✅ Add this line
    ClipboardView *clipboardView;
    ClipboardHotkeyHandler *hotkeyHandler;
    ClipboardStorage clipboardStorage;

protected:
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void onGlobalCopy();
    void onGlobalPaste();


    void sendMessageToUser(const QString &receiverIp, const QString &msg);

    void onUserSelected(QListWidgetItem *item);
    void onFileSelected(const QString &receiverIp, const QString &filePath, const QString &messageId);

    void onMessageReceived(const QString &senderIp, const QString &msg, const QString &messageId);

};

#endif // MAINSCREEN_H
