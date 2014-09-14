#include "test.h"
#include <QThread>
#include <QTimer>
#include <QDebug>

test::test(QObject *parent) :
    QObject(parent)
{
    net = new PGMNetwork;
    thread = new QThread;
    net->moveToThread(thread);

    connect(net, SIGNAL(sig()), this, SLOT(def()));
    connect(thread, SIGNAL(started()), net, SLOT(doWork()));
    connect(this, SIGNAL(mysig()), net, SLOT(abc()));

    thread->start();

    QTimer *t = new QTimer(this);
    t->setInterval(2000);
    t->setSingleShot(false);
    connect(t, SIGNAL(timeout()), this, SLOT(timer()));
    t->start();
}

void test::def()
{
    qDebug() << "def called"  << QThread::currentThreadId();
}

void test::timer()
{
    qDebug() << "timer";
    emit mysig();
}
