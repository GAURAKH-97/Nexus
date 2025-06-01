#include "userlistpanel.h"
#include "avatarwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QAction>
#include <QNetworkInterface>

UserListPanel::UserListPanel(QWidget *parent)
    : QWidget(parent) {

    setFixedWidth(400);
    setStyleSheet(R"(
        background-color: #19324c;
        border-top-left-radius: 10px;


        padding: 10px;
        margin:0px;
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);





    QLabel *userLabel = new QLabel("🧑‍💻🧑‍💻  User");
    userLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: white; padding:20px;");
    layout->addWidget(userLabel);

    // Search bar
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("🔍 Search users...");

    QAction *clearAction = new QAction(this);
    clearAction->setIconText("❌");
    clearAction->setToolTip("Clear");
    searchBar->addAction(clearAction, QLineEdit::TrailingPosition);
    connect(clearAction, &QAction::triggered, this, &UserListPanel::clearSearch);
    connect(searchBar, &QLineEdit::textChanged, this, &UserListPanel::filterUsers);




    searchBar->setStyleSheet(R"(
        QLineEdit {
            margin:0px;
            background-color: #273c56;
            color: white;
            border: none;
            border-bottom: 2px solid gray;
            border-radius: 0px;
            width:80%;
        }
        QLineEdit:focus {
            border-bottom: 2px solid #2ecc71;
        }
    )");
    layout->addWidget(searchBar);

    // User list view
    userList = new QListWidget(this);
    userList->setStyleSheet(R"(
    QListWidget {
        background-color: #19324c;
        color: white;
        border: none;
        border-radius: 0px;
        border-bottom-left-radius: 10px;

    }
    QListWidget::item {
        padding: 0px;

    }
    QListWidget::item:selected {
        background-color: #263c55;
        border:none;
        margin: 0px;
        padding: 0px;

    }
)");

    layout->addWidget(userList);
}

void UserListPanel::clearSearch() {
    searchBar->clear();
}

QListWidget* UserListPanel::getUserList() const {
    return userList;
}

static QString getLocalIp() {
    const QList<QHostAddress> &addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &addr : addresses) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
            return addr.toString();
        }
    }
    return QString();
}



void UserListPanel::addUser(const UserInfo &user) {
    // Prevent duplicate IPs in allUsers
    for (const UserInfo &u : allUsers) {
        if (u.ip == user.ip)
            return;
    }

    // Add to full user list
    allUsers.append(user);

    // If search is empty → show all
    QString currentSearch = searchBar->text();
    if (currentSearch.trimmed().isEmpty()) {
        renderUsers(allUsers);
    } else {
        filterUsers(currentSearch);
    }
}

void UserListPanel::removeUser(const QString &ip) {
    for (int i = 0; i < userList->count(); ++i) {
        QListWidgetItem *item = userList->item(i);
        if (item->data(Qt::UserRole + 1).toString() == ip) {
            delete userList->takeItem(i);
            break;
        }
    }
}


void UserListPanel::filterUsers(const QString &text) {
    if (text.trimmed().isEmpty()) {
        renderUsers(allUsers);
        return;
    }

    QList<UserInfo> filtered;
    for (const UserInfo &user : allUsers) {
        if (user.name.contains(text, Qt::CaseInsensitive) ||
            user.ip.contains(text, Qt::CaseInsensitive)) {
            filtered.append(user);
        }
    }
    renderUsers(filtered);
}
void UserListPanel::renderUsers(const QList<UserInfo> &users) {
    userList->clear();

    for (const UserInfo &user : users) {

        if (user.ip ==  "127.0.0.1" ) {
            continue; // skip yourself
        }


        QWidget *itemWidget = new QWidget();
        itemWidget->setStyleSheet("background-color: transparent;");

        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
        itemLayout->setContentsMargins(10, 6, 10, 6);
        itemLayout->setSpacing(12);

        AvatarWidget *avatar = new AvatarWidget(user.name, 36);
        avatar->setOnline(user.online);

        QString displayName = user.name;
        if (user.ip == getLocalIp())
            displayName += " (You)";

        QLabel *label = new QLabel(displayName + " (" + user.ip + ")", itemWidget);

        if (user.ip == getLocalIp())
            label->setStyleSheet("font-size: 14px; color: lightgreen; font-weight: bold;");
        else
            label->setStyleSheet("font-size: 14px; color: white; border:none;");

        label->setMinimumHeight(36);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        itemLayout->addWidget(avatar);
        itemLayout->addWidget(label);
        itemLayout->addStretch();

        QListWidgetItem *item = new QListWidgetItem(userList);
        item->setSizeHint(itemWidget->sizeHint());
        item->setData(Qt::UserRole, user.name);
        item->setData(Qt::UserRole + 1, user.ip);

        userList->addItem(item);
        userList->setItemWidget(item, itemWidget);
    }
}

QList<UserInfo> UserListPanel::getAllUsers() const {
    return allUsers;
}

