#include "test.h"
#include "qmetaobjectbuilder_p.h"
#include <QDebug>

class TestPrivate {
    QMetaObject *meta;
    QByteArray metadata;
    int catchedSignalId;
    QHash<int, QString> slotId2Name;
    QHash<QString, int> signalName2Id;
    friend class Test;
};

Test::Test(QObject *parent) :
    QObject(parent), d(new TestPrivate)
{
    QMetaObjectBuilder builder;
    builder.setClassName("Test");
    d->catchedSignalId = builder.addSignal("catched(QString,QVariantList)").index();
    d->meta = builder.toMetaObject();

    QDataStream ostream(&d->metadata, QIODevice::WriteOnly);
    builder.serialize(ostream);
}

Test::~Test()
{
    if (d->meta)
        free(d->meta);
    delete d;
}

bool Test::addSlot(const char *slot)
{
    QString slotName(&slot[1]);
    if (d->slotId2Name.key(slotName, -1) != -1)
        return false;

    QDataStream istream(d->metadata);
    QMetaObjectBuilder builder;
    QMap<QByteArray, const QMetaObject*> refs;
    builder.deserialize(istream, refs);

    int metaIndex = builder.addSlot(slotName.toLocal8Bit()).index();
    d->slotId2Name.insert(metaIndex + d->meta->methodOffset(), slotName);

    if (d->meta)
        free(d->meta);
    d->meta = builder.toMetaObject();
    QDataStream ostream(&d->metadata, QIODevice::WriteOnly);
    builder.serialize(ostream);

    return true;
}

bool Test::removeSlot(const char *slot)
{
    QString slotName(&slot[1]);
    if (d->slotId2Name.key(slotName, -1) == -1)
        return false;

    QDataStream istream(d->metadata);
    QMetaObjectBuilder builder;
    QMap<QByteArray, const QMetaObject*> refs;
    builder.deserialize(istream, refs);

    int metaIndex = d->slotId2Name.key(slotName);
    builder.removeMethod(metaIndex);
    d->slotId2Name.remove(metaIndex);

    if (d->meta)
        free(d->meta);
    d->meta = builder.toMetaObject();
    QDataStream ostream(&d->metadata, QIODevice::WriteOnly);
    builder.serialize(ostream);

    return true;
}

bool Test::addSignal(const char *signal)
{
    QString signalName(&signal[1]);
    if (d->signalName2Id.value(signalName, -1) != -1)
        return false;

    QDataStream istream(d->metadata);
    QMetaObjectBuilder builder;
    QMap<QByteArray, const QMetaObject*> refs;
    builder.deserialize(istream, refs);

    int metaIndex = builder.addSignal(signalName.toLocal8Bit()).index()
            + d->meta->methodOffset();

    if (d->meta)
        free(d->meta);
    d->meta = builder.toMetaObject();
    QDataStream ostream(&d->metadata, QIODevice::WriteOnly);
    builder.serialize(ostream);

    d->signalName2Id.insert(d->meta->method(metaIndex).name(), metaIndex);

qDebug() << "added sig" << d->meta->method(metaIndex).name();

    return true;
}

void Test::activateSignal(const QString &signal, const QVariantList &args)
{
    int metaIndex = d->signalName2Id.value(signal, -1);
    if (metaIndex == -1) {
        qWarning() << "dynamic signal" << signal << "doesn't exist";
        return;
    }

    QMetaMethod sig = d->meta->method(metaIndex);
    if (sig.parameterCount() != args.count()) {
        qWarning() << "dynamic signal has" << sig.parameterCount() << "args"
                   << args.count() << "provided";
        return;
    }

    QVariantList argsCopy = args;
    void * _a[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < sig.parameterCount(); ++i) {
        if (!argsCopy[i].convert(sig.parameterType(i))) {
            qWarning() << "dynamic signal" << signal << "wrong parameter" << i;
            return;
        }
        _a[i + 1] = argsCopy[i].data();
    }

    QMetaObject::activate(this, d->meta, metaIndex - d->meta->methodOffset(), _a);
}

const QMetaObject *Test::metaObject() const
{
    return d->meta;
}

int Test::qt_metacall(QMetaObject::Call call, int id, void **args)
{
    id = QObject::qt_metacall(call, id, args);
    if (id < 0 || !d->meta)
        return id;

    if (call == QMetaObject::InvokeMetaMethod) {
        int metaIndex = id + d->meta->methodOffset();
        qDebug() << "midx" << metaIndex;
        QMetaMethod method = d->meta->method(metaIndex);
        if (!method.isValid()) {
            qWarning() << "invoked invalid method with id" << id;
            return -1;
        }
        QVariantList vargs;
        for (int i = 0; i < method.parameterCount(); ++i)
            vargs.append(QVariant(method.parameterType(i), args[1 + i]));
        QString catchedName = d->slotId2Name.value(metaIndex);
        qDebug() << "ready to process:" <<  method.name() << vargs << catchedName;
        void *_a[] = { 0,
                       const_cast<void*>(reinterpret_cast<const void*>(&catchedName)),
                       const_cast<void*>(reinterpret_cast<const void*>(&vargs)) };
        QMetaObject::activate(this, metaObject(), d->catchedSignalId, _a);
    }

    return -1;
}

void *Test::qt_metacast(const char *className)
{
    if (!className) return 0;
    //this object should not be castable to anything but QObject
    return QObject::qt_metacast(className);
}
