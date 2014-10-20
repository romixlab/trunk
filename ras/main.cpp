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

    // *** signal catch add remove below ***
//    ToCatch t;
//    Test t2;

//    QObject::connect(&t2, SIGNAL(catched(QString, QVariantList)), &t, SLOT(s(QString, QVariantList)));

//    t2.addSlot(SLOT(my_slot(int)));
//    QObject::connect(&t, SIGNAL(sig(int)), &t2, SLOT(my_slot(int)));
//    t.e();
//    qDebug() << "---------";
//    t2.removeSlot(SLOT(my_slot(int)));
//    t.e();
//    qDebug() << "---------";
//    t2.addSlot(SLOT(my_slot(int)));
//    QObject::connect(&t, SIGNAL(sig(int)), &t2, SLOT(my_slot(int)));
//    t.e();


    ToCatch c;
    Test t;
    t.addSignal(SIGNAL(sig(int,QString)));
    QObject::connect(&t, SIGNAL(sig(int,QString)), &c, SLOT(sl(int,QString)));
    t.activateSignal("sig", QVariantList() << 12.3 << 123);
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
