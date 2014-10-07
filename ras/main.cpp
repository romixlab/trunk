#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include "localdiscoveryservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LocalDiscoveryService lds;
    lds.setMulticastGroup(QHostAddress(QCoreApplication::arguments().at(1)));
    lds.setNodeInfo(QCoreApplication::arguments().at(2), QStringList() << "gr1" << "gr2", "addr");
    if (!lds.start()) {
        qWarning() << lds.errorString();
    }

    QObject::connect(&lds, &LocalDiscoveryService::nodeFound, [=](const QString &nodeName,
                     const QStringList &nodeGroups,
                     const QString &nodeAddress) {
        qDebug() << nodeName << nodeGroups << nodeAddress;
    });

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
