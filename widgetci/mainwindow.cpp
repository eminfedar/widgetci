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
#include <QMessageBox>

#include <QtQml/qqml.h>
#include "wqmlfile.h"

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainWindow){
    ui->setupUi(this);

    appConfig();
    addTrayIcon();
    addQmlApis();

    addTreeRightClickMenu();
    openWidgetsInFile();
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::addQmlApis(){
    qmlRegisterType<wqmlfile>("com.widgetci.file", 1, 0, "WFile");
}

void mainWindow::openWidgetsInFile(){
    widgetsDataSettings = new QSettings(appDataDir + "/widgets.ini", QSettings::IniFormat);

    int count = 0;
    for(auto e : widgetsDataSettings->childGroups()){
        widgetsDataSettings->beginGroup(e);
        int wx = widgetsDataSettings->value("x", -1000).toInt();
        int wy = widgetsDataSettings->value("y", -1000).toInt();
        bool visible = widgetsDataSettings->value("visible", false).toBool();
        widgetsDataSettings->endGroup();

        if(visible){
            toggleWidget(e, wx, wy);
            count++;
        }
    }
    if(count == 0) this->show(); // if no widgets visible, show the form.
}

// Toggle widgets
void mainWindow::toggleWidget(QTreeWidgetItem *item, int wx = -1000, int wy = -1000){
    QString wid_filename = item->text(0);

    // If widget not exists. Create one.
    if(!map_widgetList.contains(wid_filename)){

        // Load configs about the widget
        QMap<QString, QVariant> widConf = getWidgetSettings(item->text(0));

        //Check if has spesific coordinates
        if(wx == -1000 || wy == -1000){
            wx = widConf["x"].toInt();
            wy = widConf["y"].toInt();
        }

        // Add the widget
        WWidget *wid = new WWidget(QUrl::fromLocalFile(widgetsDir + "/" + wid_filename + "/main.qml"), wid_filename, wx, wy);
        wid->toggleZPos(widConf["z-pos"].toInt());
        wid->toggleLock(widConf["lock"].toBool());
        wid->widgetSettings = widgetsDataSettings;


        // Check for errors. (File not found etc...)
        if( wid->status() == QQuickView::Error ){

            QString errorString = "";
            for (int i = 0; i < wid->errors().length(); ++i) {
                errorString.append(wid->errors().at(i).toString());
            }
            errorString.append(tr("\n\nPlease try to fix the error and refresh the list"));

            item->setBackground(0, QBrush(QColor(160,0,0,255)));
            item->setTextColor(0, QColor(180, 180, 180, 255));
            item->setSelected(false);

            QMessageBox::warning(this, tr("An error occured"), errorString, QMessageBox::Ok);
            return;
        }else if(wid->status() == QQuickView::Ready){
            item->setBackground(0, QBrush(QColor(0,0,0,0)));
        }


        map_widgetList.insert(wid->filename, wid);

        item->setIcon(0, ico_toggleon);
        item->setTextColor(0, colorOn);

        // Delete the widget from list when destroyed
        disconnect(wid, &QQuickWindow::destroyed, 0, 0);
        connect(wid, &QQuickWindow::destroyed, [=]{
            if(map_widgetList.contains(wid->filename)){
                if(!CLOSING) saveWidgetSettings(wid->filename); // Save settings before closing the widgets.

                map_widgetList.remove(wid->filename);

                QList<QTreeWidgetItem *> item_lst = obj_widgetList->findItems(wid->filename, Qt::MatchExactly, 0);
                if(item_lst.length() > 0){
                    item_lst[0]->setTextColor(0, colorOff);
                    item_lst[0]->setIcon(0, ico_toggleoff);
                }
            }
        });

    }else if(map_widgetList.contains(wid_filename)){
        delete map_widgetList[wid_filename];
    }
}
void mainWindow::toggleWidget(QString widgetName, int wx = -1000, int wy = -1000){
    QList <QTreeWidgetItem *> items = obj_widgetList->findItems(widgetName, Qt::MatchExactly, 0);
    if(items.length() > 0){
        toggleWidget(items[0], wx, wy);
    }
}

QMap<QString, QVariant> mainWindow::getWidgetSettings(QString widgetname){
    QMap<QString, QVariant> mapTmp;

    widgetsDataSettings->beginGroup(widgetname);
    mapTmp["x"] = widgetsDataSettings->value("x", -1000);
    mapTmp["y"] = widgetsDataSettings->value("y", -1000);
    mapTmp["visible"] = widgetsDataSettings->value("visible", false);
    mapTmp["z-pos"] = widgetsDataSettings->value("z-pos", 0);
    mapTmp["lock"] = widgetsDataSettings->value("lock", false);
    widgetsDataSettings->endGroup();

    return mapTmp;
}

void mainWindow::saveWidgetSettings(QString widgetname){
    widgetsDataSettings->beginGroup(widgetname);
    widgetsDataSettings->setValue("x", map_widgetList[widgetname]->x());
    widgetsDataSettings->setValue("y", map_widgetList[widgetname]->y());
    widgetsDataSettings->setValue("visible", map_widgetList[widgetname]->isVisible());
    widgetsDataSettings->setValue("z-pos", map_widgetList[widgetname]->z_pos);
    widgetsDataSettings->setValue("lock", map_widgetList[widgetname]->lock);
    widgetsDataSettings->endGroup();
    widgetsDataSettings->sync();
}

void mainWindow::updateWidgetList(QTreeWidget* widgetList_obj){
    QMap<QString, WWidget *> tmpWidgetsMap(map_widgetList);
    widgetList_obj->clear();

    QDir dir(widgetsDir);
    QStringList folderList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Close all widgets (to open just available ones later)
    for(QString e : map_widgetList.keys()){
        delete map_widgetList.value(e);
    }

    // Add folders one by one
    for(int i=0; i<folderList.length(); i++){
        QString folder = folderList.at(i);

        QTreeWidgetItem* item = new QTreeWidgetItem(widgetList_obj);
        item->setText(0, folder);
        item->setIcon(0, ico_toggleoff);

        if(tmpWidgetsMap.contains(folder)){
            toggleWidget(folder);
            //item->setIcon(0, ico_toggleon);
        }
    }

    // Enable|Disable widgets by double clicking on themauto
    disconnect(widgetList_obj, &QTreeWidget::itemActivated, 0, 0); // prevent multiple connections & runs
    connect(widgetList_obj, &QTreeWidget::itemActivated, [=](QTreeWidgetItem* item) {
        toggleWidget(item);
    });
}

void mainWindow::addTreeRightClickMenu(){
    // Initialize the variables
    ico_toggleoff = QIcon(":/img/toggleoff.png");
    ico_toggleon = QIcon(":/img/toggleon.png");
    colorOn = QColor(0, 230, 0, 255);
    colorOff = QColor(255, 255, 255, 255);


    // QTreeWidget list object from ui:
    obj_widgetList = ui->obj_widgetList;

    // Update & Refresh the widgets
    updateWidgetList(obj_widgetList);

    // Right Click Menu
    obj_widgetList->setContextMenuPolicy(Qt::CustomContextMenu);
    menu_wlRightClick = new QMenu(this);

    // Right Click Menu Actions
    QAction* act_wlrc_ShowHide = new QAction(tr("Show"), this);
    connect(act_wlrc_ShowHide, &QAction::triggered, [=]{
        // Show/Hide Action
        toggleWidget(obj_widgetList->currentItem());
    });
    QAction* act_wlrc_Edit = new QAction(tr("Edit"), this);
    connect(act_wlrc_Edit, &QAction::triggered, [=]{
        // Edit the widget file with default editor
        QDesktopServices::openUrl(QUrl::fromLocalFile(widgetsDir + "/" + obj_widgetList->currentItem()->text(0) + "/main.qml"));
    });
    QAction* act_wlrc_Reload = new QAction(tr("Reload"), this);
    connect(act_wlrc_Reload, &QAction::triggered, [=]{
        // Reload the widget
        if(map_widgetList.contains(obj_widgetList->currentItem()->text(0)))
            map_widgetList.value(obj_widgetList->currentItem()->text(0))->reload();
    });
    QAction* act_wlrc_RefreshTheList = new QAction(tr("Refresh the list"), this);
    connect(act_wlrc_RefreshTheList, &QAction::triggered, [=]{
        // Refresh all the list.
        updateWidgetList(obj_widgetList);
    });
    QAction* act_wlrc_Delete = new QAction(tr("Remove"), this);
    connect(act_wlrc_Delete, &QAction::triggered, [=]{
        if(map_widgetList.contains(obj_widgetList->currentItem()->text(0)))
            toggleWidget(obj_widgetList->currentItem()->text(0));
        delete obj_widgetList->currentItem();
        obj_widgetList->clearSelection();
    });
    QAction* act_wlrc_DeleteDisk = new QAction(tr("Delete From Disk..."), this);
    connect(act_wlrc_DeleteDisk, &QAction::triggered, [=]{
        int answer = QMessageBox::warning(this, tr("Warning"), tr("The '%1' folder will be deleted from disk.\n\nAre you sure?").arg(obj_widgetList->currentItem()->text(0)), QMessageBox::Cancel | QMessageBox::Ok);
        if(answer == QMessageBox::Ok){
            QDir folder(widgetsDir + "/" + obj_widgetList->currentItem()->text(0));
            if(folder.removeRecursively()){
                QMessageBox::information(this, tr("Information"), tr("The folder has been deleted successfully."), QMessageBox::Ok);
                if(map_widgetList.contains(obj_widgetList->currentItem()->text(0))){
                    toggleWidget(obj_widgetList->currentItem()->text(0));
                    updateWidgetList(obj_widgetList);
                }

            }else{
                QMessageBox::critical(this, tr("Information"), tr("The folder couldn't be removed.\nCheck the folder and the files may be using by another program."), QMessageBox::Ok);
            }


        }
    });



    // Add Actions to the QTreeWidget's Right Click Menu
    menu_wlRightClick->addAction(act_wlrc_ShowHide);
    menu_wlRightClick->addSeparator();
    menu_wlRightClick->addAction(act_wlrc_Reload);
    menu_wlRightClick->addAction(act_wlrc_Edit);
    menu_wlRightClick->addAction(act_wlrc_RefreshTheList);
    menu_wlRightClick->addSeparator();
    menu_wlRightClick->addAction(act_wlrc_Delete);
    menu_wlRightClick->addAction(act_wlrc_DeleteDisk);
    connect(obj_widgetList, &QTreeWidget::customContextMenuRequested, [=](const QPoint & pos) {
        QPoint pt(pos);
        pt.setX(pt.x()+2);
        pt.setY(pt.y()+22);

        QTreeWidgetItem *item = obj_widgetList->currentItem();
        QString wid_filename = item->text(0);
        if(!map_widgetList.contains(wid_filename)){
            act_wlrc_ShowHide->setText(tr("Show"));
            act_wlrc_Reload->setEnabled(false);
        }else if(map_widgetList.contains(wid_filename)){
            act_wlrc_ShowHide->setText(tr("Hide"));
            act_wlrc_Reload->setEnabled(true);
        }

        menu_wlRightClick->exec( obj_widgetList->mapToGlobal(pt) );
    });
}

void mainWindow::addActionsToTray(){
    // Create Menu & Items
    trayMenu = new QMenu(this);
    QAction* openManager = new QAction(tr("Open Manager..."), this);
    QAction* reloadAll = new QAction(tr("Reload All"), this);
    QAction* quit = new QAction(tr("Quit"), this);

    trayMenu->addAction(openManager);
    trayMenu->addAction(reloadAll);
    trayMenu->addSeparator();
    trayMenu->addAction(quit);



    // Connect
    connect(openManager, &QAction::triggered, [=]{
        this->setVisible(true);
        this->raise();
        QApplication::setActiveWindow(this);
    });
    connect(reloadAll, &QAction::triggered, [=]{
        for(auto e : map_widgetList.keys()){
          map_widgetList.value(e)->reload();
        }
    });
    connect(quit, &QAction::triggered, [=]{
        onAppQuit();
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
            this->setVisible(true);
            this->raise();
            QApplication::setActiveWindow(this);
        }
    });

}

void mainWindow::appConfig(){
    // Set directory variables.
    appDataDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0];
    widgetsDir = appDataDir + "/widgets";

    // Create the directory if not exists.
    QDir dir(widgetsDir);
    if(!dir.exists()){
        QDir localdir(":/widgets/defaultwidgets/");
        QStringList folderList = localdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        // Copy the local folders one by one
        QFile file;
        for(int i=0; i<folderList.length(); i++){
            QString folder = folderList.at(i);

            //qDebug() << localdir.path() + folder << widgetsDir + "/" + folder;
            dir.mkpath(widgetsDir + "/" + folder);

            // Copy the files inside folder
            QDir filedir(localdir.path() + "/" + folder);
            QStringList filelist = filedir.entryList(QDir::Files | QDir::NoDotAndDotDot);
            for (int a = 0; a < filelist.length(); ++a) {
                file.copy(localdir.path() + "/" + folder + "/" + filelist.at(a), widgetsDir + "/" + folder + "/" + filelist.at(a));
            }
        }
    }
}

void mainWindow::on_obj_showFolderBtn_clicked(){
    QDesktopServices::openUrl(widgetsDir);
}

void mainWindow::on_obj_refreshWidgetListBtn_clicked(){
    updateWidgetList(ui->obj_widgetList);
}

// Before the app quit
void mainWindow::onAppQuit(){
    CLOSING = true;

    // Close the widgets too.
    for(auto e : map_widgetList.keys()){
        // Save widget configs.
        saveWidgetSettings(e);

        // Destroy them.
        delete map_widgetList.value(e);
    }
    QApplication::quit();
}

// Window Close Event
void mainWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    this->setVisible(false);
}
