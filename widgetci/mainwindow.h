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
    class wqmlfile;
}

// MAIN MANAGER WINDOWS
class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

    bool CLOSING = false;

private slots:
    void on_obj_showFolderBtn_clicked();
    void on_obj_refreshWidgetListBtn_clicked();

private:
    Ui::mainWindow *ui;

    // Listing the widgets:
    void updateWidgetList(QTreeWidget* widgetList_obj);
    void loadWidgetsToList();
    void toggleWidget(QTreeWidgetItem *item, int x, int y);
    void toggleWidget(QString widgetName, int x, int y);
    QPoint getWidgetSavedCoordinates(QString name);
    void saveWidgetsCoordinates();

    QTreeWidget *obj_widgetList;
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
    QString appDataDir, widgetsDir;

    // Adding apis for qml using. (like file managing in qml)
    void addQmlApis();

    // onStart
    void openWidgetsInFile();

    // onQuit
    void onAppQuit();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
