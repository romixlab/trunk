#include "pgmnetwork.h"
#include <QDebug>
#include "unistd.h"
#include <QThread>
#include <QCoreApplication>

PGMNetwork::PGMNetwork(QObject *parent) :
    QObject(parent)
{
}

void PGMNetwork::abc()
{
    qDebug() << "abc slot" << QThread::currentThreadId();
}

void PGMNetwork::doWork()
{
    while (1) {
        qDebug() << "working" << QThread::currentThreadId();
        emit sig();
        QCoreApplication::processEvents();
        sleep(1);
    }
}
