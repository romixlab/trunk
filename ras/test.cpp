#include "test.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QCoreApplication>

#include <pgm/pgm.h>
#	include <unistd.h>

test::test(QObject *parent) :
    QObject(parent)
{
    pgm_error_t* pgm_err = NULL;
    if (!pgm_init (&pgm_err)) {
        qWarning() << "Unable to start PGM engine:" << pgm_err->message;
        pgm_error_free (pgm_err);
        return;
    }

    if(!startPGM()) {
        qWarning() << "startPGM() failed";
        return;
    }

connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()),
        this, SLOT(onAboutToQuit()));


    net = new PGMNetwork;
    net->setPgmSocket(m_socket);

    thread = new QThread;
    net->moveToThread(thread);

    connect(net, SIGNAL(rx(QByteArray)), this, SLOT(onRx(QByteArray)));
    connect(thread, SIGNAL(started()), net, SLOT(doWork()));
    //connect(net, SIGNAL(finished()), thread, SLOT(quit()));


    thread->start();

}

void test::onRx(const QByteArray &data)
{
    qDebug() << "recieved: " << data.size();
}

void test::onAboutToQuit()
{
    net->stop();
    if (!thread->wait(5000)) {
        qWarning() << "Thread deadlock detected, terminating it";
        thread->terminate();
        thread->wait();
    }

    if (m_socket) {
        qDebug() << "Destroying PGM socket.";
        pgm_close(m_socket, TRUE);
        m_socket = NULL;
    }

    qDebug() << "PGM engine shutdown.";
    pgm_shutdown();
    qDebug() << "finished.";
}

bool test::startPGM()
{
    struct pgm_addrinfo_t* res = NULL;
    pgm_error_t* pgm_err = NULL;
    sa_family_t sa_family = AF_UNSPEC;

    // parse network parameter into PGM socket address structure
    if (!pgm_getaddrinfo("eth0;239.192.0.1", NULL, &res, &pgm_err)) {
        qWarning() << "Parsing network parameter: %s\n" << pgm_err->message;
        pgm_error_free (pgm_err);
        return false;
    }

    sa_family = res->ai_send_addrs[0].gsr_group.ss_family;
    int udp_encap_port = 7777; // ****************
    if (udp_encap_port) {
        qDebug() << "Create PGM/UDP socket.";
        if (!pgm_socket (&m_socket, sa_family, SOCK_SEQPACKET, IPPROTO_UDP, &pgm_err)) {
            qWarning() << "Creating PGM/UDP socket: %s\n" << pgm_err->message;
            pgm_error_free (pgm_err);
            return false;
        }
        if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_UDP_ENCAP_UCAST_PORT, &udp_encap_port, sizeof(udp_encap_port))) {
            qWarning() << "settings PGM_UDP_ENCAP_UCAST_PORT =" << udp_encap_port << "failed";
            return false;
        }
        if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_UDP_ENCAP_MCAST_PORT, &udp_encap_port, sizeof(udp_encap_port))) {
            qWarning() << "settings PGM_UDP_ENCAP_MCAST_PORT =" << udp_encap_port << "failed";
            return false;
        }
    } else {
        qDebug() << "Create PGM/IP socket.";
        if (!pgm_socket (&m_socket, sa_family, SOCK_SEQPACKET, IPPROTO_PGM, &pgm_err)) {
            qWarning() << "Creating PGM/IP socket: %s\n" << pgm_err->message;
            pgm_error_free (pgm_err);
            return false;
        }
    }

    // Use RFC 2113 tagging for PGM Router Assist
    const int no_router_assist = 0;
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_IP_ROUTER_ALERT, &no_router_assist, sizeof(no_router_assist))) {
        qWarning() << "settings PGM_IP_ROUTER_ALERT =" << no_router_assist << "failed";
        return false;
    }

#ifndef CONFIG_HAVE_SCHEDPARAM
    pgm_drop_superuser();
#endif
    int g_max_tpdu = 1500; // **********
#if defined(__FreeBSD__)
/* FreeBSD defaults to low maximum socket size */
    const int txbufsize = 128 * 1024, rxbufsize = 128 * 1024;
#else
    const int txbufsize = 1024 * 1024, rxbufsize = 1024 * 1024;
#endif
    const int max_tpdu = g_max_tpdu;

    if (!pgm_setsockopt (m_socket, SOL_SOCKET, SO_RCVBUF, &rxbufsize, sizeof(rxbufsize))) {
        qWarning() << "setting SO_RCVBUF =" << rxbufsize << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, SOL_SOCKET, SO_SNDBUF, &txbufsize, sizeof(txbufsize))) {
        qWarning() << "setting SO_SNDBUF = %d" << txbufsize << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_MTU, &max_tpdu, sizeof(max_tpdu))) {
        qWarning() << "setting PGM_MTU = %d" << max_tpdu << "failed";
        return false;
    }

    // ****************
    static int		g_odata_rate = 0;
    static int		g_odata_interval = 0;
    static int		g_max_rte = 16*1000*1000;
    static int		g_odata_rte = 0;	/* 0 = disabled */
    static int		g_rdata_rte = 0;	/* 0 = disabled */
    static int		g_sqns = 200;

    // send size
    const int send_only = 0,
            txw_sqns	  = g_sqns * 4,
            txw_max_rte	  = g_max_rte,
            odata_max_rte	  = g_odata_rte,
            rdata_max_rte	  = g_rdata_rte,
            ambient_spm	  = pgm_secs (30),
            heartbeat_spm[] = { pgm_msecs (100),
                                pgm_msecs (100),
                                pgm_msecs (100),
                                pgm_msecs (100),
                                pgm_msecs (1300),
                                pgm_secs  (7),
                                pgm_secs  (16),
                                pgm_secs  (25),
                                pgm_secs  (30) };

    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_SEND_ONLY, &send_only, sizeof(send_only))) {
        qWarning() << "setting PGM_SEND_ONLY =" << send_only << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_TXW_SQNS, &txw_sqns, sizeof(txw_sqns))) {
        qWarning() << "setting PGM_TXW_SQNS =" << txw_sqns << "failed";
        return false;
    }
    if (txw_max_rte > 0 &&
            !pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_TXW_MAX_RTE, &txw_max_rte, sizeof(txw_max_rte))) {
        qWarning() << "setting PGM_TXW_MAX_RTE =" << txw_max_rte << "failed";
        return false;
    }
    if (odata_max_rte > 0 &&
            !pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_ODATA_MAX_RTE, &odata_max_rte, sizeof(odata_max_rte))) {
        qWarning() << "setting PGM_ODATA_MAX_RTE =" << odata_max_rte << "failed";
        return false;
    }
    if (rdata_max_rte > 0 &&
            !pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_RDATA_MAX_RTE, &rdata_max_rte, sizeof(rdata_max_rte))) {
        qWarning() << "setting PGM_RDATA_MAX_RTE =" << rdata_max_rte << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_AMBIENT_SPM, &ambient_spm, sizeof(ambient_spm))) {
        qWarning() << "setting PGM_AMBIENT_SPM =" << ambient_spm << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_HEARTBEAT_SPM, &heartbeat_spm, sizeof(heartbeat_spm))) {
        qWarning() << "setting HEARTBEAT_SPM failed";
        return false;
    }

    // reciever side
    const int recv_only = 0,
            not_passive = 0,
            rxw_sqns = g_sqns,
            peer_expiry = pgm_secs (300),
            spmr_expiry = pgm_msecs (250),
            nak_bo_ivl = pgm_msecs (50),
            nak_rpt_ivl = pgm_secs (2),
            nak_rdata_ivl = pgm_secs (2),
            nak_data_retries = 50,
            nak_ncf_retries = 50;

    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_RECV_ONLY, &recv_only, sizeof(recv_only))) {
        qWarning() << "setting PGM_RECV_ONLY =" << recv_only<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_PASSIVE, &not_passive, sizeof(not_passive))) {
        qWarning() << "setting PGM_PASSIVE =" << not_passive<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_RXW_SQNS, &rxw_sqns, sizeof(rxw_sqns))) {
        qWarning() << "setting PGM_RXW_SQNS =" << rxw_sqns<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_PEER_EXPIRY, &peer_expiry, sizeof(peer_expiry))) {
        qWarning() << "setting PGM_PEER_EXPIRY =" << peer_expiry<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_SPMR_EXPIRY, &spmr_expiry, sizeof(spmr_expiry))) {
        qWarning() << "setting PGM_SPMR_EXPIRY =" << spmr_expiry<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NAK_BO_IVL, &nak_bo_ivl, sizeof(nak_bo_ivl))) {
        qWarning() << "setting PGM_NAK_BO_IVL =" << nak_bo_ivl<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NAK_RPT_IVL, &nak_rpt_ivl, sizeof(nak_rpt_ivl))) {
        qWarning() << "setting PGM_NAK_RPT_IVL =" << nak_rpt_ivl<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NAK_RDATA_IVL, &nak_rdata_ivl, sizeof(nak_rdata_ivl))) {
        qWarning() << "setting PGM_NAK_RDATA_IVL =" << nak_rdata_ivl<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NAK_DATA_RETRIES, &nak_data_retries, sizeof(nak_data_retries))) {
        qWarning() << "setting PGM_NAK_DATA_RETRIES =" << nak_data_retries<< "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NAK_NCF_RETRIES, &nak_ncf_retries, sizeof(nak_ncf_retries))) {
        qWarning() << "setting PGM_NAK_NCF_RETRIES =" << nak_ncf_retries<< "failed";
        return false;
    }

    // create global session identifier
    struct pgm_sockaddr_t addr;
    memset (&addr, 0, sizeof(addr));
    int port = 0; // **************
    addr.sa_port = port ? port : DEFAULT_DATA_DESTINATION_PORT;
    addr.sa_addr.sport = DEFAULT_DATA_SOURCE_PORT;
    if (!pgm_gsi_create_from_hostname (&addr.sa_addr.gsi, &pgm_err)) {
        qWarning() << "Creating GSI failed:" << pgm_err->message;
        pgm_error_free(pgm_err);
        return false;
    }

    // assign socket to specified address
    struct pgm_interface_req_t if_req;
    memset (&if_req, 0, sizeof(if_req));
    if_req.ir_interface = res->ai_recv_addrs[0].gsr_interface;
    if_req.ir_scope_id  = 0;
    if (AF_INET6 == sa_family) {
        struct sockaddr_in6 sa6;
        memcpy (&sa6, &res->ai_recv_addrs[0].gsr_group, sizeof(sa6));
        if_req.ir_scope_id = sa6.sin6_scope_id;
    }
    if (!pgm_bind3 (m_socket,
                    &addr, sizeof(addr),
                    &if_req, sizeof(if_req),	/* tx interface */
                    &if_req, sizeof(if_req),	/* rx interface */
                    &pgm_err))
    {
        qWarning() << "Binding PGM socket failed:" << pgm_err->message;
        pgm_error_free(pgm_err);
        return false;
    }

    // join IP multicast groups
    for (unsigned i = 0; i < res->ai_recv_addrs_len; i++) {
        if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_JOIN_GROUP, &res->ai_recv_addrs[i], sizeof(struct group_req))) {
            qWarning() << "setting PGM_JOIN_GROUP failed";
            return false;
        }
    }

    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_SEND_GROUP, &res->ai_send_addrs[0], sizeof(struct group_req))) {
        qWarning() << "setting PGM_SEND_GROUP failed";
        return false;
    }
    pgm_freeaddrinfo (res);

    // set IP parameters
    const int nonblocking	   = 1,
            multicast_direct = 0,
            multicast_hops   = 16,
            dscp		   = 0x2e << 2;	/* Expedited Forwarding PHB for network elements, no ECN. */

    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_MULTICAST_LOOP, &multicast_direct, sizeof(multicast_direct))) {
        qWarning() << "setting PGM_MULTICAST_LOOP" << multicast_direct << "failed";
        return false;
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_MULTICAST_HOPS, &multicast_hops, sizeof(multicast_hops))) {
        qWarning() << "setting PGM_MULTICAST_HOPS" << multicast_hops << "failed";
        return false;
    }
    if (AF_INET6 != sa_family) {
        if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_TOS, &dscp, sizeof(dscp))) {
            qWarning() << "setting PGM_TOS" << dscp << "failed";
            return false;
        }
    }
    if (!pgm_setsockopt (m_socket, IPPROTO_PGM, PGM_NOBLOCK, &nonblocking, sizeof(nonblocking))) {
        qWarning() << "setting PGM_NOBLOCK" << nonblocking << "failed";
        return false;
    }

    qDebug() << "startup complete";
    return true;
}
