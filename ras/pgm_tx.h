#ifndef PGM_TX_H
#define PGM_TX_H

#include <QObject>

struct pgm_sock_t;

class pgm_tx : public QObject
{
    Q_OBJECT
public:
    explicit pgm_tx(QObject *parent = 0);

    void setPgmSocket(pgm_sock_t *socket);
    void stop();
signals:
    void finished();

public slots:
    void doWork();

private:
    pgm_sock_t *m_socket;
    quint64 m_out_total;
    quint64 m_msg_sent;

    bool m_is_terminated;
    int	m_terminate_pipe[2];

};

#endif // PGM_TX_H
