#ifndef USERLISTPANEL_H
#define USERLISTPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include "lan_discovery.h"  // for UserInfo

class UserListPanel : public QWidget {
    Q_OBJECT
public:
    explicit UserListPanel(QWidget *parent = nullptr);
    QListWidget* getUserList() const;

public slots:
    void addUser(const UserInfo &user);
    void clearSearch();
    void removeUser(const QString &ip);
    void filterUsers(const QString &text);
    QList<UserInfo> getAllUsers() const;



private:
    QLineEdit   *searchBar;
    QListWidget *userList;
    QList<UserInfo> allUsers;
    void renderUsers(const QList<UserInfo> &users);
};

#endif // USERLISTPANEL_H
