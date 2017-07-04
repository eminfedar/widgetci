#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wwidget.h"
#include <QSystemTrayIcon>
#include <QMenu>


namespace Ui {
    class mainWindow;
    class WWidget;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private:
    Ui::mainWindow *ui;

    // Tray Menu
    void addTrayIcon();
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QIcon *appIcon;

    // App Config
    void appConfig();
    QString configDir, widgetsDir;


private slots:
    void on_pushButton_clicked();

};

#endif // MAINWINDOW_H
