#ifndef DISCOVERYSERVICEBASE_H
#define DISCOVERYSERVICEBASE_H
#include <QObject>

class DiscoveryServiceBase : public QObject {
    Q_OBJECT
public:
    DiscoveryServiceBase(QObject *parent) : QObject(parent) {}
    virtual ~DiscoveryServiceBase() {}

    /**
     * @brief setNodeInfo set info that will be sended to other nodes
     * @param nodeName this node name (should be unique)
     * @param nodeGroups this node groups
     * @param nodeAddress other nodes can connect to this address if they want
     */
    virtual void setNodeInfo(const QString &nodeName,
                             const QStringList &nodeGroups,
                             const QString &nodeAddress) = 0;
    /**
     * @brief start start discovery service
     * @return true if service started successfully
     */
    virtual bool start() = 0;
    /**
     * @brief stop stop discovery service
     * @return true if service stopped successfully
     */
    virtual bool stop() = 0;

signals:
    /**
     * @brief nodeFound some node are found
     * @param nodeName remote node name
     * @param nodeGroups remote node groups
     * @param nodeAddress remote node address
     */
    void nodeFound(const QString &nodeName,
                   const QStringList &nodeGroups,
                   const QString &nodeAddress);
    void nodeLeft(const QString &nodeName);
};

#endif // DISCOVERYSERVICEBASE_H
