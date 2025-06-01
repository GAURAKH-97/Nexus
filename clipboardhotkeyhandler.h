#ifndef CLIPBOARDHOTKEYHANDLER_H
#define CLIPBOARDHOTKEYHANDLER_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class ClipboardHotkeyHandler : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT

public:
    explicit ClipboardHotkeyHandler(QObject *parent = nullptr);
    ~ClipboardHotkeyHandler();

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

    void registerHotkeys();
    void unregisterHotkeys();
signals:
    void ctrlShiftCPressed();
    void ctrlShiftVPressed();


};

#endif // CLIPBOARDHOTKEYHANDLER_H
