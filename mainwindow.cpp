#include "mainwindow.h"
#include "welcomescreen.h"
#include "mainscreen.h"
#include "lan_discovery.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QNetworkInterface>
#include <QHostInfo>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupTray();
}

void MainWindow::setupUI() {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create and start discovery
    lanDiscovery = new LanDiscovery(this);
    lanDiscovery->start();

    // Welcome screen with transition to main
    welcomeScreen = new WelcomeScreen(this);
    connect(welcomeScreen, &WelcomeScreen::typingFinished, this, &MainWindow::showMainScreen);
    stackedWidget->addWidget(welcomeScreen);

    // Main screen with user list, etc.
    mainScreen = new MainScreen(this);
    stackedWidget->addWidget(mainScreen);

    // Start on welcome
    stackedWidget->setCurrentWidget(welcomeScreen);
    welcomeScreen->startTyping();

    // Initially mark presence as online
    lanDiscovery->setAppPresence(true);
}

void MainWindow::setupTray() {
    trayIcon = new QSystemTrayIcon(QIcon(":/assets/logo.png"), this);
    trayIcon->setToolTip("Project Nexus is running");

    trayMenu = new QMenu(this);
    trayMenu->addAction("Show", this, [this]() {
        this->showNormal();
        this->raise();
        this->activateWindow();
        lanDiscovery->setAppPresence(true);  // back online
    });
    trayMenu->addSeparator();
    trayMenu->addAction("Quit", this, &MainWindow::quitFromTray);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        this->showNormal();
        this->raise();
        this->activateWindow();
        lanDiscovery->setAppPresence(true);  // back online
    }
}

void MainWindow::showMainScreen() {
    stackedWidget->setCurrentWidget(mainScreen);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (trayIcon && trayIcon->isVisible()) {
        hide();  // don't quit, just hide
        lanDiscovery->setAppPresence(false);  // app is hidden, not closed
        trayIcon->showMessage("Nexus", "App is running in background", QSystemTrayIcon::Information, 2000);
        event->ignore();
    } else {
        event->accept();  // fallback
    }
}

void MainWindow::quitFromTray() {
    lanDiscovery->setAppPresence(false);  // mark offline
    trayIcon->hide();
    qApp->quit();
}
