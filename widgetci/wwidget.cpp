#include "wwidget.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QDir>

WWidget::WWidget(const QUrl fileurl, const QString filepath) : QQuickView(fileurl)
{
    this->fileurl = fileurl;
    this->filepath = filepath;
    this->setParent(0);

    // Transparent and Frameless Window:
    this->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::SplashScreen);
    this->setColor(QColor(0,0,0,0));

    // Show:
    this->show();

    addRightClickMenu();
}

void WWidget::addRightClickMenu(){
    // Menu
    menu_rightClick = new QMenu();
    menu_rightClick->setContextMenuPolicy(Qt::CustomContextMenu);

    // Menu Actions
    QAction* act_Hide = new QAction("Hide",this);
    connect(act_Hide, &QAction::triggered, [=]{
        this->close();
        this->destroy();
        delete this;
    });

    QAction* act_alwaysOnTop = new QAction("Always on top",this);
    act_alwaysOnTop->setCheckable(true);
    connect(act_alwaysOnTop, &QAction::triggered, [=]{
        if(act_alwaysOnTop->isChecked())
            this->setFlags(Qt::WindowStaysOnTopHint);
        else
            this->setFlags(Qt::WindowStaysOnBottomHint);
    });

    menu_rightClick->addAction(act_Hide);
    menu_rightClick->addAction(act_alwaysOnTop);
}

void WWidget::reload(){
    this->setSource(QUrl());
    this->engine()->clearComponentCache();
    this->setSource(fileurl);
    qDebug() << "reloaded";
}

// Drag&Drop the widget: (This prevents qml's onClick events.)
void WWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton){
        QPoint pt(event->pos());
        pt.setX(pt.x()+4);
        pt.setY(pt.y()+28);

        menu_rightClick->exec( this->mapToGlobal(pt) );
    }else if(event->button() == Qt::LeftButton){
        isDragging = true;
        dragX = event->x();
        dragY = event->y();
    }

}
void WWidget::mouseMoveEvent(QMouseEvent *event){
    if(isDragging)
        this->setPosition(event->globalX() - dragX, event->globalY() - dragY);
}
void WWidget::mouseReleaseEvent(QMouseEvent *event){
    isDragging = false;
}

