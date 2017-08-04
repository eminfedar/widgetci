#ifndef WWIDGET_H
#define WWIDGET_H
#include <QQuickView>
#include <QObject>
#include <QMenu>

class WWidget : public QQuickView
{
    Q_OBJECT

public:
    WWidget(const QUrl fileurl, const QString filepath);

    void reload();
    QUrl fileurl;
    QString filepath;
private:
    bool isDragging = false;
    quint16 dragX = 0, dragY = 0;

    QMenu *menu_rightClick;
    QAction *act_seperator;
    void addRightClickMenu();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

};

#endif // WWIDGET_H
