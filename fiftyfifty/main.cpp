#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "singleinstance.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/50.png"));

    QString name = QStringLiteral("com.marsworks.fiftyfifty");

    SingleInstance instance;
    if (instance.hasPrevious(name, QCoreApplication::arguments())) {
        qDebug() << "Previous instance detected!";
        return 0;
    }

    qDebug() << "There can be only one!";
    instance.listen(name);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("singleinstance", &instance);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}
