#include "dynamicqobject.h"
#include "QDebug"
#include <QMetaMethod>
#include <QVariant>

DynamicQObject::DynamicQObject(QObject *mapToObject, const char *signalCatchMethod, QObject *parent) :
    QObject(parent), m_mapTo(mapToObject), m_catchMethod(signalCatchMethod)
{
    m_parameters[0] = 0;
}


bool DynamicQObject::addSlot(QObject *object,
                             const char *signal,
                             const QString &slotName)
{
    if (containsSlot(slotName))
        return false;
    if (signal[0] != '2') {
        qWarning() << "Use SIGNAL() macro";
        return false;
    }

    QByteArray theSignal = QMetaObject::normalizedSignature(&signal[1]);
    int signalId = object->metaObject()->indexOfSignal(theSignal);
    if (signalId < 0) {
        qWarning() << "signal" << signal << "doesn't exist";
        return false;
    }

    QVector<int> parameterTypes;
    QMetaMethod signalMethod = object->metaObject()->method(signalId);
    for (int i = 0; i < signalMethod.parameterCount(); ++i)
        parameterTypes.push_back(signalMethod.parameterType(i));

    int slotIdx = -1;
    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].isEmpty == true) {
            slotIdx = i;
            break;
        }
    }
    bool addEntry = false;
    if (slotIdx == -1) {
        addEntry = true;
        slotIdx = m_slotList.count();
    }

    if (!QMetaObject::connect(object, signalId,
                                this, slotIdx + metaObject()->methodCount())) {
        qWarning() << "can't connect" << signal << "signal to virtual slot";
        return false;
    }

    if (addEntry) {
        m_slotList.push_back({false, object, signalId, slotName, parameterTypes});
    } else {
        slot_t &slot = m_slotList[slotIdx];
        slot.isEmpty = false;
        slot.object = object;
        slot.signalIdx = signalId;
        slot.name = slotName;
        slot.parameterTypes = parameterTypes;
    }

    return true;
}

bool DynamicQObject::removeSlot(const QString &name)
{
    int slotIdx = -1;
    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].name == name) {
            slotIdx = i;
            break;
        }
    }
    if (slotIdx == -1)
        return false;
    slot_t &slot = m_slotList[slotIdx];
    if (QMetaObject::disconnect(slot.object, slot.signalIdx,
                                this, slotIdx + metaObject()->methodCount())) {
        if (slotIdx == m_slotList.count() - 1)
            m_slotList.remove(slotIdx, 1);
        else
            slot.isEmpty = true;
        return true;
    } else {
        return false;
    }
}


int DynamicQObject::qt_metacall(QMetaObject::Call call, int id, void **arguments)
{
    id = QObject::qt_metacall(call, id, arguments);
    if (id < 0 || call != QMetaObject::InvokeMetaMethod)
        return id;
    Q_ASSERT(id < m_slotList.size());

    const slot_t &slotInfo = m_slotList[id];
    QVariantList parameters;
    for (int i = 0; i < slotInfo.parameterTypes.count(); ++i) {
        void *parameter = arguments[i + 1];
        parameters.append(QVariant(slotInfo.parameterTypes[i], parameter));
    }

    QMetaObject::invokeMethod(m_mapTo, m_catchMethod,
                              Q_ARG(QString, slotInfo.name),
                              Q_ARG(QVariantList, parameters));
    return -1;
}

bool DynamicQObject::containsSlot(const QString &name)
{
    for (int i = 0; i < m_slotList.count(); ++i)
        if (m_slotList[i].name == name)
            return true;
    return false;
}

bool DynamicQObject::addSignal(const QString &name, QObject *object, const char *slot)
{
    if (slot[0] != '1') {
        qWarning() << "Use SLOT() macro";
        return false;
    }
    int slotIdx = object->metaObject()->
            indexOfSlot(&slot[1]); // without 1 added by SLOT() macro
    if (slotIdx < 0) {
        qWarning() << slot << "slot didn't exist";
        return false;
    }

    QVector<int> parameterTypes;
    QMetaMethod slotMethod = object->metaObject()->method(slotIdx);
    for (int i = 0; i < slotMethod.parameterCount(); ++i)
        parameterTypes.push_back(slotMethod.parameterType(i));

    int signalIdx = -1;
    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].isEmpty == true) {
            signalIdx = i;
            break;
        }
    }
    bool addEntry = false;
    if (signalIdx == -1) {
        addEntry = true;
        signalIdx = m_signalList.count();
    }

    if (!QMetaObject::connect(this, signalIdx + metaObject()->methodCount(),
                              object, slotIdx)) {
        qWarning() << "can't connect virtual signal" << name << "to slot" << slot;
        return false;
    }

    if (addEntry) {
        m_signalList.append({false, object, slotIdx, name, parameterTypes});
    } else {
        signal_t &signal = m_signalList[signalIdx];
        signal.isEmpty = false;
        signal.reciever = object;
        signal.slotIdx = slotIdx;
        signal.name = name;
        signal.parameterTypes = parameterTypes;
    }
    m_signalHash.insert(name, signalIdx);

    return true;
}

bool DynamicQObject::removeSignal(const QString &name)
{
    int signalIdx = -1;
    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].name == name) {
            signalIdx = i;
            break;
        }
    }
    if (signalIdx == -1)
        return false;

    signal_t &signal = m_signalList[signalIdx];
    if (QMetaObject::disconnect(this, signalIdx + metaObject()->methodCount(),
                                signal.reciever, signal.slotIdx)) {
        m_signalHash.remove(signal.name);
        if (signalIdx == m_slotList.count() - 1)
            m_slotList.remove(signalIdx, 1);
        else
            signal.isEmpty = true;
        return true;
    } else {
        return false;
    }
}


bool DynamicQObject::activate(const QString &signalName, const QVariantList &args)
{
    int signalIdx = m_signalHash.value(signalName, -1);
    if (signalIdx == -1) {
        qWarning() << "signal" << signalName << "doesn't exist";
        return false;
    }

    signal_t &signal = m_signalList[signalIdx];

    if (args.count() < signal.parameterTypes.count()) {
        qWarning() << "parameters count mismatch:" << signalName
                   << "provided:" << args.count()
                   << "need >=:" << signal.parameterTypes.count();
        return false;
    }

    QVariantList argsCopy = args;

    for (int i = 0; i < signal.parameterTypes.count(); ++i) {
        if (!argsCopy[i].convert(signal.parameterTypes[i])) {
            qWarning() << "can't cast parameter" << i << signalName;
            return false;
        }
        m_parameters[i + 1] = argsCopy[i].data();
    }


    QMetaObject::activate(this, metaObject(),
                          signalIdx + metaObject()->methodCount(),
                          m_parameters);
}

void DynamicQObject::pr()
{
    for (int i = 0; i < m_slotList.count(); ++i) {
        qDebug() << "slot" << i << m_slotList[i].isEmpty;
        if (m_slotList[i].isEmpty == false) {
            qDebug() << m_slotList[i].name;
        }
    }
}





