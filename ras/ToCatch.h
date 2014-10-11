#ifndef TOCATCH_H
#define TOCATCH_H

#include <QObject>
#include <qdebug.h>

class ToCatch : public QObject
{
    Q_OBJECT
public:
    ToCatch(QObject *parent = 0) : QObject(parent) {}

    void e() { emit sig(); }
signals:
    void sig();

    public slots:
        void slot() {
qDebug() << "normal slot called";
        }

};

#endif // TOCATCH_H

