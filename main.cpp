#include "mainwindow.h"
#include <QApplication>

#ifdef Q_OS_WIN
#  include <windows.h>
#  include <dwmapi.h>
#  ifndef DWMWA_CAPTION_COLOR
#    define DWMWA_CAPTION_COLOR 35
#  endif

static void setStaticTitleBarColor(HWND hwnd, COLORREF color) {
    DwmSetWindowAttribute(hwnd,
                          DWMWA_CAPTION_COLOR,
                          &color,
                          sizeof(color));
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("NEXUS");
    QIcon appIcon(":/assets/logo.png");
    a.setWindowIcon(appIcon);

    MainWindow w;
    w.setWindowTitle("NEXUS");
    w.setWindowIcon(appIcon);

#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(w.winId());
    COLORREF customColor = RGB(204, 204, 204);
    setStaticTitleBarColor(hwnd, customColor);
#endif

    w.showMaximized();
    return a.exec();
}
