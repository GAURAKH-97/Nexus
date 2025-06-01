#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <QWidget>
#include <QColor>

class AvatarWidget : public QWidget {
    Q_OBJECT
public:
    explicit AvatarWidget(const QString &name, int size = 36, QWidget *parent = nullptr);

    void setName(const QString &name);
    void setOnline(bool online);


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString extractInitials(const QString &name);
    QColor generateColor(const QString &text);

    QString initials;
    QColor bgColor;
    int avatarSize;
    bool isOnline;
};

#endif // AVATARWIDGET_H
