#include "floatingmenu.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

FloatingMenu::FloatingMenu(QWidget *parent)
    : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet(" background-color: transparent;");
    setupUI();
}

void FloatingMenu::setupUI() {
    QWidget *menuBox = new QWidget(this);
    menuBox->setFixedWidth(250);
    menuBox->setStyleSheet(R"(
    QWidget {
        background-color: #19324c;
        border-radius: 12px;
        color: white;
font-size:23px;
    }
    QLabel, QPushButton {
        padding: 8px;
    }
)");


    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(menuBox);

    QVBoxLayout *layout = new QVBoxLayout(menuBox);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(2);


    QPushButton *clipboardBtn = new QPushButton("📋 Clipboard");
    clipboardBtn->setCursor(Qt::PointingHandCursor);
    clipboardBtn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: white;
            text-align: left;
            padding: 8px 12px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #326498;
        }
        QPushButton:pressed {

            border:2px solid green;

            font-size:20px;
        }
    )");
    QPushButton *dropbocBtn = new QPushButton("📋 Dropbox");
    dropbocBtn->setCursor(Qt::PointingHandCursor);
    dropbocBtn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: white;
            text-align: left;
            padding: 8px 12px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #326498;

        }
        QPushButton:pressed {

            border:2px solid green;

            font-size:20px;
        }
    )");

    connect(clipboardBtn, &QPushButton::clicked, this, [=]() {
        emit clipboardSelected();
        hide(); // auto close
    });
    connect(clipboardBtn, &QPushButton::clicked, this, &FloatingMenu::clipboardClicked);

    connect(dropbocBtn, &QPushButton::clicked, this, [=]() {
        emit dropbocSelected();
        hide(); // auto close
    });
    connect(dropbocBtn, &QPushButton::clicked, this, &FloatingMenu::dropdocClicked);


    layout->addWidget(clipboardBtn);
    layout->addWidget(dropbocBtn);
    layout->addStretch();

    // Shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 150));
    menuBox->setGraphicsEffect(shadow);
}
