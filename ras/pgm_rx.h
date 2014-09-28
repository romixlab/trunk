#ifndef PGMNETWORK_H
#define PGMNETWORK_H

#include <QObject>

struct pgm_sock_t;

class PGMNetwork : public QObject
{
    Q_OBJECT
public:
    explicit PGMNetwork(QObject *parent = 0);

    void setPgmSocket(pgm_sock_t *socket);
    void stop();
signals:
    void rx(const QByteArray &data);
    void finished();

public slots:
    void doWork();

private:
    pgm_sock_t *m_socket;
    bool m_is_terminated;
    int	m_terminate_pipe[2];

};

#endif // PGMNETWORK_H
