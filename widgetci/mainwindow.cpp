#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    //trayIcon->
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::on_pushButton_clicked()
{
    WWidget *testwidget = new WWidget(this);
    testwidget->show();
    widgetsList.append(testwidget);
}
