#include "wwidget.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QDir>

WWidget::WWidget(const QUrl fileurl, const QString filepath) : QQuickView(fileurl)
{
    this->fileurl = fileurl;
    this->filepath = filepath;
    this->setParent(Q_NULLPTR);

    // Transparent and Frameless Window:
    this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint);
    this->setColor(QColor(0,0,0,0));

    // Show:
    this->show();

    addRightClickMenu();
}

void WWidget::addRightClickMenu(){
    // Menu
    menu_rightClick = new QMenu();
    menu_rightClick->setContextMenuPolicy(Qt::CustomContextMenu);
    act_seperator = new QAction(this);
    act_seperator->setSeparator(true);

    // Groups (for Radio Button effect)
    QActionGroup* actgr_zPositionGroup = new QActionGroup( this );

    // Menu Actions

    // -- Hide
    QAction* act_Hide = new QAction("Hide",this);
    connect(act_Hide, &QAction::triggered, [=]{
        this->close();
        this->destroy();
        delete this;
    });
    // -- {SEPERATOR}
    // -- Always on top
    QAction* act_alwaysOnTop = new QAction("Always on top",this);
    act_alwaysOnTop->setCheckable(true);
    act_alwaysOnTop->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnTop, &QAction::triggered, [=]{
        if(act_alwaysOnTop->isChecked())
            this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnTopHint);
    });
    // -- Normal (DEFAULT)
    QAction* act_normal = new QAction("Normal",this);
    act_normal->setCheckable(true);
    act_normal->setActionGroup( actgr_zPositionGroup );
    act_normal->setChecked(true);
    connect(act_normal, &QAction::triggered, [=]{
        this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint);
    });
    // -- Always on bottom
    QAction* act_alwaysOnBottom = new QAction("Always on bottom",this);
    act_alwaysOnBottom->setCheckable(true);
    act_alwaysOnBottom->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnBottom, &QAction::triggered, [=]{
        if(act_alwaysOnBottom->isChecked())
            this->setFlags(Qt::FramelessWindowHint | Qt::SplashScreen | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnBottomHint);
    });



    // Adding Sub-Menus & Actions
    menu_rightClick->addAction(act_Hide);
    menu_rightClick->addAction(act_seperator);
    QMenu* submenu_zindex = menu_rightClick->addMenu("Z-Position");
    submenu_zindex->addAction(act_alwaysOnTop);
    submenu_zindex->addAction(act_normal);
    submenu_zindex->addAction(act_alwaysOnBottom);

}

void WWidget::reload(){
    this->setSource(QUrl());
    this->engine()->clearComponentCache();
    this->setSource(fileurl);
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
    Q_UNUSED(event)
    isDragging = false;
}

