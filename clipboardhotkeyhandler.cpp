#include "clipboardhotkeyhandler.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>

ClipboardHotkeyHandler::ClipboardHotkeyHandler(QObject *parent)
    : QObject(parent) {
    qApp->installNativeEventFilter(this);
    registerHotkeys();
}

ClipboardHotkeyHandler::~ClipboardHotkeyHandler() {
    unregisterHotkeys();
    qApp->removeNativeEventFilter(this);
}

bool ClipboardHotkeyHandler::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *) {
    if (eventType != "windows_generic_MSG")
        return false;

    MSG *msg = static_cast<MSG *>(message);
    if (msg->message == WM_HOTKEY) {
        if (msg->wParam == 1) emit ctrlShiftCPressed();
        else if (msg->wParam == 2) emit ctrlShiftVPressed();
        return true;
    }

    return false;
}

void ClipboardHotkeyHandler::registerHotkeys() {
    // Ctrl + Shift + C
    if (!RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_SHIFT, 0x43))  // 'C'
        qDebug() << "[Hotkey] Failed to register Ctrl+Shift+C";

    // Ctrl + Shift + V
    if (!RegisterHotKey(nullptr, 2, MOD_CONTROL | MOD_SHIFT, 0x56))  // 'V'
        qDebug() << "[Hotkey] Failed to register Ctrl+Shift+V";
}

void ClipboardHotkeyHandler::unregisterHotkeys() {
    UnregisterHotKey(nullptr, 1);
    UnregisterHotKey(nullptr, 2);
}


