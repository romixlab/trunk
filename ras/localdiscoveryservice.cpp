#include "localdiscoveryservice.h"
#include "automation_global.h"
#include <qmsgpack/msgpack.h>
#include <QUdpSocket>
#include <QTimer>

LocalDiscoveryService::LocalDiscoveryService(QObject *parent) :
    DiscoveryServiceBase(parent),
    m_udp(0),
    m_multicastGroup(QHostAddress(DEFAULT_MCAST_GROUP)),
    m_multicastPort(DEFAULT_MCAST_PORT)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(DEFAULT_LOCAL_DISCOVERY_INTERVAL);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &LocalDiscoveryService::sendNotify);

    m_udp = new QUdpSocket(this);
    connect(m_udp, SIGNAL(readyRead()), this, SLOT(receiveDatagrams()));
    m_udp_tx = new QUdpSocket(this);
}

void LocalDiscoveryService::setNodeInfo(const QString &nodeName,
                                        const QStringList &nodeGroups,
                                        const QString &nodeAddress)
{
    QVariantMap info;
    m_ourName = nodeName;
    info["name"] = m_ourName;
    info["groups"] = nodeGroups;
    info["address"] = nodeAddress;
    m_nodeInfo = MsgPack::pack(info);
}

bool LocalDiscoveryService::start()
{
    if (m_udp->state() == QUdpSocket::BoundState) {
        m_error = "alredy started";
        return false;
    }
    if (!m_udp->bind(QHostAddress::AnyIPv4, m_multicastPort,
                     QUdpSocket::ShareAddress)) {
        m_error = "bind failed:" + m_udp->errorString();
        return false;
    }
    //m_udp->setSocketOption(QAbstractSocket::MulticastLoopbackOption, true);
    if (!m_udp->joinMulticastGroup(m_multicastGroup)) {
        m_error = "can't join multicast group:" + m_udp->errorString();
        return false;
    }
    m_timer->start();
    return true;
}

bool LocalDiscoveryService::stop()
{
    if (m_udp->state() != QUdpSocket::BoundState) {
        m_error = "already stopped";
        return false;
    }
    m_udp->abort();
    return true;
}

void LocalDiscoveryService::setMulticastGroup(const QHostAddress &group)
{
    m_multicastGroup = group;
}

void LocalDiscoveryService::setMulticastPort(quint16 port)
{
    m_multicastPort = port;
}

void LocalDiscoveryService::setNotifyInterval(int interval)
{
    m_timer->setInterval(interval);
}

QString LocalDiscoveryService::errorString() const
{
    return m_error;
}

void LocalDiscoveryService::sendNotify()
{
    m_udp_tx->writeDatagram(m_nodeInfo, m_multicastGroup, m_multicastPort);
}

void LocalDiscoveryService::receiveDatagrams()
{
    int size;
    QByteArray datagram;
    while (m_udp->hasPendingDatagrams()) {
        size = m_udp->pendingDatagramSize();
        datagram.resize(size);
        quint16 port;
        m_udp->readDatagram(datagram.data(), size, 0, &port);
        QVariant notify = MsgPack::unpack(datagram);
        if (notify.isValid() &&
                (QMetaType::Type)notify.type() == QMetaType::QVariantMap) {
            QVariantMap info = notify.toMap();
            if (info["name"] != m_ourName)
                emit nodeFound(info["name"].toString(),
                               info["groups"].toStringList(),
                               info["address"].toString());
        }
    }
}
