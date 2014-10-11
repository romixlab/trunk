#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include "localdiscoveryservice.h"
#include "test.h"

#include <QMetaMethod>

#include "ToCatch.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ToCatch t;
    Test t2;

   // qDebug() << t.metaObject()->methodOffset();
//    for (int i = t.metaObject()->methodOffset(); i < t.metaObject()->methodCount(); ++i) {
//        qDebug() << t.metaObject()->method(i).methodType();
//    }

// //   qDebug() << t2.metaObject()->methodOffset();
//    for (int i = t2.metaObject()->methodOffset(); i < t2.metaObject()->methodCount(); ++i) {
//        qDebug() << t2.metaObject()->method(i).methodType();
//    }

    QObject::connect(&t, SIGNAL(sig()), &t2, SLOT(sl()));
    t.e();

//    LocalDiscoveryService lds;
//    lds.setMulticastGroup(QHostAddress(QCoreApplication::arguments().at(1)));
//    lds.setNodeInfo(QCoreApplication::arguments().at(2), QStringList() << "gr1" << "gr2", "addr");
//    if (!lds.start()) {
//        qWarning() << lds.errorString();
//    }

//    QObject::connect(&lds, &LocalDiscoveryService::nodeFound, [=](const QString &nodeName,
//                     const QStringList &nodeGroups,
//                     const QString &nodeAddress) {
//        qDebug() << nodeName << nodeGroups << nodeAddress;
//    });

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //return app.exec();
}
