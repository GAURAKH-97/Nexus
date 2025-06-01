#ifndef FLOATINGMENU_H
#define FLOATINGMENU_H

#include <QWidget>

class FloatingMenu : public QWidget {
    Q_OBJECT
public:
    explicit FloatingMenu(QWidget *parent = nullptr);

signals:
    void clipboardSelected();
    void dropbocSelected();

    void clipboardClicked();  // Add this inside your FloatingMenu class
    void dropdocClicked();

private:
    void setupUI();
};

#endif // FLOATINGMENU_H
