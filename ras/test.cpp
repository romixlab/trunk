#include "test.h"
#include "qmetaobjectbuilder_p.h"
#include <QDebug>

class TestPrivate {
    QMetaObject *meta;
    friend class Test;
};

Test::Test(QObject *parent) :
    QObject(parent), d(new TestPrivate)
{
    QMetaObjectBuilder b;
    b.setClassName("Test");
    qDebug() << b.addSlot("sl()").index();
    d->meta = b.toMetaObject();
}

void Test::e()
{
    void *args[] = {0};
    QMetaObject::activate(this, metaObject(), 0, args);
}

const QMetaObject *Test::metaObject() const
{
    //qDebug() << "in metaObject";
    return d->meta;
}

int Test::qt_metacall(QMetaObject::Call call, int, void **)
{
    qDebug() << "in qt_metacall";
    qDebug() << call;
    return 0;
}

void *Test::qt_metacast(const char *name)
{
    qDebug() << "in qt_metacast";
    return QObject::qt_metacast(name);
}
