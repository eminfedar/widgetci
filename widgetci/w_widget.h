#ifndef W_WIDGET_H
#define W_WIDGET_H

#include <QWidget>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptContext>

class QTimer;

namespace Ui {
class WWidget;
}

class WWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WWidget(QWidget *parent = 0);
    ~WWidget();

    QScriptEngine* Engine;
    static WWidget* This();
    static WWidget* s_This;
private:

    Ui::WWidget *ui;

    // Widget itself
    void windowSettings();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool windowDrag = false;
    qint16 dragXsync = 0, dragYsync = 0;


    // Create WidgetElements
    static QScriptValue createLabel(QScriptContext *ctx, QScriptEngine *eng);
    static QScriptValue createButton(QScriptContext *ctx, QScriptEngine *eng);

    // Useful Tools
    static QScriptValue setInterval(QScriptContext *ctx, QScriptEngine *eng);
    static QScriptValue console_log(QScriptContext *ctx, QScriptEngine *eng);

    // Scripts
    void parseJSFile(QString filename);
    QString readJSFile(QString filename);

    static QTimer *currentTimer;
};

#endif // WIDGETSCREEN_H
