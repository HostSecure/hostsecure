#include "mqttclientbase.h"
#include <QJsonDocument>
#include <QMetaEnum>

namespace
{
//   constexpr auto MQTT_ADDRESS = "localhost";
//   constexpr auto MQTT_PORT = 1883;

   constexpr auto MQTT_ADDRESS = "92.220.65.49";
   constexpr auto MQTT_PORT = 1883;
}

MqttClientBase::MqttClientBase( QObject* a_parent )
   : QMqttClient { a_parent }
   , m_broker { MQTT_ADDRESS, MQTT_PORT, 0, true, 60 }
{
   qDebug() << "MQTT protocol version:" << protocolVersion();
   QObject::connect( this, &QMqttClient::connected, this, &MqttClientBase::brokerConnected );
   QObject::connect( this, &QMqttClient::protocolVersionChanged, this, &MqttClientBase::protocolVersionStringChanged );
   QObject::connect( this, &QMqttClient::stateChanged, this, &MqttClientBase::stateStringChanged );
   QObject::connect( this, &QMqttClient::errorChanged, this, &MqttClientBase::errorStringChanged );

   setHostname( m_broker.address );
   setPort( m_broker.port );
   connectToHost();
}

qint32 MqttClientBase::publish( const QMqttTopicName& a_topic,
                                const MsgBase& a_sample,
                                quint8 a_qos,
                                bool a_retain )
{
   QJsonDocument data( a_sample.toJson() );
   return publish( a_topic, data.toJson(), a_qos, a_retain );
}

qint32 MqttClientBase::publish( const QMqttTopicName& a_topic,
                                const QByteArray& a_sample,
                                quint8 a_qos,
                                bool a_retain )
{
   qDebug() << "Published a message to" << a_topic;
   return QMqttClient::publish( a_topic, a_sample, a_qos, a_retain );
}

QString MqttClientBase::protocolVersionString() const
{
   return QMetaEnum::fromType< QMqttClient::ClientState >().valueToKey( protocolVersion() );
}

QString MqttClientBase::stateString() const
{
   return QMetaEnum::fromType< QMqttClient::ClientState >().valueToKey( state() );
}

QString MqttClientBase::errorString() const
{
   return QMetaEnum::fromType< QMqttClient::ClientError >().valueToKey( error() );
}
