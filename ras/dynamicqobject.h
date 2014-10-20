#ifndef DYNAMICQOBJECT_H
#define DYNAMICQOBJECT_H

#include <QObject>
#include <QHash>
#include <QByteArray>
#include <QVector>
#include "sh_global.h"

class SHSHARED_EXPORT DynamicQObject : public QObject
{
public:
    DynamicQObject(QObject *mapToObject,
                   const char *signalCatchMethod,
                   QObject *parent = 0);

    bool addSlot(QObject *object,
                 const char *signal,
                 const QString &slotName);
    bool removeSlot(const QString &name);

    bool addSignal(const QString &name, QObject *object, const char *slot);
    bool removeSignal(const QString &name);
    bool activate(const QString &signalName, const QVariantList &args);

    int qt_metacall(QMetaObject::Call call, int id, void **arguments);

    void pr();
private:
    // virtual slots
    bool containsSlot(const QString &name);
    QObject *m_mapTo;
    const char *m_catchMethod;
    typedef struct {
        bool isEmpty; // true after removeSlot()
        QObject *object;
        int signalIdx;
        QString name; // virtual slot name
        QVector<int> parameterTypes;
    } slot_t;
    QVector<slot_t> m_slotList;

    // virtual signals
    typedef struct {
        bool isEmpty; // // true after removeSignal()
        QObject *reciever;
        int slotIdx;
        QString name;
        QVector<int> parameterTypes;
    } signal_t;
    QVector<signal_t> m_signalList;
    QHash<QString, int> m_signalHash;
    void *m_parameters[11]; // max 10 parameters + ret value
};

#endif // DYNAMICQOBJECT_H
