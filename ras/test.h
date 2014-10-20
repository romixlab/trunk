#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestPrivate;

class Test : public QObject
{
public:
    explicit Test(QObject *parent = 0);
    ~Test();

    /**
     * @brief addSlot create virtual slot
     * @param slot use SLOT(signature)
     * @param catchedName some name that will be in catched signal
     * @return true if slot created
     * Usage:
     * connect(dqobject, SIGNAL(catched(QString,QVariantList)),
     *      reciever, SLOT(processSignal(QString,QVariantList)));
     * dqobject->addSlot(SLOT(my_slot(int)));
     * connect(sender, SIGNAL(sig(int)), dqobject, SLOT(my_slot(int)));
     * when sender->sig will be emitted, dqobject will emit catched(name, args)
     * signal, with name = 'my_slot(int)', args = QVariantList
     */
    bool addSlot(const char *slot);
    /**
     * @brief removeSlot removes previously added slot
     * @param slot SLOT(signature)
     * @return true if removed
     * note that, when you remove the slot, connections to it still exists!
     */
    bool removeSlot(const char *slot);

    bool addSignal(const char *signal);
    void activateSignal(const QString &signal, const QVariantList &args);

    virtual const QMetaObject * metaObject() const;
    int qt_metacall(QMetaObject::Call call, int id, void **args);
    virtual void *qt_metacast(const char *className);

signals:

public slots:

private:
    TestPrivate *d;
};

#endif // TEST_H
