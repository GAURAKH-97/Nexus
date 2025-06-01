#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class WelcomeScreen : public QWidget {
    Q_OBJECT
public:
    explicit WelcomeScreen(QWidget *parent = nullptr);
    void startTyping();

signals:
    void typingFinished();

private slots:
    void typeNextCharacter();

private:
    QLabel *logoLabel;
    QLabel *textLabel;
    QTimer *timer;
    QString fullText;
    int textIndex;



};

#endif // WELCOMESCREEN_H
