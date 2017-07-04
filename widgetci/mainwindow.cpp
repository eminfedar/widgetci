#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardPaths>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QDir>
#include <QMenu>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    appConfig();
    addTrayIcon();
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::addTrayIcon(){
    // NOT COMPLETE
    // These actions will be control panel for widgets
    trayMenu = new QMenu(this);
    trayMenu->addAction("Action 1");
    trayMenu->addAction("Action 2");
    trayMenu->addAction("Action 3");
    trayMenu->addAction("Action 4");

    appIcon = new QIcon(":/img/icon.png");
    setWindowIcon(*appIcon);

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

void mainWindow::on_pushButton_clicked()
{
    // this is only a basic example for calling a widget
    // it will be expanded to serve as widget manager.
    WWidget *view = new WWidget(QUrl::fromLocalFile(widgetsDir + "/example/main.qml"));
}
