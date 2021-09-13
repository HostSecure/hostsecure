#pragma once
#include "mqttclientbase.h"
#include <msg/msgedge.h>
#include <msg/msgdevice.h>

class MmiMqttClient : public MqttClientBase
{
   Q_OBJECT

public:
   explicit MmiMqttClient( QObject* a_parent = nullptr );

signals:
   void edgeChanged( const QString& a_edgeId, const MsgEdge& a_sample );
   void edgeRemoved( const QString& a_edgeId );

   void deviceChanged( const QString& a_edgeId, const QString& a_deviceId, const MsgDevice& a_sample );
   void deviceRemoved( const QString& a_edgeId, const QString& a_deviceId );

private:
   Q_DISABLE_COPY_MOVE( MmiMqttClient )

   void brokerConnected() override;
   void incomingEdge( QMqttMessage a_sample );
};
