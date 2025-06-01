#include "welcomescreen.h"
#include <QVBoxLayout>
#include <QHostInfo>
#include <QPixmap>

WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent), textIndex(0) {
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#19314c"));
    setPalette(palette);




    QString name = QHostInfo::localHostName();
    fullText = "Welcome " + name ;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);


    logoLabel = new QLabel(this);
    QPixmap logo(":/assets/logo.png");  // ensure logo path in .qrc
    logoLabel->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);

    textLabel = new QLabel(this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("font-size: 38px; color:white; background: transparent;");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WelcomeScreen::typeNextCharacter);

    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
}

void WelcomeScreen::startTyping() {
    timer->start(90);
}

void WelcomeScreen::typeNextCharacter() {
    if (textIndex < fullText.length()) {
        textLabel->setText(textLabel->text() + fullText[textIndex]);
        textIndex++;
    } else {
        timer->stop();
        emit typingFinished();
    }
}
