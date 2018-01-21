#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    // Prevent multiple instances
    RunGuard guard( "widgetci_prevent_double_run" );
    if ( !guard.tryToRun() ) return 0;

    // Define the application
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // App configs
    QCoreApplication::setOrganizationName("Widgetci");
    QCoreApplication::setOrganizationDomain("widgetci.org");
    QCoreApplication::setApplicationName("Widgetci");

    // STYLING.
    QString style(
                "QHeaderView::section, QHeaderView::section *{"
                "background: #292929;"
                "padding: 3px;"
                "border: 1px solid #202020;"
                "}"
                ""
                ""
                "QTreeView{ selection-background-color:transparent; border: 1px solid #202020; }"
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
    app.setStyleSheet(style);


    // CHECK IF TRANSLATION AVAILABLE
    QTranslator translator;
    QString lang = QLocale::system().name().left(2);
    QString langStr = QLocale::system().languageToString(QLocale::system().language());

    if(lang != "en"){
        if(translator.load(":/translations/translations/widgetci_" + lang)){
            app.installTranslator(&translator);
        }else{
            qDebug() << "There is no available translations for your language.";
            qDebug() << "You can translate this program to " << langStr << " VISIT ==> https://github.com/eminfedar/widgetci/tree/master/widgetci/translations";
        }
    }

    // Define the window
    mainWindow window;
    //w.show(); // not show if has opened widgets. but if hasn't, inside the constructor, widgetManager will be shown.

    // Start the app loop
    return app.exec();
}
