#ifndef WWIDGET_H
#define WWIDGET_H
#include <QQuickView>
#include <QObject>

class WWidget : public QQuickView
{
    Q_OBJECT

public:
    WWidget(const QUrl fileurl);

private:
    bool isDragging = false;
    u_int16_t dragX = 0, dragY = 0;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

};

#endif // WWIDGET_H
