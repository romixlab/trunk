#include "pgm_tx.h"
#include <QDebug>
#include <pgm/pgm.h>
#include <time.h>
#ifdef CONFIG_HAVE_EPOLL
#	include <sys/epoll.h>
#endif
#include <sys/types.h>
#ifndef _WIN32
#	include <inttypes.h>
#	include <unistd.h>
#	include <netdb.h>
#	include <netinet/in.h>
#	include <sched.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#	include <sys/time.h>
#else
#	include <ws2tcpip.h>
#	include <mswsock.h>
#	include <pgm/wininttypes.h>
#	include "getopt.h"
#endif

#include <QCoreApplication>
/* PGM internal time keeper */
extern "C" {
    typedef pgm_time_t (*pgm_time_update_func)(void);
    extern pgm_time_update_func pgm_time_update_now;
}

pgm_tx::pgm_tx(QObject *parent) :
    QObject(parent), m_out_total(0), m_msg_sent(0), m_is_terminated(false)
{
    pipe(m_terminate_pipe);
}

void pgm_tx::setPgmSocket(pgm_sock_t *socket)
{
    m_socket = socket;
}

void pgm_tx::stop()
{
    m_is_terminated = true;
    const char one = '1';
    const size_t writelen = write (m_terminate_pipe[1], &one, sizeof(one));
    if (writelen != sizeof(one))
        qWarning() << "write to m_terminate_pipe[1] failed";
}

void pgm_tx::doWork()
{
    quint64 latency, now, last = 0;
    int g_odata_interval = 0;
#ifdef CONFIG_HAVE_EPOLL
    const long ev_len = 1;
    struct epoll_event events[ev_len];

    int efd_again = epoll_create (IP_MAX_MEMBERSHIPS);
    if (efd_again < 0) {
        g_error ("epoll_create failed errno %i: \"%s\"", errno, strerror(errno));
        g_main_loop_quit (g_loop);
        return NULL;
    }
/* Add write event to epoll domain in order to re-enable as required by return
 * value.  We use one-shot flag to disable ASAP, as we don't want such events
 * until triggered.
 */
    if (pgm_epoll_ctl (tx_sock, efd_again, EPOLL_CTL_ADD, EPOLLOUT | EPOLLONESHOT) < 0) {
        g_error ("pgm_epoll_ctl failed errno %i: \"%s\"", errno, strerror(errno));
        g_main_loop_quit (g_loop);
        return NULL;
    }
    struct epoll_event event;
    memset (&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = g_quit_pipe[0];
    if (epoll_ctl (efd_again, EPOLL_CTL_ADD, g_quit_pipe[0], &event) < 0) {
        g_error ("epoll_ctl failed errno %i: \"%s\"", errno, strerror(errno));
        g_main_loop_quit (g_loop);
        return NULL;
    }
#elif defined(CONFIG_HAVE_POLL)
/* does not include ACKs */
    int n_fds = PGM_BUS_SOCKET_WRITE_COUNT;
    struct pollfd fds[ PGM_BUS_SOCKET_WRITE_COUNT + 1 ];
#elif defined(CONFIG_HAVE_WSAPOLL)
    ULONG n_fds = PGM_BUS_SOCKET_WRITE_COUNT;
    WSAPOLLFD fds[ PGM_BUS_SOCKET_WRITE_COUNT + 1 ];
#elif defined(CONFIG_WSA_WAIT)
/* does not include ACKs */
    SOCKET send_sock;
    DWORD cEvents = PGM_BUS_SOCKET_WRITE_COUNT + 1;
    WSAEVENT waitEvents[ PGM_BUS_SOCKET_WRITE_COUNT + 1 ];
    socklen_t socklen = sizeof (SOCKET);

    waitEvents[0] = WSACreateEvent();
    WSAEventSelect (g_quit_socket[0], waitEvents[0], FD_READ);
    waitEvents[1] = WSACreateEvent();
    g_assert (1 == PGM_BUS_SOCKET_WRITE_COUNT);
    pgm_getsockopt (tx_sock, IPPROTO_PGM, PGM_SEND_SOCK, &send_sock, &socklen);
    WSAEventSelect (send_sock, waitEvents[1], FD_WRITE);
#endif /* !CONFIG_HAVE_EPOLL */

#ifdef CONFIG_HAVE_SCHEDPARAM
/* realtime scheduling */
    pthread_t thread_id = pthread_self ();
    int policy;
    struct sched_param param;

    if (0 == pthread_getschedparam (thread_id, &policy, &param)) {
        policy = SCHED_FIFO;
        param.sched_priority = 50;
        if (0 != pthread_setschedparam (thread_id, policy, &param))
            g_warning ("Cannot set thread scheduling parameters.");
    } else
        g_warning ("Cannot get thread scheduling parameters.");
#endif

    last = now = pgm_time_update_now();
    do {

/* wait on packet rate limit */
        if ((last + g_odata_interval) > now) {
#ifndef _WIN32
            const unsigned int usec = g_odata_interval - (now - last);
            usleep (usec);
#else
#	define usecs_to_msecs(t)	( ((t) + 999) / 1000 )
            const DWORD msec = (DWORD)usecs_to_msecs (g_odata_interval - (now - last));
/* Avoid yielding on Windows XP/2000 */
            if (msec > 0)
                Sleep (msec);
#endif
            now = pgm_time_update_now();
        }
        last += g_odata_interval;

        struct timeval tv;
#if defined(CONFIG_HAVE_EPOLL) || defined(CONFIG_HAVE_POLL)
        int timeout;
#elif defined(CONFIG_HAVE_WSAPOLL)
        DWORD dwTimeout;
#elif defined(CONFIG_WSA_WAIT)
        DWORD dwTimeout, dwEvents;
#else
        int n_fds;
        fd_set readfds, writefds;
#endif
        size_t bytes_written;
        int status;
again:
        QCoreApplication::processEvents();
        sleep(1);
        status = pgm_send (m_socket, "hello i'm send", 15, &bytes_written);
        switch (status) {
/* rate control */
        case PGM_IO_STATUS_RATE_LIMITED:
        {
            socklen_t optlen = sizeof (tv);
            bool status = pgm_getsockopt (m_socket, IPPROTO_PGM, PGM_RATE_REMAIN, &tv, &optlen);
            if (Q_UNLIKELY(!status)) {
                qWarning() << "getting PGM_RATE_REMAIN failed";
                break;
            }
#if defined(CONFIG_HAVE_EPOLL) || defined(CONFIG_HAVE_POLL)
            timeout = (tv.tv_sec * 1000) + ((tv.tv_usec + 500) / 1000);
/* busy wait under 2ms */
#	ifdef CONFIG_BUSYWAIT
            if (timeout < 2)
                goto again;
#	else
            if (0 == timeout)
                goto again;
#	endif
#elif defined(CONFIG_HAVE_WSAPOLL) || defined(CONFIG_WSA_WAIT)
/* round up wait */
#	ifdef CONFIG_BUSYWAIT
            dwTimeout = (DWORD)(tv.tv_sec * 1000) + ((tv.tv_usec + 500) / 1000);
#	else
            dwTimeout = (DWORD)(tv.tv_sec * 1000) + ((tv.tv_usec + 999) / 1000);
#	endif
            if (0 == dwTimeout)
                goto again;
#endif
#if defined(CONFIG_HAVE_EPOLL)
            const int ready = epoll_wait (efd_again, events, G_N_ELEMENTS(events), timeout /* ms */);
#elif defined(CONFIG_HAVE_POLL)
            memset (fds, 0, sizeof(fds));
            fds[0].fd = g_quit_pipe[0];
            fds[0].events = POLLIN;
            n_fds = G_N_ELEMENTS(fds) - 1;
            pgm_poll_info (tx_sock, &fds[1], &n_fds, POLLIN);
            poll (fds, 1 + n_fds, timeout /* ms */);
#elif defined(CONFIG_HAVE_WSAPOLL)
            ZeroMemory (fds, sizeof(WSAPOLLFD) * (n_fds + 1));
            fds[0].fd = g_quit_socket[0];
            fds[0].events = POLLRDNORM;
            n_fds = G_N_ELEMENTS(fds) - 1;
            pgm_poll_info (tx_sock, &fds[1], &n_fds, POLLRDNORM);
            WSAPoll (fds, 1 + n_fds, dwTimeout /* ms */);
#elif defined(CONFIG_WSA_WAIT)
/* only wait for quit or timeout events */
            cEvents = 1;
            dwEvents = WSAWaitForMultipleEvents (cEvents, waitEvents, FALSE, dwTimeout, FALSE);
            switch (dwEvents) {
            case WSA_WAIT_EVENT_0+1: WSAResetEvent (waitEvents[1]); break;
            default: break;
            }
#else
            FD_ZERO(&readfds);
#	ifndef _WIN32
            FD_SET(m_terminate_pipe[0], &readfds);
            n_fds = m_terminate_pipe[0] + 1;		/* highest fd + 1 */
#	else
            FD_SET(g_quit_socket[0], &readfds);
            n_fds = 1;				/* count of fds */
#	endif
            pgm_select_info (m_socket, &readfds, NULL, &n_fds);
            n_fds = select (n_fds, &readfds, NULL, NULL, PGM_IO_STATUS_WOULD_BLOCK == status ? NULL : &tv);
#endif /* !CONFIG_HAVE_EPOLL */
            if (Q_UNLIKELY(m_is_terminated))
                break;
            goto again;
        }
/* congestion control */
        case PGM_IO_STATUS_CONGESTION:
/* kernel feedback */
        case PGM_IO_STATUS_WOULD_BLOCK:
        {
#ifdef CONFIG_HAVE_EPOLL
#	if 1
/* re-enable write event for one-shot */
            if (pgm_epoll_ctl (tx_sock, efd_again, EPOLL_CTL_MOD, EPOLLOUT | EPOLLONESHOT) < 0)
            {
                g_error ("pgm_epoll_ctl failed errno %i: \"%s\"", errno, strerror(errno));
                g_main_loop_quit (g_loop);
                return NULL;
            }
            const int ready = epoll_wait (efd_again, events, G_N_ELEMENTS(events), -1 /* ms */);
            if (G_UNLIKELY(g_quit))
                break;
#	else
            const int ready = epoll_wait (efd_again, events, G_N_ELEMENTS(events), -1 /* ms */);
            if (G_UNLIKELY(g_quit))
                break;
            if (ready > 0 &&
                pgm_epoll_ctl (tx_sock, efd_again, EPOLL_CTL_MOD, EPOLLOUT | EPOLLONESHOT) < 0)
            {
                g_error ("pgm_epoll_ctl failed errno %i: \"%s\"", errno, strerror(errno));
                g_main_loop_quit (g_loop);
                return NULL;
            }
#	endif
#elif defined(CONFIG_HAVE_POLL)
            memset (fds, 0, sizeof(fds));
            fds[0].fd = g_quit_pipe[0];
            fds[0].events = POLLIN;
            n_fds = G_N_ELEMENTS(fds) - 1;
            pgm_poll_info (m_socket, &fds[1], &n_fds, POLLOUT);
            poll (fds, 1 + n_fds, -1 /* ms */);
#elif defined(CONFIG_HAVE_WSAPOLL)
            ZeroMemory (fds, sizeof(WSAPOLLFD) * (n_fds + 1));
            fds[0].fd = g_quit_socket[0];
            fds[0].events = POLLRDNORM;
            n_fds = G_N_ELEMENTS(fds) - 1;
            pgm_poll_info (tx_sock, &fds[1], &n_fds, POLLWRNORM);
            WSAPoll (fds, 1 + n_fds, -1 /* ms */);
#elif defined(CONFIG_WSA_WAIT)
            cEvents = PGM_BUS_SOCKET_WRITE_COUNT + 1;
            dwEvents = WSAWaitForMultipleEvents (cEvents, waitEvents, FALSE, WSA_INFINITE, FALSE);
            switch (dwEvents) {
            case WSA_WAIT_EVENT_0+1: WSAResetEvent (waitEvents[1]); break;
            default: break;
            }
#else
            FD_ZERO(&readfds);
#	ifndef _WIN32
            FD_SET(m_terminate_pipe[0], &readfds);
            n_fds = m_terminate_pipe[0] + 1;		/* highest fd + 1 */
#	else
            FD_SET(g_quit_socket[0], &readfds);
            n_fds = 1;				/* count of fds */
#	endif
            pgm_select_info (m_socket, &readfds, &writefds, &n_fds);
            n_fds = select (n_fds, &readfds, &writefds, NULL, NULL);

#endif /* !CONFIG_HAVE_EPOLL */
            goto again;
        }
/* successful delivery */
        case PGM_IO_STATUS_NORMAL:
    //		qDebug() << "sent payload: %s", ping.DebugString().c_str());
            qDebug() << "sent %u bytes" << (unsigned)bytes_written;
            break;
        default:
            qWarning() << "pgm_send_skbv failed, status:" << status;
            return; // WARNING!
        }
        m_out_total += bytes_written;
        m_msg_sent++;
    } while (Q_LIKELY(!m_is_terminated));
    qDebug() << "tx terminated";

#if defined(CONFIG_HAVE_EPOLL)
    close (efd_again);
#elif defined(CONFIG_WSA_WAIT)
    WSACloseEvent (waitEvents[0]);
    WSACloseEvent (waitEvents[1]);
#endif
}
