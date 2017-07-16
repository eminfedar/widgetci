#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardPaths>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QSystemTrayIcon>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    appConfig();
    addTrayIcon();
    loadWidgets();
}

mainWindow::~mainWindow()
{
    delete ui;
}
void mainWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    this->setVisible(false);
}

void mainWindow::toggleWidget(QTreeWidgetItem *item){
    if(!map_widgetList.contains(item->text(0))){
        WWidget *wid = new WWidget(QUrl::fromLocalFile(widgetsDir + "/" + item->text(0) + "/main.qml"), widgetsDir + "/" + item->text(0));
        map_widgetList.insert(item->text(0), wid);

        // Delete the widget from list when destroyed
        connect(wid, &QQuickWindow::destroyed, [=]{
            map_widgetList.remove(item->text(0));

            item->setIcon(0, ico_toggleoff);
            item->setTextColor(0, colorOff);
        });

        item->setIcon(0, ico_toggleon);
        item->setTextColor(0, colorOn);
    }else if(map_widgetList.contains(item->text(0))){
        delete map_widgetList[item->text(0)];
        map_widgetList.remove(item->text(0));

        item->setIcon(0, ico_toggleoff);
        item->setTextColor(0, colorOff);
    }
}

void mainWindow::loadWidgets(){
    // Initialize the variables and load widgets from disk
    obj_widgetList = ui->obj_widgetList;
    ico_toggleoff = QIcon(":/img/toggleoff.png");
    ico_toggleon = QIcon(":/img/toggleon.png");
    colorOn = QColor(0, 230, 0, 255);
#ifndef Q_OS_WIN
    colorOff = QColor(255, 255, 255, 255);
#else
    colorOff = QColor(0, 0, 0, 255);
#endif

    QDir dir(widgetsDir);
    QStringList folderList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i=0; i<folderList.length(); i++){
        QString folder = folderList.at(i);

        QTreeWidgetItem* item = new QTreeWidgetItem(obj_widgetList);
        item->setText(0, folder);
        item->setIcon(0, ico_toggleoff);
    }



    // Enable|Disable widgets by double clicking on them
    connect(obj_widgetList, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem* item) {
        toggleWidget(item);
    });

    // Right Click Menu
    obj_widgetList->setContextMenuPolicy(Qt::CustomContextMenu);
    menu_wlRightClick = new QMenu(this);



    // Right Click Menu Actions
    QAction* act_wlrc_Enable = new QAction("Show", this);
    connect(act_wlrc_Enable, &QAction::triggered, [=]{
        qDebug() << "Enable/Disable the Widget, Label will change (Show/Hide)";

        toggleWidget(obj_widgetList->currentItem());
    });
    QAction* act_wlrc_Edit = new QAction("Edit", this);
    connect(act_wlrc_Edit, &QAction::triggered, [=]{
        qDebug() << "Edit the widget with default editor";

        QDesktopServices::openUrl(QUrl::fromLocalFile(widgetsDir + "/" + obj_widgetList->currentItem()->text(0) + "/main.qml"));
    });
    QAction* act_wlrc_Reload = new QAction("Reload", this);
    connect(act_wlrc_Reload, &QAction::triggered, [=]{
        qDebug() << "Reload  the widget";

        if(map_widgetList.contains(obj_widgetList->currentItem()->text(0)))
            map_widgetList.value(obj_widgetList->currentItem()->text(0))->reload();
    });
    QAction* act_wlrc_Delete = new QAction("Remove", this);
    connect(act_wlrc_Delete, &QAction::triggered, [=]{
        qDebug() << "Just remove from list.";
    });
    QAction* act_wlrc_DeleteDisk = new QAction("Delete From Disk...", this);
    connect(act_wlrc_DeleteDisk, &QAction::triggered, [=]{
        qDebug() << "ASK: Are you sure for delete from disk?";
    });



    // Add Actions to the Menu
    menu_wlRightClick->addAction(act_wlrc_Enable);
    menu_wlRightClick->addAction(act_wlrc_Edit);
    menu_wlRightClick->addAction(act_wlrc_Reload);
    menu_wlRightClick->addAction(act_seperator);
    menu_wlRightClick->addAction(act_wlrc_Delete);
    menu_wlRightClick->addAction(act_wlrc_DeleteDisk);
    connect(obj_widgetList, &QTreeWidget::customContextMenuRequested, [=](const QPoint & pos) {
        QPoint pt(pos);
        pt.setX(pt.x()+4);
        pt.setY(pt.y()+28);

        QTreeWidgetItem *item = obj_widgetList->currentItem();
        if(!map_widgetList.contains(item->text(0))){
            act_wlrc_Enable->setText("Show");
            act_wlrc_Reload->setEnabled(false);
        }else if(map_widgetList.contains(item->text(0))){
            act_wlrc_Enable->setText("Hide");
            act_wlrc_Reload->setEnabled(true);
        }

        menu_wlRightClick->exec( obj_widgetList->mapToGlobal(pt) );
    });
}

void mainWindow::addActionsToTray(){
    // Create Menu & Items
    trayMenu = new QMenu(this);
    QAction* openManager = new QAction("Open Manager...", this);
    QAction* reloadAll = new QAction("Reload All", this);
    act_seperator = new QAction(this);
    act_seperator->setSeparator(true);
    QAction* quit = new QAction("Quit", this);

    trayMenu->addAction(openManager);
    trayMenu->addAction(reloadAll);
    trayMenu->addAction(act_seperator);
    trayMenu->addAction(quit);



    // Connect
    connect(openManager, &QAction::triggered, [=]{
        qDebug() << "show Manager";
        this->setVisible(true);
        this->raise();
        QApplication::setActiveWindow(this);
    });
    connect(reloadAll, &QAction::triggered, [=]{
        for(auto e : map_widgetList.keys())
        {
          map_widgetList.value(e)->reload();
        }
    });
    connect(quit, &QAction::triggered, [=]{
        qDebug() << "Quit";

        // Close the widgets too.
        for(auto e : map_widgetList.keys()){
          delete map_widgetList.value(e);
        }
        QApplication::quit();
    });
}

void mainWindow::addTrayIcon(){
    // Add actions
    addActionsToTray();


    // Set the app icon
    appIcon = new QIcon(":/img/icon.png");
    setWindowIcon(*appIcon);


    // Set the tray
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(*appIcon);
    trayIcon->show();
    connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::DoubleClick){
            qDebug() << "DoubleClick ShowManager";
            this->setVisible(true);
            this->raise();
            QApplication::setActiveWindow(this);
        }
    });

}

void mainWindow::appConfig(){
    // Set directory variables.
    configDir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)[0];
    widgetsDir = configDir + "/widgets";


    // Create the directory if not exists.
    QDir dir(widgetsDir);
    if(!dir.exists()){
        dir.mkpath(widgetsDir + "/example");

        // Copy the example widget to the configDir
        QFile file;
        if(file.copy(":/widgets/example/example.qml", widgetsDir + "/example/main.qml")
        && file.copy(":/widgets/example/test.png", widgetsDir + "/example/test.png")){
            qDebug() << "Example successfully copied.";
        }else qDebug() << "ERR: " << file.errorString();
    }
}

void mainWindow::on_obj_showFolderBtn_clicked()
{
    QDesktopServices::openUrl(widgetsDir);
}
