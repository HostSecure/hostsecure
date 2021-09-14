#include "databasemanager.h"

#include "databasehandler.h"
#include "databasemqttclient.h"

#include <QTime>

DatabaseManager::DatabaseManager(const QString &databaseName, QObject *parent)
    : QObject(parent)
    , m_DatabaseHandler(new DatabaseHandler(databaseName))
    , m_MqttCient(new DatabaseMqttClient(parent))
{
    connect( m_MqttCient.get(), &DatabaseMqttClient::edgeChanged, this, &DatabaseManager::edgeChanged );
    connect( m_MqttCient.get(), &DatabaseMqttClient::edgeRemoved, this, &DatabaseManager::edgeRemoved );
    connect( m_MqttCient.get(), &DatabaseMqttClient::deviceChanged, this, &DatabaseManager::deviceChanged );
    connect( m_MqttCient.get(), &DatabaseMqttClient::deviceRemoved, this, &DatabaseManager::deviceRemoved );
}

void DatabaseManager::edgeChanged(const QString &edgeId, const MsgEdge &sample)
{
    m_DatabaseHandler->registerEdgeNode(edgeId, sample.isOnline, QDateTime::currentDateTimeUtc().toString());
}

void DatabaseManager::edgeRemoved(const QString &edgeId)
{
    m_DatabaseHandler->setEdgeNodeOnlineStatus(edgeId, false);
    m_DatabaseHandler->unregisterConnectedDevicesOnEdgeNode(edgeId);
}

void DatabaseManager::deviceChanged(const QString &edgeId, const QString &deviceId, const MsgDevice &sample)
{
    QStringList vendorProductIds = deviceId.split(":");
    if(vendorProductIds.size() != 2)
    {
        qCritical() << "Received device changed with unrecognizable deviceid: " << deviceId;
    }
    else
    {
        m_DatabaseHandler->registerDevice(vendorProductIds[1], vendorProductIds[0], sample.deviceSerial);
        m_DatabaseHandler->registerConnectedDevice(edgeId, vendorProductIds[1], vendorProductIds[0], sample.deviceSerial, sample.lastHeartBeat);
        m_DatabaseHandler->logEvent(edgeId, vendorProductIds[1], vendorProductIds[0], sample.deviceSerial, QDateTime::currentDateTimeUtc().toString(), "Device connected");
    }
}

void DatabaseManager::deviceRemoved(const QString &edgeId, const QString &deviceId, const QString& deviceSerial)
{
    QStringList vendorProductIds = deviceId.split(":");
    if(vendorProductIds.size() != 2)
    {
        qCritical() << "Received device removed with unrecognizable deviceid: " << deviceId;
    }
    m_DatabaseHandler->unregisterConnectedDevice(edgeId, vendorProductIds[1], vendorProductIds[0], deviceSerial);
    m_DatabaseHandler->logEvent(edgeId, vendorProductIds[1], vendorProductIds[0], deviceSerial, QDateTime::currentDateTimeUtc().toString(), "Device disconnected");
}
