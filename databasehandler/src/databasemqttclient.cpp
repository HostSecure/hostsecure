#include "databasemqttclient.h"
#include <QJsonDocument>

DatabaseMqttClient::DatabaseMqttClient( QObject* a_parent )
   : MqttClientBase { a_parent }
{
}

void DatabaseMqttClient::brokerConnected()
{
   qInfo() << "Broker connected!";

   const QMqttTopicFilter topicFilter( "edges/#" ); //! Subscribe with wildcard
   auto edgeSub = subscribe( topicFilter );
   if ( edgeSub != nullptr )
   {
      QObject::connect( edgeSub, &QMqttSubscription::messageReceived,
                        this, &DatabaseMqttClient::incomingEdge );
   }
}

void DatabaseMqttClient::incomingEdge( QMqttMessage a_sample )
{
   QJsonDocument document = QJsonDocument::fromJson( a_sample.payload() );
   const auto levelCount = a_sample.topic().levelCount();
   const auto levels = a_sample.topic().levels();

   qInfo() << "Message rc: " << a_sample.topic().name() << "\n" << a_sample.payload();
   qInfo() << "Size: " << levelCount;
   qInfo() << "Levels: " << levels;

   if ( levelCount >= 2 )
   {
      const auto edgeName = levels.at( 1 );

      if ( levelCount == 2 )
      {
         MsgEdge sample;
         if ( sample.fromJson( document.object() ) )
            emit edgeChanged( edgeName, sample );
         else
            emit edgeRemoved( edgeName );
      }
      else if ( levelCount == 3 )
      {
         const auto deviceId = levels.at( 2 );

         MsgDevice sample;
         if ( sample.fromJson( document.object() ) )
            emit deviceChanged( edgeName, deviceId, sample );
         else
            emit deviceRemoved( edgeName, deviceId, sample.deviceSerial );
      }
   }
}
