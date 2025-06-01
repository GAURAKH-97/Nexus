#include "rightpanel.h"
#include <QHBoxLayout>
#include <QPixmap>
#include <QDateTime>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>
#include <QUuid>
#include <QFileDialog>
#include <QFileInfo>
#include "avatarwidget.h"
#include "udp_messagesender.h"

extern UdpMessageSender *globalSender;

RightPanel::RightPanel(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: #121212; color: white;");
    setupUI();
    clearPanel();

}

void RightPanel::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setStyleSheet("background-color: white; border-top-right-radius: 10px;");


    placeholderLogo = new QLabel(this);
    QPixmap logo(":/assets/logo.png");
    placeholderLogo->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    placeholderLogo->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(placeholderLogo);

    headerWidget = new QPushButton(this);
    headerWidget->setFlat(true);
    headerWidget->setCursor(Qt::PointingHandCursor);
    headerWidget->setStyleSheet("background-color: #19324c; margin-left:1px; text-align: left;");
    headerWidget->setVisible(false);
    headerWidget->setFixedHeight(60);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 0, 12, 0);

    avatar = new AvatarWidget("?", 46);

    headerLayout->addWidget(avatar);


    QLabel *nameLabel = new QLabel("User Name");
    nameLabel->setObjectName("usernameLabel");
    nameLabel->setStyleSheet("color:white; font-size: 14px; font-weight: bold;");


    headerLayout->addWidget(avatar);
    headerLayout->addWidget(nameLabel);
    headerLayout->addStretch();

    connect(headerWidget, &QPushButton::clicked, this, &RightPanel::handleHeaderClicked);

    chatWidget = new QWidget(this);
    chatLayout = new QVBoxLayout(chatWidget);
    chatLayout->setAlignment(Qt::AlignTop);
    chatLayout->setContentsMargins(10, 10, 10, 10);
    chatLayout->setSpacing(8);
    chatWidget->setStyleSheet("background-color: #cccccc; border-radius:0px; margin-left:1px;");


    chatScroll = new QScrollArea(this);
    chatScroll->setWidgetResizable(true);
    chatScroll->setWidget(chatWidget);
    chatScroll->setVisible(false);
    chatScroll->setStyleSheet(R"(
    QScrollArea {
        background-color: #cccccc;
        border: none;
    }

    QScrollBar:vertical {
        background: transparent;   /* Light background */
        width: 10px;           /* Width of vertical scrollbar */
        border-radius: 5px;    /* Rounded corners for the scrollbar */
    }

    QScrollBar::handle:vertical {
        background: #888;      /* Color of the scrollbar handle */
        border-radius: 5px;
        min-height: 20px;      /* Minimum size of the handle */
    }

    QScrollBar::handle:vertical:hover {
        background: #555;      /* Hover effect for scrollbar handle */
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        border: none;          /* Remove up/down arrows */
    }


)");


    inputWidget = new QWidget(this);
    inputWidget->setFixedHeight(50);
    inputWidget->setVisible(false);
    inputWidget->setStyleSheet("background-color: #19324c;"
                               " border-top-right-radius: 0px;"
                               "margin-left:1px;"
                               " border-bottom-right-radius: 10px;");
    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(8, 6, 8, 6);
    inputLayout->setSpacing(8);

    fileButton = new QPushButton("📁");
    fileButton->setFixedSize(42, 42);
    // fileButton->setStyleSheet("background-color: #254b72; font-size:22px; border-radius:10px;");
    fileButton->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            font-size:25px;
            border-radius:10px;
        }
        QPushButton:hover {
            background-color: #254b72;
        }
        QPushButton:pressed {
            font-size:20px;
        }
    )");
    messageInput = new QLineEdit();
    messageInput->setPlaceholderText("Type a Message");

    messageInput->setStyleSheet(R"(
        QLineEdit {
            background-color: transparent;
            font-size:22px;
            color:white;
        }
        QLineEdit:focus {
            background-color: transparent;
        }
        QLineEdit:pressed {
            font-size:18px;
        }

    )");
    sendButton = new QPushButton("Send 📤");
    sendButton->setFixedSize(82, 42);

    sendButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3ba58a;
            font-size:18px;
            border-radius:10px;
            color:white;
        }
        QPushButton:hover {
            background-color: transparent;
            color:#3ba58a;
        }
        QPushButton:pressed {
            font-size:15px;
        }
    )");

    inputLayout->addWidget(fileButton);
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);


    disconnect(fileButton, &QPushButton::clicked, nullptr, nullptr);

    if (!fileDialogConnected) {
        connect(fileButton, &QPushButton::clicked, this, &RightPanel::handleFileSelection);
        fileDialogConnected = true;
    }

    connect(sendButton, &QPushButton::clicked, this, &RightPanel::handleSendClicked);
    connect(messageInput, &QLineEdit::returnPressed, this, &RightPanel::handleSendClicked);





    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(chatScroll);
    mainLayout->addWidget(inputWidget);


}

void RightPanel::clearPanel() {
    userSelected = false;
    headerWidget->setVisible(false);
    chatScroll->setVisible(false);
    inputWidget->setVisible(false);
    placeholderLogo->setVisible(true);
}

void RightPanel::loadChatHistory() {
    const QList<ChatMessage> msgs = storage.loadMessages(currentUser.ip);
    for (const ChatMessage &msg : msgs) {
        addMessage(msg.message, msg.direction == "sent", msg.messageId, msg.status);
        qDebug() <<"[loadchathistory] "<<msg.message ;
        qDebug() <<"[loadchathistory] "<<msg.status ;

    }
}


void RightPanel::addMessage(const QString &text, bool sentByMe, const QString &messageId, const QString &status) {

    QWidget *wrapper = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *messageLabel = new QLabel(text);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet(sentByMe
                                    ? "background-color: green; color: black; padding: 6px; padding-right:0px; border-radius: 6px; border-top-right-radius: 0px; border-bottom-right-radius: 0px;"
                                    : "background-color: #4c436d; color: white; padding: 6px; border-radius: 6px;");

    if (sentByMe) {
        QLabel *statusIcon = new QLabel();
        layout->addStretch();
        layout->addWidget(messageLabel);
        layout->addWidget(statusIcon);


        if (!messageId.isEmpty()) {
            pendingStatusIcons[messageId] = statusIcon;
            messageTextLabels[messageId] = messageLabel;
        }

        // Set icon based on stored status
        if (status == "delivered") {
            statusIcon->setText("✅");
        } else if (status == "failed") {
            statusIcon->setText("❌");
        } else {
            statusIcon->setText("⏳");
            pendingStatusIcons[messageId] = statusIcon;

            // Add timeout to mark as failed
            QTimer *timer = new QTimer(this);
            timer->setSingleShot(true);
            connect(timer, &QTimer::timeout, this, [=]() {
                if (pendingStatusIcons.contains(messageId)) {
                    pendingStatusIcons[messageId]->setText("❌");
                    pendingStatusIcons.remove(messageId);
                    qDebug() << "[addMessage] ❌ Timeout. No ACK for ID:" << messageId;

                    // update status in DB
                    storage.updateMessageStatus(messageId, "failed");
                }
            });
            timer->start(5000);
            messageTimeouts[messageId] = timer;
        }

        statusIcon->setStyleSheet("background-color: green; padding-right:3px; border-top-right-radius: 6px; border-bottom-right-radius: 6px;");
    } else {
        layout->addWidget(messageLabel);
        layout->addStretch();
    }

    chatLayout->addWidget(wrapper);

    QTimer::singleShot(0, this, [=]() {
        chatScroll->verticalScrollBar()->setValue(chatScroll->verticalScrollBar()->maximum());
    });
}




void RightPanel::handleSendMessage() {
    QString msg = messageInput->text().trimmed();
    if (msg.isEmpty() || currentUser.ip.isEmpty())
        return;

    // 1. Generate a unique message ID
    QString messageId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // 2. Add message to UI and track ID
    qDebug() << "[addMessage] Sent by me. Storing icon for ID: ui track id" << messageId;

    addMessage(msg, true, messageId);

    // 3. Save to DB

    storage.saveMessage(currentUser.ip, msg, "sent", messageId, "pending");


    // 4. Send via UDP
    if (globalSender){
        qDebug() << "[addMessage] Sent by me. Storing icon for ID: global send" << messageId;

        globalSender->sendMessage(currentUser.ip, msg, messageId);
    }
    // 5. Clear input
    messageInput->clear();

    emit messageSendRequested(currentUser.ip, msg);
}




void RightPanel::handleSendClicked() {
    handleSendMessage();
}


void RightPanel::handleHeaderClicked() {
    qDebug() << "[Header] Open user info view (not implemented yet)";
}

void RightPanel::showUserPanel(const UserInfo &user) {
    currentUser = user;
    userSelected = true;

    if (avatar) {
        avatar->setName(user.name);           // regenerate initials
        avatar->setOnline(user.online);       // set status
    }

    QLabel *nameLabel = headerWidget->findChild<QLabel *>("usernameLabel");
    if (nameLabel) {
        nameLabel->setText(user.name + " (" + user.ip + ")");
    }

    headerWidget->setVisible(true);
    chatScroll->setVisible(true);
    inputWidget->setVisible(true);
    placeholderLogo->setVisible(false);

    QLayoutItem *item;
    while ((item = chatLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QList<ChatMessage> history = storage.loadMessages(user.ip);
    for (const ChatMessage &msg : history) {

        addMessage(msg.message, msg.direction == "sent", msg.messageId, msg.status);

    }
}

QString RightPanel::getCurrentUserIP() const {
    return currentUser.ip;
}
void RightPanel::onMessageAcknowledged(const QString &senderIp, const QString &messageId) {


    qDebug() << "[ACK] Looking for:" << messageId;
    qDebug() << "[ACK] All message IDs:" << pendingStatusIcons.keys();

    if (pendingStatusIcons.contains(messageId)) {
        QLabel *icon = pendingStatusIcons[messageId];
        qDebug() << "[ACK] setting icon ✅";
        icon->setText("✅");
        pendingStatusIcons.remove(messageId);
        storage.updateMessageStatus(messageId, "delivered");
    } else {
        qDebug() << "[RightPanel] ❓ Could not find icon for" << messageId;
    }

    if (messageTimeouts.contains(messageId)) {
        messageTimeouts[messageId]->stop();
        messageTimeouts[messageId]->deleteLater();
        messageTimeouts.remove(messageId);
    }
}




void RightPanel::handleFileSelection() {
    static bool fileDialogOpen = false;

    if (fileDialogOpen) {
        qDebug() << "[File] Dialog already open – ignoring.";
        return;
    }

    fileDialogOpen = true;

    QFileDialog dialog(this);
    dialog.setWindowTitle("Select File to Send");
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (dialog.exec() == QDialog::Accepted) {
        QString filePath = dialog.selectedFiles().first();
        if (!filePath.isEmpty()) {
            QString messageId = QUuid::createUuid().toString(QUuid::WithoutBraces);
            emit fileSelected(currentUser.ip, filePath, messageId);
        }
    }

    fileDialogOpen = false;
}
void RightPanel::updateMessageText(const QString &messageId, const QString &newText) {
    if (messageTextLabels.contains(messageId)) {
        QLabel *label = messageTextLabels[messageId];
        label->setText(newText);
        qDebug() << "[RightPanel] Updated message text for ID:" << messageId;
    } else {
        qDebug() << "[RightPanel] ⚠️ No message label found for ID:" << messageId;
    }
}


