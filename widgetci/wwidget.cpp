#include "wwidget.h"
#include <QQmlEngine>
#include <QQuickItem>
#include <QDir>
#include <QPushButton>
#include <QMessageBox>


WWidget::WWidget(const QUrl fileurl, const QString filename) : QQuickView(fileurl)
{
    // Store the widget's qml file url and path.`
    this->fileurl = fileurl;
    this->filename = filename;
    this->setParent(Q_NULLPTR);

    // Transparent and Frameless Window:
    this->setFlags(this->normalFlags);
    this->setColor(QColor(0,0,0,0));

    // Show:
    this->show();

    // Widget's RightClick Menu
    addRightClickMenu();
}

WWidget::~WWidget(){
    // Deconstructor
}

void WWidget::reload(){
    this->setSource(QUrl());
    this->engine()->clearComponentCache();
    this->setSource(fileurl);
}

// Drag&Drop the widget:
void WWidget::mousePressEvent(QMouseEvent *event){
    QQuickView::mousePressEvent(event);

    switch (event->button()) {
    case (Qt::LeftButton): {
        isDragging = true;
        dragX = event->x();
        dragY = event->y();
        break;
    }
    case (Qt::RightButton): {
        QPoint pt(event->pos());
        pt.setX(pt.x()+2);
        pt.setY(pt.y()+2);
        menu_rightClick->exec( this->mapToGlobal(pt) );
        break;
    }
    default:
        break;
    }

}
void WWidget::mouseMoveEvent(QMouseEvent *event){
    QQuickView::mouseMoveEvent(event);

    if(isDragging)
        this->setPosition(event->globalX() - dragX, event->globalY() - dragY);
}
void WWidget::mouseReleaseEvent(QMouseEvent *event){
    QQuickView::mouseReleaseEvent(event);

    isDragging = false;
}

void WWidget::addRightClickMenu(){
    // Menu
    menu_rightClick = new QMenu();
    menu_rightClick->setContextMenuPolicy(Qt::CustomContextMenu);
    act_seperator = new QAction(this);
    act_seperator->setSeparator(true);

    // Groups (for Radio Button effect)
    QActionGroup* actgr_zPositionGroup = new QActionGroup( this );

    // MENU ACTIONS -->>
    // -- Hide
    QAction* act_Hide = new QAction("Hide",this);
    connect(act_Hide, &QAction::triggered, [=]{
        delete this;
    });
    // -- Reload
    QAction* act_Reload = new QAction("Reload",this);
    connect(act_Reload, &QAction::triggered, [=]{
        this->reload();
    });
    // -- {SEPERATOR_AT_HERE}
    // -- Always on Most Top
    QAction* act_alwaysOnMostTop = new QAction("Always on Most top",this);
    act_alwaysOnMostTop->setCheckable(true);
    act_alwaysOnMostTop->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnMostTop, &QAction::triggered, [=]{
        if(act_alwaysOnMostTop->isChecked()){
            if(this->latestFlags == this->alwaysMostTopFlags){
                this->destroy();
                this->create();
                this->show();
                this->setFlags(this->normalFlags | this->alwaysMostTopFlags);
            }else{
                this->hide();
                this->setFlags((this->flags() ^ this->latestFlags) | this->alwaysMostTopFlags);
                this->show();
                this->requestActivate();
            }

            this->latestFlags = this->alwaysMostTopFlags;
        }
    });
    // -- Always on top
    QAction* act_alwaysOnTop = new QAction("Always on top",this);
    act_alwaysOnTop->setCheckable(true);
    act_alwaysOnTop->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnTop, &QAction::triggered, [=]{
        if(act_alwaysOnTop->isChecked()){
            if(this->latestFlags == this->alwaysMostTopFlags){
                this->destroy();
                this->create();
                this->show();
                this->setFlags(this->normalFlags);
                this->setFlags(this->flags() | this->alwaysTopFlags);
            }else{
                this->setFlags((this->flags() ^ this->latestFlags) | this->alwaysTopFlags);
                this->show();
            }

            this->latestFlags = this->alwaysTopFlags;
        }
    });
    // -- Normal (DEFAULT)
    QAction* act_normal = new QAction("Normal",this);
    act_normal->setCheckable(true);
    act_normal->setActionGroup( actgr_zPositionGroup );
    act_normal->setChecked(true);
    connect(act_normal, &QAction::triggered, [=]{
        if(this->latestFlags == this->alwaysMostTopFlags){
            this->destroy();
            this->create();
            this->show();
            this->setFlags(this->normalFlags);
        }else{
            this->setFlags((this->flags() ^ this->latestFlags));
            this->show();
        }

        this->latestFlags = Q_NULLPTR;
    });
    // -- Always on bottom
    QAction* act_alwaysOnBottom = new QAction("Always on bottom",this);
    act_alwaysOnBottom->setCheckable(true);
    act_alwaysOnBottom->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnBottom, &QAction::triggered, [=]{
        if(act_alwaysOnBottom->isChecked()){
            if(this->latestFlags == this->alwaysMostTopFlags){
                this->destroy();
                this->create();
                this->show();
                this->setFlags(this->normalFlags | this->alwaysBottomFlags);
            }else{
                this->setFlags((this->flags() ^ this->latestFlags) | this->alwaysBottomFlags);
                this->show();
            }

            this->latestFlags = this->alwaysBottomFlags;
        }
    });



    // Adding Sub-Menus & Actions
    menu_rightClick->addAction(act_Hide);
    menu_rightClick->addAction(act_Reload);
    menu_rightClick->addAction(act_seperator);
    QMenu* submenu_zindex = menu_rightClick->addMenu("Z-Position");
        submenu_zindex->addAction(act_alwaysOnMostTop);
        submenu_zindex->addAction(act_alwaysOnTop);
        submenu_zindex->addAction(act_normal);
        submenu_zindex->addAction(act_alwaysOnBottom);

}

