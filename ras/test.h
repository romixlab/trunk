#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "pgm_rx.h"

struct pgm_sock_t;

class QThread;

class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = 0);

public slots:
    void onRx(const QByteArray &data);

private slots:
    void onAboutToQuit();
private:
    bool startPGM();

    PGMNetwork *net;
    QThread *thread;

    pgm_sock_t *m_socket;

};

#endif // TEST_H
