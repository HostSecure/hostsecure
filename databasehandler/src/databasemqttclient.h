#ifndef DATABASEMQTTCLIENT_H
#define DATABASEMQTTCLIENT_H

#include <mqttclientbase.h>
#include <msg/msgedge.h>
#include <msg/msgdevice.h>

class DatabaseMqttClient : public MqttClientBase
{
    Q_OBJECT

public:
   explicit DatabaseMqttClient( QObject* a_parent = nullptr );

signals:
   void edgeChanged( const QString& a_edgeId, const MsgEdge& a_sample );
   void edgeRemoved( const QString& a_edgeId );

   void deviceChanged( const QString& a_edgeId, const QString& a_deviceId, const MsgDevice& a_sample );
   void deviceRemoved( const QString& a_edgeId, const QString& a_deviceId, const QString& a_deviceSerial );

private:
   Q_DISABLE_COPY_MOVE( DatabaseMqttClient )

   void brokerConnected() override;
   void incomingEdge( QMqttMessage a_sample );
};

#endif // DATABASEMQTTCLIENT_H