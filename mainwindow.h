#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QListWidgetItem>

class QStackedWidget;
class WelcomeScreen;
class MainScreen;
class LanDiscovery;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void onUserSelected(QListWidgetItem *item);
protected:
    void closeEvent(QCloseEvent *event) override;


private slots:


    void showMainScreen();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void quitFromTray();

private:
    void setupUI();
    void setupTray();

    QStackedWidget   *stackedWidget;
    WelcomeScreen    *welcomeScreen;
    MainScreen       *mainScreen;
    LanDiscovery     *lanDiscovery;

    QSystemTrayIcon  *trayIcon;
    QMenu            *trayMenu;
};

#endif // MAINWINDOW_H
