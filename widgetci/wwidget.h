#ifndef WWIDGET_H
#define WWIDGET_H
#include <QQuickView>
#include <QObject>
#include <QMenu>
#include <QSettings>

class WWidget : public QQuickView
{
    Q_OBJECT

public:
    WWidget(const QUrl fileurl, const QString filename, const int wx, const int wy);
    ~WWidget();

    // QML File and Reloading the QML Engine
    void reload();

    // Widget's path
    QString widgetpath;
    QUrl fileurl;
    QString filename;

    // Z-Pos & Lock settings
    int z_pos = 0;
    bool lock = false;

    void toggleZPos(int z_index); // Toggle between z positions.
    void toggleLock(bool t_lock); // Toggle the lock
    QAction* act_Lock;
    QSettings* widgetSettings;

    void saveSettings();

private:
    // Window Dragging.
    bool isDragging = false;
    quint16 dragX = 0, dragY = 0;

    // Right Click Menu
    QMenu *menu_rightClick;
    void addRightClickMenu();

    // Window Flags
    Qt::WindowFlags normalFlags = Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint; // Normal [0]
    Qt::WindowFlags alwaysMostTopFlags = Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint | Qt::BypassGraphicsProxyWidget; // Always on Most top [1]
    Qt::WindowFlags alwaysTopFlags = Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint; // Always on top [2]
    Qt::WindowFlags alwaysBottomFlags = Qt::CustomizeWindowHint | Qt::WindowStaysOnBottomHint; // Always on bottom [3]
    Qt::WindowFlags latestFlags; // For storing the latest change of flags
    QVector<QAction*> actionsList;



protected:
    // Window Dragging & Right Click
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void focusOutEvent(QFocusEvent* event) override;

};

#endif // WWIDGET_H
