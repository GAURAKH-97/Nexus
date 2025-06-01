#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QStackedWidget;

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

signals:
    void accountClicked();
    void chatClicked();
    void notificationClicked();
    void storageClicked();
    void helpClicked();

private:
    void setupUI();

    QPushButton *accountBtn;
    QPushButton *chatBtn;
    QPushButton *notificationBtn;
    QPushButton *storageBtn;
    QPushButton *helpBtn;

    QVBoxLayout *leftLayout;
    QStackedWidget *rightPanel;
};

#endif // SETTINGS_H
