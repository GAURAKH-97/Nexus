#include "settingspanel.h"
#include "avatarwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background-color: transparent;");
    setupUI();
}

void Settings::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);  // Horizontal layout to divide into 2 columns
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignTop);  // Ensure the main layout aligns at the top

    // Left Column: Contains the buttons
    QWidget *leftColumn = new QWidget(this);
    leftColumn->setFixedWidth(120);
    leftColumn->setStyleSheet("background-color: #19324c; border-top-left-radius:12px; border-bottom-left-radius:12px;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftColumn);
    leftLayout->setContentsMargins(0, 0, 0, 0);  // Ensure no extra margins
    leftLayout->setSpacing(0);  // Remove extra spacing
    leftLayout->setAlignment(Qt::AlignTop);  // Align buttons to the top

    // Buttons for the left column
    accountBtn = new QPushButton("Profile");
    chatBtn = new QPushButton("Storage ");
    notificationBtn = new QPushButton("Notifications");
    storageBtn = new QPushButton("Video");
    helpBtn = new QPushButton("Help");

    // Button styling
    QPushButton *buttons[] = {accountBtn, chatBtn, notificationBtn, storageBtn, helpBtn};
    for (auto button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                color: white;
                text-align: left;
                padding: 8px 15px;
                border-radius: 8px;
            }
            QPushButton:hover {
                background-color: transparent;
            }
            QPushButton:pressed {
                background-color: #326498;
                font-size: 12px;
            }
        )");
    }

    // Connect buttons to right panel content
    connect(accountBtn, &QPushButton::clicked, this, [=]() {
        rightPanel->setCurrentIndex(0);
    });
    connect(chatBtn, &QPushButton::clicked, this, [=]() {
        rightPanel->setCurrentIndex(1);
    });
    connect(notificationBtn, &QPushButton::clicked, this, [=]() {
        rightPanel->setCurrentIndex(2);
    });
    connect(storageBtn, &QPushButton::clicked, this, [=]() {
        rightPanel->setCurrentIndex(3);
    });
    connect(helpBtn, &QPushButton::clicked, this, [=]() {
        rightPanel->setCurrentIndex(4);
    });

    // Add buttons to the left column
    leftLayout->addWidget(accountBtn);
    leftLayout->addWidget(chatBtn);
    leftLayout->addWidget(notificationBtn);
    leftLayout->addWidget(storageBtn);
    leftLayout->addWidget(helpBtn);

    // Right Column: Contains the stacked widget for displaying details
    rightPanel = new QStackedWidget(this);
    rightPanel->setStyleSheet("background-color: transparent; border-radius: 0px; margin:0px; padding:0px; ");
    rightPanel->setFixedWidth(350);



    // Help session
    QWidget *accountsession = new QWidget(this);
    QVBoxLayout *accountLayout = new QVBoxLayout(accountsession);
    accountLayout->setAlignment(Qt::AlignTop);
    accountLayout->setContentsMargins(0, 0, 0, 0);  // Remove any margin
    accountLayout->setSpacing(10);  // Set some spacing between elements
    accountsession->setStyleSheet("background-color: #cccccc; "
                                  "color:#19324c; margin-left:0px; "
                                  "border-radius:0px; "
                                  "border-top-right-radius: 12px; border-bottom-right-radius: 12px;");

    // Add text content and align it to the top
    QLabel *titleLabel = new QLabel(tr("Help-centre"), accountsession);
    titleLabel->setTextFormat(Qt::PlainText);
    titleLabel->setStyleSheet("font-size:18px; font-weight:bold; margin-left:10px; padding:0;");

    QLabel *nameLabel = new QLabel(tr("Nexus"), accountsession);
    nameLabel->setTextFormat(Qt::PlainText);
    nameLabel->setStyleSheet("font-size:14px; margin-left:10px; padding:0;");

    QLabel *versionLabel = new QLabel(tr("Version 1.0"), accountsession);
    versionLabel->setMargin(10);

    QLabel *messageLabel = new QLabel(tr("We'd like to know your thoughts about this app."), accountsession);
    messageLabel->setMargin(10);

    QPushButton *contactButton = new QPushButton(tr("Contact Us"), accountsession);
    contactButton->setStyleSheet("margin-left:10px;");

    QPushButton *helpCenterButton = new QPushButton(tr("Help Center"), accountsession);
    helpCenterButton->setStyleSheet("margin-left:10px;");

    QLabel *footerLabel = new QLabel(tr("© 2025 Nexus"), accountsession);
    footerLabel->setMargin(10);

    // Add widgets to the account section, ensuring alignment is at the top
    accountLayout->addWidget(titleLabel, 0, Qt::AlignTop);
    accountLayout->addWidget(nameLabel, 0, Qt::AlignTop);
    accountLayout->addWidget(versionLabel, 0, Qt::AlignTop);
    accountLayout->addWidget(messageLabel, 0, Qt::AlignTop);
    accountLayout->addWidget(contactButton, 0, Qt::AlignTop);
    accountLayout->addWidget(helpCenterButton, 0, Qt::AlignTop);
    accountLayout->addWidget(footerLabel, 0, Qt::AlignTop);
    //help-session-end


    // General session
    QWidget *generalSession = new QWidget(this);
    QVBoxLayout *generalLayout = new QVBoxLayout(generalSession);
    generalLayout->setAlignment(Qt::AlignTop);  // Align content to the top
    generalLayout->setContentsMargins(0, 0, 0, 0);  // Remove any margin
    generalLayout->setSpacing(10);  // Set spacing between elements
    generalSession->setStyleSheet("background-color: #cccccc; color:#19324c; margin-left:0px; "
                                  "border-top-right-radius: 12px; border-bottom-right-radius: 12px;");

    // Avatar
    AvatarWidget *avatar = new AvatarWidget("Gaurakh", 72);
    avatar->setOnline(true);  // Replace with the actual path of the avatar image

    // User Name - QLineEdit
    QLineEdit *userNameLineEdit = new QLineEdit(generalSession);
    userNameLineEdit->setPlaceholderText("Enter your name:");
    userNameLineEdit->setStyleSheet("padding: 5px; border: 1px solid #19324c; border-radius: 4px;");
    userNameLineEdit->setFixedHeight(30);  // Set height for the line edit

    // Additional text or labels
    QLabel *userNameLabel = new QLabel(tr("User Name"), generalSession);
    userNameLabel->setStyleSheet("font-size:14px; font-weight:bold; margin-left:10px; padding:0;");

    // Add Avatar and User Name to Layout
    generalLayout->addWidget(avatar, 0, Qt::AlignCenter);  // Avatar at the top and center aligned
    generalLayout->addWidget(userNameLabel, 0, Qt::AlignTop);  // User name label aligned to top
    generalLayout->addWidget(userNameLineEdit, 0, Qt::AlignTop);  // QLineEdit for user name
    generalLayout->addStretch();  // To push everything to the top
    //account session end


    // Chats session
    QWidget *chatsSession = new QWidget(this);
    QVBoxLayout *chatsLayout = new QVBoxLayout(chatsSession);
    chatsLayout->setAlignment(Qt::AlignTop);  // Align content to the top
    chatsLayout->setContentsMargins(0, 0, 0, 0);  // Remove any margin
    chatsLayout->setSpacing(10);  // Set spacing between elements
    chatsSession->setStyleSheet("background-color: #cccccc; color:#19324c; "
                                "margin-left:0px; "
                                "border-top-right-radius: 12px; border-bottom-right-radius: 12px;");

    // Title: Chats
    QLabel *chatsTitleLabel = new QLabel(tr("Chats"), chatsSession);
    chatsTitleLabel->setStyleSheet("font-size:18px; font-weight:bold; margin-left:10px; padding:0;");

    // Chat history label
    QLabel *chatHistoryLabel = new QLabel(tr("Chat history"), chatsSession);
    chatHistoryLabel->setStyleSheet("font-size:14px; margin-left:10px; padding:0;");

    // Buttons for actions
    QPushButton *archiveChatsButton = new QPushButton(tr("Archive all chats"), chatsSession);
    archiveChatsButton->setStyleSheet("padding: 5px; border: 1px solid #19324c; border-radius: 4px; background-color: transparent;");
    archiveChatsButton->setFixedHeight(30);  // Set height for the button

    QPushButton *clearMessagesButton = new QPushButton(tr("Clear all messages"), chatsSession);
    clearMessagesButton->setStyleSheet("padding: 5px; border: 1px solid red; border-radius: 4px; color: red;");
    clearMessagesButton->setFixedHeight(30);  // Set height for the button

    QPushButton *deleteChatsButton = new QPushButton(tr("Delete all chats"), chatsSession);
    deleteChatsButton->setStyleSheet("padding: 5px; border: 1px solid red; border-radius: 4px; color: red;");
    deleteChatsButton->setFixedHeight(30);  // Set height for the button

    // Add widgets to the chats session layout
    chatsLayout->addWidget(chatsTitleLabel);
    chatsLayout->addWidget(chatHistoryLabel);
    chatsLayout->addWidget(archiveChatsButton);
    chatsLayout->addWidget(clearMessagesButton);
    chatsLayout->addWidget(deleteChatsButton);
    //storage session end


    // Notification session
    QWidget *notificationSession = new QWidget(this);
    QVBoxLayout *notificationLayout = new QVBoxLayout(notificationSession);
    notificationLayout->setAlignment(Qt::AlignTop);  // Align content to the top
    notificationLayout->setContentsMargins(0, 0, 0, 0);  // Remove any margin
    notificationLayout->setSpacing(10);  // Set spacing between elements
    notificationSession->setStyleSheet("background-color: #cccccc; color:#19324c; "
                                       "margin-left:0px; "
                                       "border-top-right-radius: 12px; border-bottom-right-radius: 12px;");

    // Title: Notifications
    QLabel *notificationTitleLabel = new QLabel(tr("Notifications"), notificationSession);
    notificationTitleLabel->setStyleSheet("font-size:18px; font-weight:bold; margin-left:10px; padding:0;");

    // Show taskbar notification badge label
    QLabel *taskbarLabel = new QLabel(tr("Show taskbar notification badge"), notificationSession);
    taskbarLabel->setStyleSheet("font-size:14px; margin-left:10px; padding:0;");

    // Dropdown for taskbar notification setting (like Always, On, etc.)
    QComboBox *taskbarComboBox = new QComboBox(notificationSession);
    taskbarComboBox->addItem(tr("Always"));
    taskbarComboBox->addItem(tr("On"));
    taskbarComboBox->addItem(tr("Off"));
    taskbarComboBox->setStyleSheet("padding: 5px; border: 1px solid #19324c; border-radius: 4px;");
    taskbarComboBox->setFixedHeight(30);  // Set height for the combo box

    // Messages Section
    QLabel *messagesLabel = new QLabel(tr("Messages"), notificationSession);
    messagesLabel->setStyleSheet("font-size:14px; margin-left:10px; padding:0;");

    // Toggle button for Messages (On/Off)
    QPushButton *messagesToggleButton = new QPushButton(tr("On"), notificationSession);
    messagesToggleButton->setStyleSheet("padding: 5px; border: 1px solid green; border-radius: 4px; background-color: #4CAF50; color: white;");
    messagesToggleButton->setFixedHeight(40);  // Set height for the button

    // Connect the Messages toggle button to switch between On and Off
    connect(messagesToggleButton, &QPushButton::clicked, this, [=]() {
        if (messagesToggleButton->text() == tr("On")) {
            messagesToggleButton->setText(tr("Off"));
            messagesToggleButton->setStyleSheet("padding: 5px; border: 1px solid red; border-radius: 4px; background-color: #e57373; color: white;");
        } else {
            messagesToggleButton->setText(tr("On"));
            messagesToggleButton->setStyleSheet("padding: 5px; border: 1px solid green; border-radius: 4px; background-color: #4CAF50; color: white;");
        }
    });

    // File Section
    QLabel *fileLabel = new QLabel(tr("File"), notificationSession);
    fileLabel->setStyleSheet("font-size:14px; margin-left:10px; padding:0;");

    // Toggle button for File (On/Off)
    QPushButton *fileToggleButton = new QPushButton(tr("On"), notificationSession);
    fileToggleButton->setStyleSheet("padding: 5px; border: 1px solid green; border-radius: 4px; background-color: #4CAF50; color: white;");
    fileToggleButton->setFixedHeight(40);  // Set height for the button

    // Connect the File toggle button to switch between On and Off
    connect(fileToggleButton, &QPushButton::clicked, this, [=]() {
        if (fileToggleButton->text() == tr("On")) {
            fileToggleButton->setText(tr("Off"));
            fileToggleButton->setStyleSheet("padding: 5px; border: 1px solid red; border-radius: 4px; background-color: #e57373; color: white;");
        } else {
            fileToggleButton->setText(tr("On"));
            fileToggleButton->setStyleSheet("padding: 5px; border: 1px solid green; border-radius: 4px; background-color: #4CAF50; color: white;");
        }
    });

    // Add widgets to the notification session layout
    notificationLayout->addWidget(notificationTitleLabel);
    notificationLayout->addWidget(taskbarLabel);
    notificationLayout->addWidget(taskbarComboBox);
    notificationLayout->addWidget(messagesLabel);
    notificationLayout->addWidget(messagesToggleButton);
    notificationLayout->addWidget(fileLabel);
    notificationLayout->addWidget(fileToggleButton);

    // Notification_session_end

    //Video session
    QLabel *videoSettingsLabel = new QLabel("Coming soon...");
    videoSettingsLabel->setStyleSheet("background-color: #cccccc; color:#19324c; "
                                      " font-size:20px; "
                                      "border-top-right-radius: 12px; border-bottom-right-radius: 12px;" );  // Set text color to white


    videoSettingsLabel->setAlignment(Qt::AlignCenter);
    //vodeo session end

    // Add sections to the right panel
    rightPanel->addWidget(generalSession);
    rightPanel->addWidget(chatsSession);
    rightPanel->addWidget(notificationSession);

    rightPanel->addWidget(videoSettingsLabel);
    rightPanel->addWidget(accountsession);

    // Set default section (Account)
    rightPanel->setCurrentIndex(0);

    // Add left and right columns to the main layout
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(rightPanel);

    // Shadow effect for left column
    // QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    // shadow->setBlurRadius(24);
    // shadow->setOffset(0, 2);
    // shadow->setColor(QColor(0, 0, 0, 150));
    // leftColumn->setGraphicsEffect(shadow);
}
