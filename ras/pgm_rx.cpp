#include "pgm_rx.h"
#include <QDebug>
#include "unistd.h"
#include <QThread>
#include <QCoreApplication>
#include <QByteArray>
#include <pgm/pgm.h>


PGMNetwork::PGMNetwork(QObject *parent) :
    QObject(parent), m_is_terminated(false)
{
    pipe(m_terminate_pipe);
}

void PGMNetwork::setPgmSocket(pgm_sock_t *socket)
{
    m_socket = socket;
}

void PGMNetwork::stop()
{
    m_is_terminated = true;
    const char one = '1';
    const size_t writelen = write (m_terminate_pipe[1], &one, sizeof(one));
    if (writelen != sizeof(one))
        qWarning() << "write to m_terminate_pipe[1] failed";
}

void PGMNetwork::doWork()
{
    /* dispatch loop */
#ifndef _WIN32
    int fds;
    fd_set readfds;


#else
    SOCKET recv_sock, pending_sock;
    DWORD cEvents = PGM_RECV_SOCKET_READ_COUNT + 1;
    WSAEVENT waitEvents[ PGM_RECV_SOCKET_READ_COUNT + 1 ];
    socklen_t socklen = sizeof (SOCKET);

    waitEvents[0] = terminateEvent;
    waitEvents[1] = WSACreateEvent();
    waitEvents[2] = WSACreateEvent();
    assert (2 == PGM_RECV_SOCKET_READ_COUNT);
    pgm_getsockopt (sock, IPPROTO_PGM, PGM_RECV_SOCK, &recv_sock, &socklen);
    WSAEventSelect (recv_sock, waitEvents[1], FD_READ);
    pgm_getsockopt (sock, IPPROTO_PGM, PGM_PENDING_SOCK, &pending_sock, &socklen);
    WSAEventSelect (pending_sock, waitEvents[2], FD_READ);

    static WSAEVENT		terminateEvent;
#endif /* !_WIN32 */

    pgm_error_t* pgm_err = NULL;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    qDebug() << "Entering PGM message loop..." << QThread::currentThreadId();

    do {

#ifdef _WIN32
        DWORD dwTimeout, dwEvents;
#endif
        char buffer[4096];
        size_t len;
        struct pgm_sockaddr_t from;
        socklen_t fromlen = sizeof (from);
        const int status = pgm_recvfrom (m_socket,
                                         buffer,
                                         4096,
                                         0,
                                         &len,
                                         &from,
                                         &fromlen,
                                         &pgm_err);
        switch (status) {
        case PGM_IO_STATUS_NORMAL:
        {
            QByteArray ba(buffer, len);
            emit rx(ba);

            break;
        }
        case PGM_IO_STATUS_TIMER_PENDING:
        {
            socklen_t optlen = sizeof (tv);
            pgm_getsockopt (m_socket, IPPROTO_PGM, PGM_TIME_REMAIN, &tv, &optlen);
        }
            goto block;
        case PGM_IO_STATUS_RATE_LIMITED:
        {
            socklen_t optlen = sizeof (tv);
            pgm_getsockopt (m_socket, IPPROTO_PGM, PGM_RATE_REMAIN, &tv, &optlen);
        }
        case PGM_IO_STATUS_WOULD_BLOCK:
            /* select for next event */
block:
#ifndef _WIN32
            fds = m_terminate_pipe[0] + 1;
            FD_ZERO(&readfds);
            FD_SET(m_terminate_pipe[0], &readfds);
            pgm_select_info (m_socket, &readfds, NULL, &fds);
            fds = select (fds, &readfds, NULL, NULL, PGM_IO_STATUS_WOULD_BLOCK == status ? NULL : &tv);
#else
            dwTimeout = PGM_IO_STATUS_WOULD_BLOCK == status ? WSA_INFINITE : (DWORD)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
            dwEvents = WSAWaitForMultipleEvents (cEvents, waitEvents, FALSE, dwTimeout, FALSE);
            switch (dwEvents) {
            case WSA_WAIT_EVENT_0+1: WSAResetEvent (waitEvents[1]); break;
            case WSA_WAIT_EVENT_0+2: WSAResetEvent (waitEvents[2]); break;
            default: break;
            }
#endif /* !_WIN32 */
            break;

        default:
            if (pgm_err) {
                qDebug() << "pgm error:" << pgm_err->message;
                pgm_error_free (pgm_err);
                pgm_err = NULL;
            }
            if (PGM_IO_STATUS_ERROR == status)
                break;
        }

        QCoreApplication::processEvents();
    } while (!m_is_terminated);

    qDebug() << "Message loop terminated, cleaning up.";

    /* cleanup */
#ifndef _WIN32
    close (m_terminate_pipe[0]);
    close (m_terminate_pipe[1]);
#else
    WSACloseEvent (waitEvents[0]);
    WSACloseEvent (waitEvents[1]);
    WSACloseEvent (waitEvents[2]);
#endif /* !_WIN32 */

    this->thread()->quit();
}
