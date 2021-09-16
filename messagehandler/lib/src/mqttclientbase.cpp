#include "mqttclientbase.h"
#include <QJsonDocument>
#include <QMetaEnum>

namespace
{
   constexpr auto MQTT_ADDRESS = "92.220.65.49";
   constexpr auto MQTT_PORT = 1883;
}

//!
//! \brief MqttClientBase::MqttClientBase
//! Sets up the client ready for connection, and adds automatic reconnect.
//! \param a_parent
//!
MqttClientBase::MqttClientBase( QObject* a_parent )
   : QMqttClient { a_parent }
{
   qDebug() << "MQTT protocol version:" << protocolVersion();
   QObject::connect( this, &QMqttClient::connected, this, &MqttClientBase::brokerConnected );
   QObject::connect( this, &QMqttClient::protocolVersionChanged, this, &MqttClientBase::protocolVersionStringChanged );
   QObject::connect( this, &QMqttClient::stateChanged, this, &MqttClientBase::stateStringChanged );
   QObject::connect( this, &QMqttClient::errorChanged, this, &MqttClientBase::errorStringChanged );

   setHostname( MQTT_ADDRESS );
   setPort( MQTT_PORT );

   m_reconnectTimer.setInterval( 3000 );
   m_reconnectTimer.setSingleShot( true );

   QObject::connect( &m_reconnectTimer, &QTimer::timeout, this, [ this ] {
      if ( state() == QMqttClient::Disconnected )
         connectToHost();
   } );

   QObject::connect( this, &QMqttClient::stateChanged, this, [ this ] {
      if ( state() == QMqttClient::Disconnected )
         m_reconnectTimer.start();

      qDebug() << "StateChanged: " << state();
   } );
}

//!
//! \brief MqttClientBase::publish Publish a message
//! \param a_topic
//! \param a_sample
//! \param a_qos
//! \param a_retain
//! \return
//!
qint32 MqttClientBase::publish( const QMqttTopicName& a_topic,
                                const MsgBase& a_sample,
                                quint8 a_qos,
                                bool a_retain )
{
   QJsonDocument data( a_sample.toJson() );
   return publish( a_topic, data.toJson(), a_qos, a_retain );
}

//!
//! \brief MqttClientBase::publish Publish a message
//! \param a_topic
//! \param a_sample
//! \param a_qos
//! \param a_retain
//! \return
//!
qint32 MqttClientBase::publish( const QMqttTopicName& a_topic,
                                const QByteArray& a_sample,
                                quint8 a_qos,
                                bool a_retain )
{
   qDebug() << "Published a message to" << a_topic;
   return QMqttClient::publish( a_topic, a_sample, a_qos, a_retain );
}

//!
//! \brief MqttClientBase::protocolVersionString
//! \return Enum as string
//!
QString MqttClientBase::protocolVersionString() const
{
   return QMetaEnum::fromType< QMqttClient::ProtocolVersion >().valueToKey( protocolVersion() );
}

//!
//! \brief MqttClientBase::stateString
//! \return Enum as string
//!
QString MqttClientBase::stateString() const
{
   return QMetaEnum::fromType< QMqttClient::ClientState >().valueToKey( state() );
}

//!
//! \brief MqttClientBase::errorString
//! \return Enum as string
//!
QString MqttClientBase::errorString() const
{
   return QMetaEnum::fromType< QMqttClient::ClientError >().valueToKey( error() );
}
