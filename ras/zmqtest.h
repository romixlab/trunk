#ifndef ZMQTEST_H
#define ZMQTEST_H

#include <QObject>
#include <nzmqt/nzmqt.hpp>

class zmqtest : public QObject
{
    Q_OBJECT
public:
    explicit zmqtest(QObject *parent = 0);

signals:

private slots:
    void sendSomething();
    void run();

private:

    nzmqt::ZMQSocket *m_socket;
    nzmqt::ZMQContext *m_context;
};

#endif // ZMQTEST_H
