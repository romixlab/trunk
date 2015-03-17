#include "zmqtest.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>

zmqtest::zmqtest(QObject *parent) :
    QObject(parent)
{
    QTimer::singleShot(0, this, SLOT(run()));
}

void zmqtest::run()
{
    m_context = nzmqt::createDefaultContext(this);
    m_context->start();

    m_socket = m_context->createSocket(nzmqt::ZMQSocket::TYP_PUB, this);
    m_socket->setObjectName("myzmqsocket");
    m_socket->bindTo("tcp://0.0.0.0:7777");

    QTimer *tim = new QTimer(this);
    tim->setInterval(1000);
    tim->setSingleShot(false);
    connect(tim, SIGNAL(timeout()), this, SLOT(sendSomething()));
    tim->start();
}

void zmqtest::sendSomething()
{
    static quint64 counter = 0;

    QList< QByteArray > msg;
    msg += "topic";
    msg += QString("MSG[%1: %2]")
            .arg(++counter)
            .arg(QDateTime::currentDateTime().toLocalTime().toString(Qt::ISODate)).toLocal8Bit();
    try {
    qDebug() << m_socket->sendMessage(msg);
    } catch (std::exception& ex){
        qDebug() << "fail";
    }
    qDebug() << "Publisher> " << msg;
}


