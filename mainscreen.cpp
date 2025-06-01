#include "mainscreen.h"
#include "userlistpanel.h"
#include "rightpanel.h"
#include "lan_discovery.h"
#include "udp_messagesender.h"
#include "udp_messagereceiver.h"
#include "dropboxview.h"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QAbstractAnimation>


#include <QVBoxLayout>
#include <QPushButton>
#include <QHostInfo>
#include <QFileInfo>
#include <QNetworkInterface>
#include <QClipboard>
#include <QGuiApplication>
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>



static QString getLocalIp() {
    const QList<QHostAddress> &addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &addr : addresses) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
            return addr.toString();
        }
    }
    return QString();
}
UdpMessageSender *globalSender = nullptr;

MainScreen::MainScreen(QWidget *parent)
    : QWidget(parent) {

    setStyleSheet("background-color: #121212; color: white;");

    // === Main horizontal layout ===
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === Column 1: Left Menu ===
    QWidget *leftColumn = new QWidget(this);
    leftColumn->setFixedWidth(60);
    leftColumn->setStyleSheet("background-color: #f3f3f3;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftColumn);
    leftLayout->setContentsMargins(5, 10, 10, 10);
    leftLayout->setSpacing(20);

    QPushButton *menuButton = new QPushButton("☰");
    menuButton->setStyleSheet(R"(
    /* normal state */
    QPushButton {
        color:        #0067c0;
        font-size:    18px;
        border-left: 2px solid #0067c0;
        border-top:2px solid #0067c0;
        border-radius:10px;
        background:   #cccccc;

    }
    /* hover state */
    QPushButton:hover {

        color:        #004a8c;     /* darker blue */
        border-right: 2px solid #004a8c;
        border-bottom:2px solid #004a8c;
        border-left: none;
        border-top:none;
    }
    /* pressed state (optional) */
    QPushButton:pressed {
        background:   #cce0ff;
    }
)");menuButton->setFixedSize(50, 50);
    floatingMenu = new FloatingMenu(this);
    connect(menuButton, &QPushButton::clicked, this, [=]() {
        QPoint globalPos = menuButton->mapToGlobal(QPoint(menuButton->width() + 10, 0));
        floatingMenu->move(globalPos);
        floatingMenu->setFixedHeight(this->height()-20);
        floatingMenu->show();
    });


    QPushButton *settingsButton = new QPushButton("⚙");
    settingsButton->setFixedSize(50, 50);
    settingsButton->setStyleSheet(R"(
    /* normal state */
    QPushButton {
        color:        #0067c0;
        font-size:    18px;
        border-right: 2px solid #0067c0;
        border-bottom:2px solid #0067c0;
        border-radius:10px;
        background:   #cccccc;

    }
    /* hover state */
    QPushButton:hover {

        color:        #004a8c;     /* darker blue */
        border-left: 2px solid #004a8c;
        border-top:2px solid #004a8c;
        border-right: none;
        border-bottom:none;
    }
    /* pressed state (optional) */
    QPushButton:pressed {
        background:   #cce0ff;
    }
)");
    // floatingMenu = new FloatingMenu(this);
    settingsWindow = new Settings(this);
    connect(settingsButton, &QPushButton::clicked, this, [=]() {
        QPoint bottomLeft = menuButton->mapToGlobal(QPoint(0, menuButton->height()));
        settingsWindow->move(bottomLeft);
        settingsWindow->setFixedHeight(this->height()*0.70);
        settingsWindow->show();
    });

    leftLayout->addWidget(menuButton, 0, Qt::AlignTop);

    leftLayout->addStretch();
    leftLayout->addWidget(settingsButton, 0, Qt::AlignBottom);

    // === Column 2: User List Panel ===
    userListPanel = new UserListPanel(this);
    // === Add self manually ===
    UserInfo selfUser;
    selfUser.name = QHostInfo::localHostName();
    selfUser.ip = getLocalIp();  // You already created this at the top of mainscreen.cpp
    selfUser.online = true;
    userListPanel->addUser(selfUser);

    // UserInfo selfUser1;
    // selfUser1.name = "Cheris";
    // selfUser1.ip = "192.168.152.21";  // You already created this at the top of mainscreen.cpp
    // selfUser1.online = false;
    // userListPanel->addUser(selfUser1);

    // UserInfo selfUser2;
    // selfUser2.name = "Dheeraj";
    // selfUser2.ip = "192.168.152.212";  // You already created this at the top of mainscreen.cpp
    // selfUser2.online = false;
    // userListPanel->addUser(selfUser2);

    // UserInfo selfUser3;
    // selfUser3.name = "Annu";
    // selfUser3.ip = "192.168.152.23";  // You already created this at the top of mainscreen.cpp
    // selfUser3.online = false;
    // userListPanel->addUser(selfUser3);

    // UserInfo selfUser4;
    // selfUser4.name = "Keshav";
    // selfUser4.ip = "192.168.152.24";  // You already created this at the top of mainscreen.cpp
    // selfUser4.online = true;
    // userListPanel->addUser(selfUser4);











    // === Column 3: Right Panel (default view) ===
    rightPanel = new RightPanel(this);

    // === Add to main layout ===
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(userListPanel);
    mainLayout->addWidget(rightPanel);


    receiver = new UdpMessageReceiver(this);
    // QTimer::singleShot(2000, this, [=](){
    //     qDebug() << "[Test] Manually triggering receiver slot";
    //     emit receiver->messageReceived("127.0.0.1", "Manual Test", "msg123");
    // });


    connect(receiver, &UdpMessageReceiver::messageReceived,
            this, &MainScreen::onMessageReceived);
    qDebug() << "[MainScreen] messageReceived signal connected.";
    connect(receiver, &UdpMessageReceiver::messageReceived, this, [](const QString &ip, const QString &msg, const QString &id){
        qDebug() << "[Test SLOT] Got message from:" << ip << "message:" << msg << "id:" << id;
    });




    connect(receiver, &UdpMessageReceiver::messageAcknowledged,
            rightPanel, &RightPanel::onMessageAcknowledged);
    // receiver->startListening();

    connect(rightPanel, &RightPanel::fileSelected, this, &MainScreen::onFileSelected);



    clipboardView = new ClipboardView(this);
    clipboardView->setVisible(false);  // Hide initially
    mainLayout->addWidget(clipboardView);
    QList<ClipboardEntry> entries = clipboardStorage.getAllEntries();
    for (const ClipboardEntry &entry : entries) {
        clipboardView->addEntry(entry.text, entry.sender);
    }
    dbView  = new DropboxView(this);
    dbView->setVisible(false);
    mainLayout->addWidget(dbView);
    dbView ->setMyFiles(QStringList{"report.pdf", "slides.pptx"});
    dbView ->setOthersFiles({{"design.docx","Alice"},{"data.csv","Bob"}});


    connect(floatingMenu, &FloatingMenu::clipboardClicked, this, [=]() {
        clipboardView->setVisible(true);
        // clipboardView->addEntry("Hello, world!", "Gaurakh");
        rightPanel->setVisible(false);
        dbView->setVisible(false);
    });
    connect(floatingMenu, &FloatingMenu::dropdocClicked, this, [=]() {
        clipboardView->setVisible(false);
        // clipboardView->addEntry("Hello, world!", "Gaurakh");
        rightPanel->setVisible(false);
        dbView->setVisible(true);
    });

    connect(userListPanel->getUserList(), &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
        clipboardView->setVisible(false);
        dbView->setVisible(false);
        rightPanel->setVisible(true);
    });

    hotkeyHandler = new ClipboardHotkeyHandler(this);

    connect(hotkeyHandler, &ClipboardHotkeyHandler::ctrlShiftCPressed, this, &MainScreen::onGlobalCopy);
    connect(hotkeyHandler, &ClipboardHotkeyHandler::ctrlShiftVPressed, this, &MainScreen::onGlobalPaste);


    hotkeyHandler->registerHotkeys();  // Don't forget to call this

    // === LanDiscovery & connection ===
    lanDiscovery = new LanDiscovery(this);
    connect(lanDiscovery, &LanDiscovery::userDiscovered, userListPanel, &UserListPanel::addUser);
    connect(lanDiscovery, &LanDiscovery::userExpired, userListPanel, &UserListPanel::removeUser);

    connect(userListPanel->getUserList(), &QListWidget::itemClicked,
            this, &MainScreen::onUserSelected);






    lanDiscovery->start();
    globalSender = new UdpMessageSender(this);


    fileTransferManager = new FileTransferManager(this);
    fileTransferManager->setupReceiver();  // Make sure this is called

    connect(rightPanel, &RightPanel::fileSelected, this, [=](const QString &receiverIp, const QString &filePath, const QString &messageId) {
        fileTransferManager->sendFile(receiverIp, filePath, messageId);
    });

    connect(fileTransferManager, &FileTransferManager::fileReceiveStarted, this, [=](const QString &fileName, const QString &senderIp, const QString &messageId) {
        if (rightPanel->getCurrentUserIP() == senderIp) {
            rightPanel->addMessage("📥 Receiving file: " + fileName + "...", false, messageId);
            storage.saveMessage(senderIp, "📥 Receiving file: " + fileName, "received", messageId, "pending");


        }
    });

    connect(fileTransferManager, &FileTransferManager::fileReceived, this, [=](const QString &fileName, const QString &senderIp, const QString &messageId) {
        QString updatedText = "✅ Received: " + fileName;
        storage.updateMessageText(messageId,updatedText);
        if (rightPanel->getCurrentUserIP() == senderIp) {


            rightPanel->updateMessageText(messageId, updatedText);
            rightPanel->addMessage("✅ File  Received " + fileName + "...", false, messageId);




        }
    });


    connect(fileTransferManager, &FileTransferManager::fileReceiveFailed, this, [=](const QString &senderIp, const QString &reason) {
        if (rightPanel->getCurrentUserIP() == senderIp) {
            rightPanel->addMessage("❌ File receive failed: " + reason, false);
        }
    });


}



void MainScreen::onMessageReceived(const QString &senderIp, const QString &msg, const QString &messageId) {
    if (senderIp == getLocalIp()) return;
    qDebug() << "[onMessageReceived] message received:" << msg << "from:" << senderIp << "id:" << messageId;

    if (msg.startsWith("CLIP|")) {
        QStringList parts = msg.split("|", Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            QString senderName = parts[1];
            QString text = parts.mid(2).join("|");

            clipboardStorage.addEntry(text, senderName);
            clipboardView->addEntry(text, senderName);
            return;
        }
    }

    // ✅ Save to DB with messageId and status
    storage.saveMessage(senderIp, msg, "received", messageId, "received");

    if (rightPanel->getCurrentUserIP() == senderIp) {
        rightPanel->addMessage(msg, false, messageId, "received");
    }
}






void MainScreen::onUserSelected(QListWidgetItem *item) {
    QString name = item->data(Qt::UserRole).toString();
    QString ip = item->data(Qt::UserRole + 1).toString();

    UserInfo user;
    user.name = name;
    user.ip = ip;
    user.online = true;

    rightPanel->showUserPanel(user);
}
void MainScreen::sendMessageToUser(const QString &receiverIp, const QString &msg) {
    if (globalSender) {
        globalSender->sendMessage(receiverIp, msg);
    }
}
void MainScreen::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (floatingMenu && floatingMenu->isVisible()) {
        floatingMenu->setFixedHeight(this->height());
    }
}

void MainScreen::toggleFloatingMenu() {
    if (!floatingMenu) return;

    if (floatingMenu->isVisible()) {
        floatingMenu->hide();
    } else {
        QPoint globalPos = menuButton->mapToGlobal(QPoint(menuButton->width()-100, 0));
        floatingMenu->move(0,0);
        floatingMenu->setFixedWidth(150);  // Same as left column

        floatingMenu->setFixedHeight(0);
        floatingMenu->show();

        QPropertyAnimation *anim = new QPropertyAnimation(floatingMenu, "maximumHeight", this);
        anim->setDuration(200);
        anim->setStartValue(0);
        anim->setEndValue(height());
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainScreen::onGlobalCopy() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString text = clipboard->text().trimmed();
    if (text.isEmpty()) return;

    QString sender = QHostInfo::localHostName();
    clipboardStorage.addEntry(text, sender);
    // clipboardView->addEntry(text, sender);

    QString messageId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    // rightPanel->addMessage(text, true, messageId);  // ✅ Add message with ID
    // storage.saveMessage("broadcast", text, "sent"); // optional logging

    for (const UserInfo &user : userListPanel->getAllUsers()) {
        if (user.ip != getLocalIp()) {
             globalSender->sendMessage(user.ip, "CLIP|" + sender + "|" + text, messageId);
        }
    }
}

void MainScreen::onGlobalPaste() {
    QString latest = clipboardStorage.getLatestEntryText();

    if (!latest.isEmpty()) {
        QClipboard *clip = QGuiApplication::clipboard();
        clip->setText(latest);
        qDebug() << "[Paste] Inserted latest clipboard entry into system clipboard:" << latest;
    } else {
        qDebug() << "[Paste] Clipboard is empty.";
    }
}

void MainScreen::onFileSelected(const QString &receiverIp, const QString &filePath, const QString &messageId) {
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    QString filePlaceholder = QString("📁 %1 – sending...").arg(fileName);

    rightPanel->addMessage(filePlaceholder, true, messageId); // Use messageId for status icon

    // TODO in Step 3: Start actual file transfer...
    qDebug() << "[File] Selected file:" << fileName << ", ID:" << messageId;
}


