#include "wqmlfile.h"
#include <QtQml/qqml.h>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include "wwidget.h"

wqmlfile::wqmlfile()
{}


QString wqmlfile::readFile(QString file) const{
    // Creating the file and opening.
    QFile fi(file);
    if(!fi.open(QFile::ReadWrite | QFile::Text))
        return "ERROR: The file isn't readable:" + file;

    // Reading.
    QString returnString = "";
    QTextStream in(&fi);
    while(!in.atEnd()){
        returnString += in.readLine();
    }

    return returnString;
}

bool wqmlfile::saveFile(QString file, QString data) const{
    // Opening the file
    QFile fi(file);
    if(!fi.open(QFile::WriteOnly | QFile::Text))
        return false;

    // Write the file.
    QTextStream out(&fi);
    out << data;

    return true;
}
