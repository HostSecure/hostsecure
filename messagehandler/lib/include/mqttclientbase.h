#pragma once
#include <QMqttClient>
#include <msg/msgbase.h>

class MqttClientBase : public QMqttClient
{
   Q_OBJECT
   Q_PROPERTY( QString protocolVersionString READ protocolVersionString NOTIFY protocolVersionStringChanged )
   Q_PROPERTY( QString stateString READ stateString NOTIFY stateStringChanged )
   Q_PROPERTY( QString errorString READ errorString NOTIFY errorStringChanged )

public:
   qint32 publish( const QMqttTopicName& a_topic,
                   const MsgBase& a_sample,
                   quint8 a_qos = 0,
                   bool a_retain = false );
   qint32 publish( const QMqttTopicName& a_topic,
                   const QByteArray& a_sample = QByteArray(),
                   quint8 a_qos = 0,
                   bool a_retain = false );

   QString protocolVersionString() const;
   QString stateString() const;
   QString errorString() const;

signals:
   void protocolVersionStringChanged();
   void stateStringChanged();
   void errorStringChanged();

protected:
   explicit MqttClientBase( QObject* a_parent = nullptr );
   virtual void brokerConnected() = 0;

private:
   Q_DISABLE_COPY_MOVE( MqttClientBase )

   struct Broker
   {
      QString address;    //! Server hostname
      quint16 port;       //! Server port
      quint8 qos;         //! Quality of Service
      bool retain;        //! Retain message
      quint8 heartbeat;   //! Ping response (seconds)
   };
   const Broker m_broker;
};

