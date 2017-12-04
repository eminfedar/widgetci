#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Prevent multiple instances
    RunGuard guard( "widgetci_prevent_double_run" );
    if ( !guard.tryToRun() ) return 0;

    // Define the application
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // STYLING.
    #ifdef Q_OS_LINUX || Q_OS_UNIX
        // because in Windows, it's not looking like supposed to be.
        // later can be fixed.
        QString style(
                    "QHeaderView::section, QHeaderView::section *{"
                    "padding: 3px;"
                    "}"
                    ""
                    ""
                    "QTreeView{ selection-background-color:transparent; }"
                    "QTreeView::item{"
                    "margin-left: 0px;"
                    "padding: 3px;"
                    "}"
                    "QTreeView::item:selected, QTreeView::item:pressed{"
                    "background-color: rgba(0, 150, 0, 80);"
                    "margin:0px;"
                    "padding:0px;"
                    "}"
                    "QTreeView::branch:selected{"
                    "background-color: rgba(0, 0, 0, 0);"
                    "margin:0;"
                    "padding:0;"
                    "}"
                    ""
                    ""
                    "QMenu::item:selected{"
                    "background-color: rgb(0, 150, 0);"
                    "}"
                    ""
                    "QMenu::item:disabled{"
                    "color: #BBBBBB;"
                    "background-color: #393939;"
                    "}"
                    ""
                    "*{"
                    "background: #292929;"
                    "color: #FFFFFF;"
                    "}"
                    ""
                    );
        a.setStyleSheet(style);
    #endif

    // Define the window
    mainWindow w;
    w.show();

    // Start the app
    return a.exec();
}
