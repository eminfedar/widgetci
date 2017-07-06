#ifndef WWIDGET_H
#define WWIDGET_H
#include <QQuickView>
#include <QObject>

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

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

};

#endif // WWIDGET_H
