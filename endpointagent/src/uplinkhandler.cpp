#include "uplinkhandler.h"
#include <msg/msgedge.h>
#include <msg/msgdevice.h>
#include <QDebug>
#include <iostream>

namespace
{
   QString fetchMac()
   {
      QString mac = "";
      for( const QNetworkInterface& interface : QNetworkInterface::allInterfaces() )
      {
         if( !( interface.flags() & QNetworkInterface::IsLoopBack ) )
         {
            mac = interface.hardwareAddress();
         }
      }
      return mac;
   }
}


UplinkHandler::UplinkHandler(QObject *a_parent)
   : MqttClientBase { a_parent }
   , m_hardwareAddress { fetchMac() }
   , m_hardwareAddressTopic { QString( "edges/%1" ).arg( m_hardwareAddress ) }
{
   // Creat Last Will Testament
   setWillRetain( true );
   setWillTopic( m_hardwareAddressTopic );
   setWillMessage( QByteArray() );
   setWillQoS( 0 );

   connectToHost();
}

UplinkHandler::~UplinkHandler()
{
   // Proper disconnect
   publish( QMqttTopicName( m_hardwareAddressTopic ), QByteArray(), 0, true );
}

void UplinkHandler::brokerConnected()
{
   qInfo() << "Broker connected!";

   MsgEdge edge;
   edge.macaddress = m_hardwareAddress;
   edge.isOnline = true;

   publish(QMqttTopicName(QString("edges/%1").arg(m_hardwareAddress)), edge);
}

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void UplinkHandler::devicePresenceUpload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface, const uint a_event)
{
   qDebug() << "Updating devicePresenceUpload";
   if(a_event == 3)
   {
      publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, a_device_id )), QByteArray());
   }
   else
   {
      MsgDevice msg;
      msg.deviceId = a_device_id;
      msg.deviceSerial = a_device_serial;
      msg.target = a_target;
      msg.interface = a_interface;
      msg.event = a_event;
      msg.lastHeartBeat = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss.zzz");
      publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, a_device_id ) ), msg);
   }
}

void UplinkHandler::rulesetDownload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface)
{
   qDebug() << "New rule from cloud";
   emit appendServiceRule(a_device_id, a_device_serial,a_target,a_interface);
}

void UplinkHandler::devicePolicyUpload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface)
{
   qDebug() << "Updating devicePolicyUpload";
   MsgDevice msg;
   msg.deviceId = a_device_id;
   msg.deviceSerial = a_device_serial;
   msg.target = a_target;
   msg.interface = a_interface;
   msg.event = 0;
   msg.lastHeartBeat = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss.zzz");
   publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, a_device_id ) ), msg);
}



