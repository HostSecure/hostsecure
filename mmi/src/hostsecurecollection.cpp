#include "hostsecurecollection.h"


HostSecureCollection::HostSecureCollection( QObject* a_parent )
   : QObject { a_parent }
   , m_mmiMqttClient { new MmiMqttClient( this ) }
   , m_edgeModel { new EdgeModel( this ) }
{
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::edgeChanged, m_edgeModel, &EdgeModel::edgeChanged );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::edgeRemoved, m_edgeModel, &EdgeModel::edgeRemoved );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::deviceChanged, m_edgeModel, &EdgeModel::deviceChanged );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::deviceRemoved, m_edgeModel, &EdgeModel::deviceRemoved );
}

void HostSecureCollection::sendEdgeMessage()
{
   MsgEdge sample;
   sample.macaddress = "21:12:324:324";
   sample.isOnline = true;

   m_mmiMqttClient->publish( QMqttTopicName( "edges/21:12:324:324" ), sample );
}

#include <QDateTime>
void HostSecureCollection::sendDeviceMessage()
{
   static int deviceId = 123124;

   MsgDevice sample;
   sample.lastHeartBeat = QDateTime::currentDateTimeUtc().toString();
   sample.deviceId = QString::number( deviceId );
   sample.deviceSerial = "serial";
   sample.target = 44;
   sample.event = 321;
   sample.interface = "interface";

   m_mmiMqttClient->publish( QMqttTopicName( QString( "edges/21:12:324:324/%1" ).arg( sample.deviceId ) ), sample );
   deviceId++;
}


