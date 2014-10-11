#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestPrivate;

class Test : public QObject
{
public:
    explicit Test(QObject *parent = 0);

    void e();


    virtual const QMetaObject * metaObject() const;
    int qt_metacall(QMetaObject::Call call, int, void **);
    virtual void *qt_metacast(const char *name);

signals:

public slots:

private:
    TestPrivate *d;
};

#endif // TEST_H
