#include "wwidget.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QDir>

WWidget::WWidget(const QUrl fileurl, const QString filepath) : QQuickView(fileurl)
{
    this->fileurl = fileurl;
    this->filepath = filepath;
    // Transparent and Frameless Window:
    this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    this->setColor(QColor(0,0,0,0));
    this->setObjectName("window");

    // Show:
    this->show();
}

void WWidget::reload(){
    this->setSource(QUrl());
    this->engine()->clearComponentCache();
    this->setSource(fileurl);
    qDebug() << "reloaded";
}

// Drag&Drop the widget: (This prevents qml's onClick events.)
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

