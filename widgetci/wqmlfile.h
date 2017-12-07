#ifndef WQMLFILE_H
#define WQMLFILE_H

#include <QObject>
#include <QQuickItem>

class wqmlfile : public QObject
{
    Q_OBJECT

public:
    wqmlfile();

    QString appDataDir, widgetsDir;

public slots:
    QString readFile(QString widgetName, QString path) const;
    QString saveFile(QString widgetName, QString path, QString data) const;

    QString readFileAnywhere(QString path) const;
    QString saveFileAnywhere(QString path, QString data) const;
};

#endif // WQMLFILE_H
