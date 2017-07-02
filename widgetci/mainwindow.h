#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "w_widget.h"
#include <QSystemTrayIcon>
#include <QMenu>


namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::mainWindow *ui;
    QVector<WWidget*> widgetsList;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    //QIcon *trayIcon;
};

#endif // MAINWINDOW_H
