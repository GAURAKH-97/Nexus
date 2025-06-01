// dropboxview.cpp
#include "dropboxview.h"
#include <QPixmap>
#include <QFileIconProvider>
#include <QHostInfo>

DropboxView::DropboxView(QWidget *parent)
    : QWidget(parent),
    nameLabel(new QLabel(this)),
    myFilesList(new QListWidget(this)),
    othersFilesList(new QListWidget(this))
{

    this->setStyleSheet("background-color:red; margin:0px; padding:0px; ");
    // Avatar
    QString name = QHostInfo::localHostName();
    avatar = new AvatarWidget(name, 72);
    avatar->setOnline(true);




    nameLabel->setText(name);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-weight: bold; margin-top: 5px; font-size:20px; background-color:green;");

    // My files list setup
    myFilesList->setSelectionMode(QAbstractItemView::NoSelection);
    myFilesList->setSpacing(4);
    myFilesList->setFrameShape(QFrame::NoFrame);
    QLabel *myLabel = new QLabel(tr("My Shared Files"), this);
    myLabel->setStyleSheet("font-size: 14px; margin-bottom: 4px;");

    // Others’ files list setup
    othersFilesList->setSelectionMode(QAbstractItemView::SingleSelection);
    othersFilesList->setSpacing(2);
    othersFilesList->setFrameShape(QFrame::NoFrame);
    QLabel *othersLabel = new QLabel(tr("Others’ Shared Files"), this);
    othersLabel->setStyleSheet("font-size: 14px; margin-bottom: 4px;");

    connect(myFilesList, &QListWidget::itemClicked,
            this, &DropboxView::handleDeselectClicked);
    connect(othersFilesList, &QListWidget::itemActivated,
            this, &DropboxView::handleOthersItemActivated);

    // Layouts
    QVBoxLayout *leftCol = new QVBoxLayout;
    leftCol->addWidget(avatar);
    leftCol->addWidget(nameLabel);
    leftCol->addSpacing(10);
    leftCol->addWidget(myLabel);
    leftCol->addWidget(myFilesList);
    leftCol->setStretchFactor(myFilesList, 1);

    QVBoxLayout *rightCol = new QVBoxLayout;
    rightCol->addWidget(othersLabel);
    rightCol->addWidget(othersFilesList);
    rightCol->setStretchFactor(othersFilesList, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(leftCol, 1);
    mainLayout->addLayout(rightCol, 2);
    setLayout(mainLayout);
}

// Public API
void DropboxView::setMyFiles(const QStringList &files)
{
    myFilesList->clear();
    for (const QString &f : files) {
        QListWidgetItem *item = new QListWidgetItem(myFilesList);
        QWidget *w = createMyFileItem(f);
        item->setSizeHint(w->sizeHint());
        myFilesList->setItemWidget(item, w);
    }
}

void DropboxView::setOthersFiles(const QList<QPair<QString, QString>> &filesWithOwners)
{
    othersFilesList->clear();
    QFileIconProvider iconProvider;
    for (auto &p : filesWithOwners) {
        QString display = QString("%1  —  %2").arg(p.first, p.second);
        QListWidgetItem *item = new QListWidgetItem(iconProvider.icon(QFileIconProvider::File), display);
        // store raw data in item for slot
        item->setData(Qt::UserRole, p.first);
        item->setData(Qt::UserRole + 1, p.second);
        othersFilesList->addItem(item);
    }
}

// Create a widget for “My Shared Files” with a deselect button
QWidget* DropboxView::createMyFileItem(const QString &fileName)
{
    QWidget *w = new QWidget;
    QLabel *lbl = new QLabel(fileName);
    QPushButton *btn = new QPushButton(tr("✅ Deselect"));
    btn->setProperty("fileName", fileName);
    btn->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->addWidget(lbl);
    lay->addStretch();
    lay->addWidget(btn);
    lay->setContentsMargins(2,2,2,2);

    connect(btn, &QPushButton::clicked, this, &DropboxView::handleDeselectClicked);
    return w;
}

// Slot implementations
void DropboxView::handleDeselectClicked()
{
    // figure out which button was clicked
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString fileName = btn->property("fileName").toString();
    emit deselectFile(fileName);
}

void DropboxView::handleOthersItemActivated(QListWidgetItem *item)
{
    QString fileName = item->data(Qt::UserRole).toString();
    QString owner    = item->data(Qt::UserRole + 1).toString();
    emit openFile(fileName, owner);
}
