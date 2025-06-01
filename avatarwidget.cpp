#include "avatarwidget.h"
#include <QPainter>
#include <QFontMetrics>
#include <QCryptographicHash>
#include <QDebug>

AvatarWidget::AvatarWidget(const QString &name, int size, QWidget *parent)
    : QWidget(parent), avatarSize(size), isOnline(false) {
    setFixedSize(size, size);
    setName(name);
}

void AvatarWidget::setName(const QString &name) {
    initials = extractInitials(name);
    bgColor = generateColor(name);
    update();
}

void AvatarWidget::setOnline(bool online) {
    isOnline = online;
    update();
}

QString AvatarWidget::extractInitials(const QString &name) {
    QStringList parts = name.trimmed().toUpper().split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return "?";
    if (parts.size() == 1) return parts[0].left(1);
    return parts[0].left(1) + parts[1].left(1);
}

QColor AvatarWidget::generateColor(const QString &text) {
    QByteArray hash = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5);
    int r = static_cast<unsigned char>(hash[0]);
    int g = static_cast<unsigned char>(hash[1]);
    int b = static_cast<unsigned char>(hash[2]);
    return QColor(r, g, b);
}

void AvatarWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Background circle
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, avatarSize, avatarSize);

    // Initials
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(avatarSize / 3);
    painter.setFont(font);

    QFontMetrics metrics(font);
    int textWidth = metrics.horizontalAdvance(initials);
    int textHeight = metrics.ascent();
    painter.drawText((width() - textWidth) / 2, (height() + textHeight) / 2 - 2, initials);

    // Online/offline dot
    int dotSize = avatarSize / 5;
    QColor dotColor = isOnline ? QColor("#2ecc71") : QColor("#7f8c8d");

    painter.setBrush(dotColor);
    painter.setPen(Qt::white);
    int x = width() - dotSize - 4;
    int y = height() - dotSize - 4;
    painter.drawEllipse(x, y, dotSize, dotSize);
}
