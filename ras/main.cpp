#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include "localdiscoveryservice.h"
#include "test.h"

#include <QMetaMethod>

#include "ToCatch.h"
#include "zmqtest.h"

#include <qmsgpack/msgpack.h>
#include <yami.h>
#include <yami4-cpp/yami.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);


    Yami y;
    ToCatch c;
    QObject::connect(&y, SIGNAL(message(QString)), &c, SLOT(y(QString)));

yami::agent subscriber_agent;

    try
        {

            // prepare subscription update callback

            const std::string update_object_name =
                "update_handler";

            subscriber_agent.register_object(
                update_object_name, y);

            // subscribe to the producer

            yami::parameters params;
            params.set_string(
                "destination_object", update_object_name);

            std::auto_ptr<yami::outgoing_message> om(subscriber_agent.send("tcp://127.0.0.1:1234",
                "random_number2", "subscribe", params));
            om->wait_for_completion();
            qDebug() << "state err" << (om->get_state() != yami::replied);
            const yami::parameters & content =
                    om->get_reply();
            qDebug() << om->get

            qDebug()
                << "subscribed, waiting for updates";

            // block forever and receive updates in background
                   //int dummy;
                   //std::cin >> dummy;

        }
        catch (const std::exception & e)
        {
               qDebug() << "error: " << e.what();
        }

    // *** signal catch add remove below ***
//    ToCatch t;
//    Test t2;

//    QObject::connect(&t2, SIGNAL(catched(QString, QVariantList)), &t, SLOT(s(QString, QVariantList)));

//    t2.addSlot("my_slot(int)");
//    QObject::connect(&t, SIGNAL(sig(int)), &t2, SLOT(my_slot(int)));
//    t.e();
//    qDebug() << "---------";
//    qDebug() << "remove slot" << t2.removeSlot("my_slot(int)");
//    t.e();
//    qDebug() << "---------";
//    t2.addSlot("my_slot(int)");
//    QObject::connect(&t, SIGNAL(sig(int)), &t2, SLOT(my_slot(int)));
//    t.e();

    // *** signal create below ***
//    ToCatch c;
//    Test t;
//    t.addSignal("sig(int,QString)");
//    QObject::connect(&t, SIGNAL(sig(int,QString)), &c, SLOT(sl(int,QString)));
//    t.activateSignal("sig(int,QString)", QVariantList() << 12.3 << 123);

//    qDebug() << "remove";
//    qDebug() << t.removeSignal("sig(int,QString)");
//    qDebug() << "act after rem";
//    t.activateSignal("sig", QVariantList() << 12.3 << 123);

//    qDebug() << "add 2";
//    t.addSignal("sig(int,QString)");
//    qDebug() << "act after add 2";
//    QObject::connect(&t, SIGNAL(sig(int,QString)), &c, SLOT(sl(int,QString)));
//    t.activateSignal("sig(int,QString)", QVariantList() << 12.3 << 123);

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

    return app.exec();
}
