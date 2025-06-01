QT       += core gui
QT += core gui widgets network
QT += core gui sql

win32 {
    # Link against the Windows DWM API
    LIBS += -ldwmapi

    # Ensure the DWM symbols (and DWMWA_CAPTION_COLOR) are visible
    DEFINES += _WIN32_WINNT=0x0602
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    avatarwidget.cpp \
    chatstorage.cpp \
    clipboardhotkeyhandler.cpp \
    clipboardstorage.cpp \
    clipboardview.cpp \
    dropboxview.cpp \
    filetransfermanager.cpp \
    floatingmenu.cpp \
    lan_discovery.cpp \
    main.cpp \
    mainscreen.cpp \
    mainwindow.cpp \
    rightpanel.cpp \
    settingspanel.cpp \
    udp_messagereceiver.cpp \
    udp_messagesender.cpp \
    userlistpanel.cpp \
    welcomescreen.cpp

HEADERS += \
    avatarwidget.h \
    chatstorage.h \
    clipboardhotkeyhandler.h \
    clipboardstorage.h \
    clipboardview.h \
    dropboxview.h \
    filetransfermanager.h \
    floatingmenu.h \
    lan_discovery.h \
    mainscreen.h \
    mainwindow.h \
    rightpanel.h \
    settingspanel.h \
    udp_messagereceiver.h \
    udp_messagesender.h \
    userlistpanel.h \
    welcomescreen.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc

DISTFILES += \
    readme.md
RC_FILE = appicon.rc
