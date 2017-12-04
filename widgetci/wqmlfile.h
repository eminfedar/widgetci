#ifndef WQMLFILE_H
#define WQMLFILE_H

#include <QObject>
#include <QQuickItem>

class wqmlfile : public QObject
{
    Q_OBJECT

public:
    wqmlfile();

public slots:
    QString readFile(QString path) const;
    bool saveFile(QString path, QString data) const;
};

#endif // WQMLFILE_H
