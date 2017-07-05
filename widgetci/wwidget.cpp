#include "wwidget.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>

WWidget::WWidget(const QUrl fileurl) : QQuickView(fileurl)
{
    // Transparent and Frameless Window:
    this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    this->setColor(QColor(0,0,0,0));
    this->setObjectName("window");

    // Show:
    this->show();
}

// Drag&Drop the widget:

void WWidget::mousePressEvent(QMouseEvent *event){
    isDragging = true;
    dragX = event->x();
    dragY = event->y();
}
void WWidget::mouseMoveEvent(QMouseEvent *event){
    if(isDragging)
        this->setPosition(event->globalX() - dragX, event->globalY() - dragY);
}
void WWidget::mouseReleaseEvent(QMouseEvent *event){
    isDragging = false;
}

