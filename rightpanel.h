#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QLabel>
#include "lan_discovery.h"
#include "chatstorage.h"
#include "avatarwidget.h"


class RightPanel : public QWidget {
    Q_OBJECT
public:
    explicit RightPanel(QWidget *parent = nullptr);
    void showUserPanel(const UserInfo &user);
    void clearPanel();
    void addMessage(const QString &text, bool sentByMe, const QString &messageId = QString(), const QString &status = QString());

    QMap<QString, QLabel*> messageTextLabels;  // messageId → QLabel*
    void updateMessageText(const QString &messageId, const QString &newText);





    QString getCurrentUserIP() const;

private slots:
    void handleSendMessage();
    void handleFileSelection();
    void handleHeaderClicked();
    void handleSendClicked();
signals:
    void messageSendRequested(const QString &receiverIp, const QString &message);
    void fileSelected(const QString &receiverIp, const QString &filePath, const QString &messageId);


public slots:
    void onMessageAcknowledged(const QString &senderIp, const QString &messageId);


private:
    // Layout containers
    QVBoxLayout *mainLayout;
    QPushButton *headerWidget;
    QWidget     *chatWidget;
    QWidget     *inputWidget;
    QScrollArea *chatScroll;
    QLabel      *placeholderLogo;
    ChatStorage storage;
    QWidget *chatHistoryWidget;
    AvatarWidget *avatar = nullptr;
    QMap<QString, QLabel*> messageLabels;  // messageId → label


    QMap<QString, QLabel*> messageStatusLabels;  // Map of messageId -> QLabel
    QMap<QString, QLabel*> pendingMessageStatus;
    QMap<QString, QLabel*> pendingStatusIcons;
    QMap<QString, QTimer*> messageTimeouts;  // Add this to your RightPanel.h

    QMap<QString, QLabel*> messageStatusMap;



    // Chat area
    QVBoxLayout *chatLayout;

    // Bottom row
    QPushButton *fileButton;
    QLineEdit   *messageInput;
    QPushButton *sendButton;

    // Current user
    UserInfo currentUser;
    bool userSelected = false;
    bool fileDialogOpen = false;
private:
    bool fileDialogConnected = false;  // Add this line



    void setupUI();
    void loadChatHistory();  // will connect to SQLite later
    };

#endif // RIGHTPANEL_H
