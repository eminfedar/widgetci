#include "wqmlfile.h"
#include <QtQml/qqml.h>
#include <QFile>
#include <QMap>
#include <QStandardPaths>

wqmlfile::wqmlfile()
{
    appDataDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0];
    widgetsDir = appDataDir + "/widgets/";
}


QString wqmlfile::readFile(QString widgetName, QString file) const{
    // Checking if parameters are null or empty.
    if(widgetName.isEmpty() || widgetName.isNull()){
        return tr("ERR: 1st parameter is empty or null.") + " (widgetName)";
    }
    if(file.isEmpty() || file.isNull()){
        return tr("ERR: 2nd parameter is empty or null.") + " (file)";
    }


    // Default path for creating file from qml is '%appDataDir%/widgets/%widgetName%'.
    file = widgetsDir + widgetName + "/" + file;


    // Creating the file and opening.
    QFile fi(file);
    if(!fi.open(QFile::ReadWrite | QFile::Text))
        return tr("ERR: Can't access the file\n(maybe some program using it):") + file;

    // Reading.
    QString content = fi.readAll();
    fi.close();

    return content;
}

QString wqmlfile::saveFile(QString widgetName, QString file, QString data) const{
    // Checking if parameters are null or empty.
    if(widgetName.isEmpty() || widgetName.isNull()){
        return tr("ERR: 1st parameter is empty or null.") + " (widgetName)";
    }
    if(file.isEmpty() || file.isNull()){
        return tr("ERR: 2nd parameter is empty or null.") + " (file)";
    }


    // Default path for creating file from qml is '%appDataDir%/widgets/%widgetName%'.
    file = widgetsDir + widgetName + "/" + file;


    // Opening the file
    QFile fi(file);
    if(!fi.open(QFile::WriteOnly | QFile::Text))
        return tr("ERR: Can't access the file\n(maybe some program using it):") + file;

    // Write the file.
    fi.write(data.toUtf8());
    fi.close();

    return "";
}

QString wqmlfile::readFileAnywhere(QString file) const{
    // Checking if parameters are null or empty.
    if(file.isEmpty() || file.isNull()){
        return tr("ERR: 1st parameter is empty or null.") + " (file)";
    }

    // Creating the file and opening.
    QFile fi(file);
    if(!fi.open(QFile::ReadWrite | QFile::Text))
        return tr("ERR: Can't access the file\n(maybe some program using it):") + file;

    // Reading.
    QString content = fi.readAll();
    fi.close();

    return content;
}

QString wqmlfile::saveFileAnywhere(QString file, QString data) const{
    // Checking if parameters are null or empty.
    if(file.isEmpty() || file.isNull()){
        return tr("ERR: 1st parameter is empty or null.") + " (file)";
    }

    // Opening the file
    QFile fi(file);
    if(!fi.open(QFile::WriteOnly | QFile::Text))
        return tr("ERR: Can't access the file\n(maybe some program using it):") + file;

    // Write the file.
    fi.write(data.toUtf8());
    fi.close();

    return "";
}
