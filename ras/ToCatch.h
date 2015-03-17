#ifndef TOCATCH_H
#define TOCATCH_H

#include <QObject>
#include <qdebug.h>

class ToCatch : public QObject
{
    Q_OBJECT
public:
    ToCatch(QObject *parent = 0) : QObject(parent) {}

    void e() { emit sig(753); }
signals:
    void sig(int a);


public slots:
    void slot() {
        qDebug() << "normal slot called";
    }

    void sl(int a, const QString &b) {
        qDebug() << "slot called" << a << b;
    }

    void s(const QString &name, const QVariantList &v) {
        qDebug() << "catch" << name << v;


    }
    void y(const QString &str) {
        qDebug() << str;
    }
};

#endif // TOCATCH_H

