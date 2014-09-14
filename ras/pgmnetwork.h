#ifndef PGMNETWORK_H
#define PGMNETWORK_H

#include <QObject>

class PGMNetwork : public QObject
{
    Q_OBJECT
public:
    explicit PGMNetwork(QObject *parent = 0);

signals:
    void sig();

public slots:
    void abc();
    void doWork();

};

#endif // PGMNETWORK_H
