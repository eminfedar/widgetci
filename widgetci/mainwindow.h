#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wwidget.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QIcon>
#include <QColor>


namespace Ui {
    class mainWindow;
    class WWidget;
}

// MAIN MANAGER WINDOWS
class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:
    void on_obj_showFolderBtn_clicked();

private:
    Ui::mainWindow *ui;

    // Listing the widgets:
    void loadWidgets();
    void toggleWidget(QTreeWidgetItem *item);
    QTreeWidget *obj_widgetList;
    QMap<QString, QTreeWidgetItem *> map_widgetItemList;
    QMap<QString, WWidget *> map_widgetList;
    QIcon ico_toggleoff, ico_toggleon;
    QMenu *menu_wlRightClick; // menu_widgetlistRightClick
    QColor colorOn;
    QColor colorOff;

    // Tray Menu
    void addTrayIcon();
    void addActionsToTray();
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QIcon *appIcon;
    QAction *act_seperator;

    // App Config
    void appConfig();
    QString configDir, widgetsDir;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
