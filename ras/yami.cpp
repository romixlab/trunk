#include "yami.h"
#include <QDebug>
#include <QCoreApplication>

Yami::Yami(QObject *parent) :
    QObject(parent)
{
}

void Yami::operator ()(yami::incoming_message &im)
{
    const yami::parameters & content =
            im.get_parameters();
    qDebug() << "msg name" << im.get_message_name().c_str();


        const int value = content.get_integer("value");
        qDebug() << "op";
    emit message(QString("%1").arg(value));
}
