#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Prevent multiple instances
    RunGuard guard( "some_random_key" );
        if ( !guard.tryToRun() )
            return 0;


    QApplication a(argc, argv);

    // STYLING.
    #ifdef Q_OS_LINUX || Q_OS_UNIX
        // because in Windows, it's not looking like supposed to be.
        // later can be fixed.
        QString style(
                    "QHeaderView::section, QHeaderView::section *{"
                    "padding: 3px;"
                    "}"
                    ""
                    "QTreeView::item{"
                    "margin-left: 0px;"
                    "padding: 3px;"
                    "}"
                    ""
                    "QMenu::item:selected{"
                    "background-color: rgb(0, 150, 0);"
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


    mainWindow w;
    w.show();

    return a.exec();
}
