#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "test.h"
#include <QCoreApplication>
#include <signal.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
        qApp->quit();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    signal(SIGINT, signal_handler);

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    test t;

    return app.exec();
}
