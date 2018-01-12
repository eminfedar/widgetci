#include "wwidget.h"
#include <QQmlEngine>
#include <QQuickItem>
#include <QDir>
#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>


WWidget::WWidget(const QUrl fileurl, const QString filename, const int wx, const int wy):
    QQuickView(fileurl),
    widgetpath(fileurl.toDisplayString()),
    fileurl(fileurl),
    filename(filename)
{
    this->setParent(Q_NULLPTR);

    // Transparent and Frameless Window:
    this->setFlags(this->normalFlags);
    this->setColor(QColor(0,0,0,0));

    // Position
    if(wx != -1000 && wy != -1000) this->setPosition(wx, wy);

    // Widget's RightClick Menu
    addRightClickMenu();

    // Show:
    this->show();
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
        if(!lock){
            isDragging = true;
            dragX = event->x();
            dragY = event->y();
        }
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

    if(isDragging && !lock)
        this->setPosition(event->globalX() - dragX, event->globalY() - dragY);
}
void WWidget::mouseReleaseEvent(QMouseEvent *event){
    QQuickView::mouseReleaseEvent(event);

    isDragging = false;
}

void WWidget::toggleZPos(int z_index = 0){
    this->z_pos = z_index;
    switch (z_index) {
    // Normal [0]            -------------
    case 0:
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
        break;

        // Always on most top [1] -------------
    case 1:
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
        break;

        // Always on top [2] -------------
    case 2:
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
        break;

        // Always on bottom [3] -------------
    case 3:
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
        break;
    default:
        break;
    }

    if(!actionsList.at(z_index)->isChecked())
        actionsList.at(z_index)->setChecked(true);
}

void WWidget::toggleLock(bool t_lock){
    lock = t_lock;
    act_Lock->setChecked(lock);
}

void WWidget::addRightClickMenu(){
    // Menu
    menu_rightClick = new QMenu();
    menu_rightClick->setContextMenuPolicy(Qt::CustomContextMenu);

    // Groups (for Radio Button effect)
    QActionGroup* actgr_zPositionGroup = new QActionGroup( this );

    // MENU ACTIONS -->>
    // -- Hide
    QAction* act_Hide = new QAction(tr("Hide"),this);
    connect(act_Hide, &QAction::triggered, [=]{
        delete this;
    });

    // -- Reload
    QAction* act_Reload = new QAction(tr("Reload"),this);
    connect(act_Reload, &QAction::triggered, [=]{
        this->reload();
    });

    // -- Edit
    QAction* act_Edit = new QAction(tr("Edit"),this);
    connect(act_Edit, &QAction::triggered, [=]{
        QDesktopServices::openUrl(fileurl);
    });

    // -- {SEPERATOR_AT_HERE} --

    // -- Always on Most Top [1]
    QAction* act_alwaysOnMostTop = new QAction(tr("Always on Most top"),this);
    act_alwaysOnMostTop->setCheckable(true);
    act_alwaysOnMostTop->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnMostTop, &QAction::triggered, [=]{
        toggleZPos(1);
    });

    // -- Always on top [2]
    QAction* act_alwaysOnTop = new QAction(tr("Always on top"),this);
    act_alwaysOnTop->setCheckable(true);
    act_alwaysOnTop->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnTop, &QAction::triggered, [=]{
        toggleZPos(2);
    });

    // -- Normal (DEFAULT) [0]
    QAction* act_normal = new QAction(tr("Normal"),this);
    act_normal->setCheckable(true);
    act_normal->setActionGroup( actgr_zPositionGroup );
    connect(act_normal, &QAction::triggered, [=]{
        toggleZPos(0);
    });

    // -- Always on bottom [3]
    QAction* act_alwaysOnBottom = new QAction(tr("Always on bottom"),this);
    act_alwaysOnBottom->setCheckable(true);
    act_alwaysOnBottom->setActionGroup( actgr_zPositionGroup );
    connect(act_alwaysOnBottom, &QAction::triggered, [=]{
        toggleZPos(3);
    });

    // -- Lock
    act_Lock = new QAction(tr("Lock"),this);
    act_Lock->setCheckable(true);
    connect(act_Lock, &QAction::triggered, [=]{
        toggleLock(!lock);
    });



    // Adding Sub-Menus & Actions
    menu_rightClick->addAction(act_Hide);
    menu_rightClick->addSeparator();
    menu_rightClick->addAction(act_Reload);
    menu_rightClick->addAction(act_Edit);
    menu_rightClick->addSeparator();
    QMenu* submenu_zindex = menu_rightClick->addMenu(tr("Z-Position"));
        submenu_zindex->addAction(act_alwaysOnMostTop);
        submenu_zindex->addAction(act_alwaysOnTop);
        submenu_zindex->addAction(act_normal);
        submenu_zindex->addAction(act_alwaysOnBottom);
    menu_rightClick->addSeparator();
    menu_rightClick->addAction(act_Lock);



    // Adding actions to list.
    actionsList.append(act_normal);
    actionsList.append(act_alwaysOnMostTop);
    actionsList.append(act_alwaysOnTop);
    actionsList.append(act_alwaysOnBottom);
}



