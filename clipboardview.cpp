#include "clipboardview.h"
#include <QHBoxLayout>
#include <QClipboard>
#include <QApplication>
#include <QDebug>
#include <QHostInfo>

ClipboardView::ClipboardView(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #17314b; "
                  "color: white; margin:0px; "
                  "padding: 0px; "
                  "border-top-right-radius: 8px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 0, 0, 0);
    mainLayout->setSpacing(1);




    QWidget *headerWidget =  new QWidget(this);
    headerWidget->setFixedHeight(50);
    headerWidget->setVisible(true);
    headerWidget->setStyleSheet("background-color: #19324c;"
                               " border-top-right-radius: 10px;"
                               " border-bottom-right-radius: 0px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 20, 0);
    headerLayout->setSpacing(0);

    QLabel *title = new QLabel("📋 Clipboard");
    title->setStyleSheet("font-size: 22px; font-weight: bold; padding: 10px;");

    QPushButton *settingsButton = new QPushButton("⚙");
    settingsButton->setFixedSize(50, 50);
    connect(settingsButton, &QPushButton::clicked, this, &ClipboardView::handlesettingClicked);
    settingsButton->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            font-size:25px;
            border-radius:10px;

        }
        QPushButton:hover {
            background-color: #254b72;
        }
    QPushButton:pressed {
        background:   #cce0ff;
    }
    )");




    headerLayout->addWidget(title);
    headerLayout->addWidget(settingsButton);



    mainLayout->addWidget(headerWidget);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");
    scrollArea->setStyleSheet(R"(
    QScrollArea {
        background-color: #cccccc;
        border: none;


    }

    QScrollBar:vertical {
        background: transparent;   /* Light background */
        width: 10px;           /* Width of vertical scrollbar */
        border-radius: 5px;    /* Rounded corners for the scrollbar */
    }

    QScrollBar::handle:vertical {
        background: #888;      /* Color of the scrollbar handle */
        border-radius: 5px;
        min-height: 20px;      /* Minimum size of the handle */
    }

    QScrollBar::handle:vertical:hover {
        background: #555;      /* Hover effect for scrollbar handle */
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        border: none;          /* Remove up/down arrows */
    }


)");

    entryContainer = new QWidget(scrollArea);
    entryLayout = new QVBoxLayout(entryContainer);
    entryLayout->setContentsMargins(15,5,15,0);
    entryLayout->setAlignment(Qt::AlignTop);
    entryLayout->setSpacing(5);
    entryContainer->setLayout(entryLayout);

    scrollArea->setWidget(entryContainer);
    mainLayout->addWidget(scrollArea);
}

void ClipboardView::addEntry(const QString &text, const QString &user) {
    QWidget *entryWidget = new QWidget(this);
    QVBoxLayout *entryBox = new QVBoxLayout(entryWidget);
    entryWidget->setStyleSheet(" border-radius:10px; background-color: #273f59;");

    // Main text + Copy button
    QHBoxLayout *topLine = new QHBoxLayout();
    QLabel *textLabel = new QLabel(text);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("font-size:20px; border:none; color:white; margin-left:20px; margin-bottom:0px;");
    QPushButton *copyBtn = new QPushButton("📝 copy");
    // copyBtn->setFixedWidth(60);
    copyBtn->setFixedSize(70,50);
    copyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            font-size:15px;
            border-radius:10px;
color:white;
border:none;

        }
        QPushButton:hover {
            background-color: #254b72;
        }
    QPushButton:pressed {
        font-size: 13px  ;
        border:1px solid blue;
    }
    )");

    connect(copyBtn, &QPushButton::clicked, [text]() {
        QApplication::clipboard()->setText(text);
        qDebug() << "Copied to clipboard:" << text;
    });
    connect(copyBtn, &QPushButton::clicked, this, [=]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(text);
    });

    topLine->addWidget(textLabel);
    topLine->addStretch();
    topLine->addWidget(copyBtn);

    // Subtext with username
    QLabel *fromLabel;
    if(user==QHostInfo::localHostName()){
        fromLabel = new QLabel("└── " + user+" (You)");
        fromLabel->setStyleSheet("color: white; font-size: 12px; border:none; margin-left:20px;");
    }else{
        fromLabel = new QLabel("└── " + user);
        fromLabel->setStyleSheet("color: #90ee79; font-size: 12px; border:none; margin-left:20px;");
    }


    entryBox->addLayout(topLine);
    entryBox->addWidget(fromLabel);
    entryWidget->setLayout(entryBox);

    // entryLayout->addWidget(entryWidget);
    entryLayout->insertWidget(0, entryWidget);
}

void ClipboardView::clearEntries() {
    QLayoutItem *child;
    while ((child = entryLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}
void ClipboardView::handlesettingClicked(){
    qDebug() << "[Header] Open clipboard settings view (not implemented yet)";
}
