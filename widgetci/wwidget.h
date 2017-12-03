#ifndef WWIDGET_H
#define WWIDGET_H
#include <QQuickView>
#include <QObject>
#include <QMenu>

class WWidget : public QQuickView
{
    Q_OBJECT

public:
    WWidget(const QUrl fileurl, const QString filename);
    ~WWidget();

    // QML File and Reloading the QML Engine
    void reload();
    QUrl fileurl;
    QString filename;
private:
    // Window Dragging.
    bool isDragging = false;
    quint16 dragX = 0, dragY = 0;

    // Right Click Menu
    QMenu *menu_rightClick;
    QAction *act_seperator;
    void addRightClickMenu();

    // Window Flags
    Qt::WindowFlags alwaysMostTopFlags = Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint | Qt::BypassGraphicsProxyWidget; // Always on Most top
    Qt::WindowFlags alwaysTopFlags = Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint; // Always on top
    Qt::WindowFlags normalFlags = Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint; // Normal
    Qt::WindowFlags alwaysBottomFlags = Qt::CustomizeWindowHint | Qt::WindowStaysOnBottomHint; // Always on bottom
    Qt::WindowFlags latestFlags; // For storing the latest change of flags

protected:
    // Window Dragging & Right Click
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

};

#endif // WWIDGET_H
