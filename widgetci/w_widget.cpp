#include "w_widget.h"
#include "ui_w_widget.h"
#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QMouseEvent>
#include <QMenu>
#include <QPushButton>
#include <QTimer>

QMap<QString, QFrame*> elementList;
uint16_t maxWidth = 10, maxHeight = 10;

WWidget* WWidget::s_This;
WWidget* WWidget::This(){ return s_This; }
QTimer* WWidget::currentTimer = Q_NULLPTR;

WWidget::WWidget(QWidget *parent) :
    QWidget(parent, Qt::Dialog)
{

    s_This = this;
    Engine = new QScriptEngine(this);
    windowSettings();

    parseJSFile(":/script/test.js");
}





// Create Elements
// QLabel
QScriptValue WWidget::createLabel(QScriptContext *ctx, QScriptEngine *eng){
    QString text = ctx->argument(0).toString();
    uint16_t x = ctx->argument(1).toNumber();
    uint16_t y = ctx->argument(2).toNumber();

    QLabel* element = new QLabel(This());
    element->move(x,y);
    element->setText(text);
    element->show();
    if(element->geometry().width() + element->geometry().x() > maxWidth) maxWidth = element->geometry().width() + element->geometry().x();
    if(element->geometry().height() + element->geometry().y() > maxHeight) maxHeight = element->geometry().height() + element->geometry().y();
    return eng->toScriptValue(element);
}

// QPushbutton
QScriptValue WWidget::createButton(QScriptContext *ctx, QScriptEngine *eng){
    QString text = ctx->argument(0).toString();
    uint16_t x = ctx->argument(1).toNumber();
    uint16_t y = ctx->argument(2).toNumber();

    QPushButton* element = new QPushButton(This());
    element->move(x,y);
    element->setText(text);
    element->show();
    if(element->geometry().width() + x > maxWidth) maxWidth = element->geometry().width() + x;
    if(element->geometry().height() + y > maxHeight) maxHeight = element->geometry().height() + y;
    return eng->toScriptValue(element);
}

// QTimer

QScriptValue WWidget::console_log(QScriptContext *ctx, QScriptEngine *eng) {
    qDebug() << ctx->argument(0).toString();
    return QScriptValue();
}

QScriptValue WWidget::setInterval(QScriptContext *ctx, QScriptEngine *eng){
    const QScriptValue func = ctx->argument(0);
    uint32_t milliseconds = ctx->argument(1).toNumber();
    milliseconds = milliseconds>=5?milliseconds:5;

   //  qDebug() << "Func Context: " << func.toString() << "IS FUNCTION?: " << func.isFunction();

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, [func, eng]{
        QScriptValue copy(func);
        copy.call();
        if (eng->hasUncaughtException()) {
            qDebug() << "Exception occured: " << eng->uncaughtException().toString();
        }

    });
    timer->start(milliseconds);

    if (currentTimer != Q_NULLPTR) {
        currentTimer->stop();
        currentTimer->deleteLater();
    }

    currentTimer = timer;

    return eng->toScriptValue(true);
}





// Reading Scripts
void WWidget::parseJSFile(QString filename){
    QString script = readJSFile(filename);
    if(script != ""){
        qDebug() << "Script parsed successfully " << script;

        //engine.globalObject().setProperty("WWidget", );

        // Add Creating Functions
        Engine->globalObject().setProperty("createLabel", Engine->newFunction(createLabel)); // createLabel
        Engine->globalObject().setProperty("createButton", Engine->newFunction(createButton)); // createButton
        Engine->globalObject().setProperty("setInterval", Engine->newFunction(setInterval)); // setInterval
        Engine->globalObject().setProperty("console_log", Engine->newFunction(console_log)); // setInterval

        QScriptProgram program(script + "; main(); ");
        QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(script);
        if (result.state() == QScriptSyntaxCheckResult::Error){
            qDebug() << "Syntax error in script.";
            return;
        }

        // Run the script
        Engine->evaluate(program);
        if (Engine->hasUncaughtException()) {
            qDebug() << "System got exception: " << Engine->uncaughtExceptionBacktrace();
        }


        //engine.abortEvaluation();

        this->setFixedSize(QSize(maxWidth, maxHeight));
    } else {
        qDebug() << "Script is empty.";
    }
}

QString WWidget::readJSFile(QString filename){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream inputStream(&file);
        inputStream.setCodec("UTF-8");

        QString inputString = inputStream.readAll();

        file.close();

        return inputString;
    }
    return "";
}








// Window Events

void WWidget::windowSettings(){
    //setSizeGripEnabled(false);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setParent(0); // Create TopLevel-Widget
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void WWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() & Qt::LeftButton){
        // Start Drag
        windowDrag = true;
        dragXsync = event->x();
        dragYsync = event->y();
    }
    if(event->button() & Qt::RightButton){
        QMenu* contextMenu = new QMenu(this);
        contextMenu->addAction("1st Selection");
        contextMenu->addAction("2nd Selection");
        contextMenu->addAction("3rd Selection");
        contextMenu->addAction("4th Selection");
        contextMenu->popup(event->globalPos());
    }
}

void WWidget::mouseMoveEvent(QMouseEvent *event){
    if(windowDrag){
        move(event->globalX() - dragXsync, event->globalY() - dragYsync);
    }
}

void WWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() & Qt::LeftButton){
        // Stop Drag
        windowDrag = false;
    }
}

WWidget::~WWidget()
{
    delete ui;
    if (currentTimer != Q_NULLPTR)
        currentTimer->deleteLater();
    currentTimer = Q_NULLPTR;
}
