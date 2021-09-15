#include "databasemanager.h"

#include "databasehandler.h"
#include "databasemqttclient.h"

#include <QTime>

DatabaseManager::DatabaseManager(const QString &a_databaseName, QObject *a_parent)
    : QObject(a_parent)
    , m_DatabaseHandler(new DatabaseHandler(a_databaseName))
    , m_MqttCient(new DatabaseMqttClient(a_parent))
{
    connect( m_MqttCient.get(), &DatabaseMqttClient::edgeChanged, this, &DatabaseManager::edgeChanged );
    connect( m_MqttCient.get(), &DatabaseMqttClient::edgeRemoved, this, &DatabaseManager::edgeRemoved );
    connect( m_MqttCient.get(), &DatabaseMqttClient::deviceChanged, this, &DatabaseManager::deviceChanged );
    connect( m_MqttCient.get(), &DatabaseMqttClient::deviceRemoved, this, &DatabaseManager::deviceRemoved );
}

void DatabaseManager::edgeChanged(const QString &a_edgeId, const MsgEdge &a_sample)
{
    try
    {
        m_DatabaseHandler->registerOrUpdateEdgeNode(a_edgeId, a_sample.isOnline, QDateTime::currentDateTimeUtc().toString());
    }
    catch (std::exception& e)
    {
        // Ignore. Handled in database
    }
}

void DatabaseManager::edgeRemoved(const QString &a_edgeId)
{
    try
    {
        m_DatabaseHandler->setEdgeNodeOnlineStatus(a_edgeId, false);
        m_DatabaseHandler->unregisterConnectedDevicesOnEdgeNode(a_edgeId);
    }
    catch (std::exception& e)
    {
        // Ignore. Handled in database
    }
}

void DatabaseManager::deviceChanged(const QString &a_edgeId, const QString &a_deviceId, const MsgDevice &a_sample)
{
    QStringList vendorProductIds = a_deviceId.split(":");
    if(vendorProductIds.size() != 2)
    {
        qCritical() << "Received device changed with unrecognizable deviceid: " << a_deviceId;
    }
    else
    {
        try
        {
            m_DatabaseHandler->registerDevice(vendorProductIds[1], vendorProductIds[0], a_sample.deviceSerial);
            m_DatabaseHandler->registerConnectedDevice(a_edgeId, vendorProductIds[1], vendorProductIds[0], a_sample.deviceSerial, a_sample.lastHeartBeat);
            m_DatabaseHandler->logEvent(a_edgeId, vendorProductIds[1], vendorProductIds[0], a_sample.deviceSerial, QDateTime::currentDateTimeUtc().toString(), "Device connected");
        }
        catch (std::exception& e)
        {
            // Ignore. Handled in database
        }
    }
}

void DatabaseManager::deviceRemoved(const QString &a_edgeId, const QString &a_deviceId, const QString& a_deviceSerial)
{
    QStringList vendorProductIds = a_deviceId.split(":");
    if(vendorProductIds.size() != 2)
    {
        qCritical() << "Received device removed with unrecognizable deviceid: " << a_deviceId;
    }
    else
    {
        try
        {
            m_DatabaseHandler->unregisterConnectedDevice(a_edgeId, vendorProductIds[1], vendorProductIds[0], a_deviceSerial);
            m_DatabaseHandler->logEvent(a_edgeId, vendorProductIds[1], vendorProductIds[0], a_deviceSerial, QDateTime::currentDateTimeUtc().toString(), "Device disconnected");
        }
        catch (std::exception& e)
        {
            // Ignore. Handled in database
        }
    }
}
