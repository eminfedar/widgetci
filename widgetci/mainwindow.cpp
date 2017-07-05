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

void mainWindow::loadWidgets(){
    obj_widgetList = ui->obj_widgetList;
    ico_toggleoff = QIcon(":/img/toggleoff.png");
    ico_toggleon = QIcon(":/img/toggleon.png");

    QDir dir(widgetsDir);
    QStringList folderList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i=0; i<folderList.length(); i++){
        QString folder = folderList.at(i);

        QTreeWidgetItem* item = new QTreeWidgetItem(obj_widgetList);
        item->setText(0, folder);
        item->setIcon(0, ico_toggleoff);

        map_widgetItemList.insert(folder, item);
    }

    // Enable|Disable widgets by double clicking on them
    connect(obj_widgetList, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem* item) {
        QColor colorOn(0, 230, 0, 255);
        QColor colorOff(255, 255, 255, 255);

        if(item->textColor(0) == colorOn){
            item->setIcon(0, ico_toggleoff);
            item->setTextColor(0, colorOff);
        }else{
            item->setIcon(0, ico_toggleon);
            item->setTextColor(0, colorOn);
        }

    });

    // Right Click Menu
    obj_widgetList->setContextMenuPolicy(Qt::CustomContextMenu);
    menu_wlRightClick = new QMenu(this);

    // Right Click Menu Actions
    QAction* act_wlrc_Enable = new QAction("Show", this);
    connect(act_wlrc_Enable, &QAction::triggered, [=]{
        qDebug() << "Enable/Disable the Widget, Label will change (Show/Hide)";
    });
    QAction* act_wlrc_Edit = new QAction("Edit", this);
    connect(act_wlrc_Edit, &QAction::triggered, [=]{
        qDebug() << "Edit the widget with default editor";
    });
    QAction* act_wlrc_Delete = new QAction("Remove", this);
    connect(act_wlrc_Delete, &QAction::triggered, [=]{
        qDebug() << "Just remove from list.";
    });
    QAction* act_wlrc_DeleteDisk = new QAction("Delete From Disk...", this);
    connect(act_wlrc_DeleteDisk, &QAction::triggered, [=]{
        qDebug() << "ASK: Are you sure for delete from disk?";
    });

    menu_wlRightClick->addAction(act_wlrc_Enable);
    menu_wlRightClick->addAction(act_wlrc_Edit);
    menu_wlRightClick->addAction(act_seperator);
    menu_wlRightClick->addAction(act_wlrc_Delete);
    menu_wlRightClick->addAction(act_wlrc_DeleteDisk);
    connect(obj_widgetList, &QTreeWidget::customContextMenuRequested, [=](const QPoint & pos) {
        QPoint pt(pos);
        pt.setX(pt.x()-9);
        pt.setY(pt.y()+9);

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
    });

    connect(reloadAll, &QAction::triggered, [=]{
        qDebug() << "reloadAll";
    });

    connect(quit, &QAction::triggered, [=]{
        qDebug() << "Quit";
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

    // this is only a basic example for calling a widget
    // it will be expanded to serve as widget manager.
    //WWidget *view = new WWidget(QUrl::fromLocalFile(widgetsDir + "/example/main.qml"));

void mainWindow::on_obj_showFolderBtn_clicked()
{
    QDesktopServices::openUrl(widgetsDir);
}
