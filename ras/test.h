#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "pgmnetwork.h"

class QThread;

class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = 0);

signals:
    void mysig();

public slots:
    void def();
    void timer();


private:
    PGMNetwork *net;
    QThread *thread;

};

#endif // TEST_H
