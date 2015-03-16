#ifndef LOCALDISCOVERYSERVICE_H
#define LOCALDISCOVERYSERVICE_H
#include "discoveryservicebase.h"
#include "automation_export.h"
#include <QHostAddress>

class QUdpSocket;
class QTimer;

class AUTOMATION_EXPORT LocalDiscoveryService : public DiscoveryServiceBase
{
    Q_OBJECT
public:
    explicit LocalDiscoveryService(QObject *parent = 0);

    virtual void setNodeInfo(const QString &nodeName,
                     const QStringList &nodeGroups,
                     const QString &nodeAddress) override;
    virtual bool start() override;
    virtual bool stop() override;

    void setMulticastGroup(const QHostAddress &group);
    void setMulticastPort(quint16 port);
    void setNotifyInterval(int interval);
    QString errorString() const;

private slots:
    void sendNotify();
    void receiveDatagrams();

private:
    QUdpSocket *m_udp, *m_udp_tx;
    QHostAddress m_multicastGroup;
    quint16 m_multicastPort;
    QTimer *m_timer;
    QByteArray m_nodeInfo; // this node info, packed
    QString m_ourName; // this node name
    QString m_error;
};

#endif // LOCALDISCOVERYSERVICE_H
