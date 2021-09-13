#include <msg/msgedge.h>

namespace
{
   constexpr auto JSON_ID = "id";
   constexpr auto JSON_NAME = "name";
   constexpr auto JSON_CONNECTED = "connected";
   constexpr auto JSON_DEVICES = "devices";
}

MsgEdge::MsgEdge( int a_id,
                  const QString a_name,
                  bool a_connected,
                  QStringList a_devices )
   : MsgBase { "edges/new/edge/#" }
   , id { a_id }
   , name { a_name }
   , connected { a_connected }
   , devices { a_devices }
{ }

QJsonObject MsgEdge::toJson() const
{
   QJsonObject msg;
   msg[ JSON_ID ] = id;
   msg[ JSON_NAME ] = name;
   msg[ JSON_CONNECTED ] = connected;
   msg[ JSON_DEVICES ] = devices.join( ":" );
   return msg;
}

bool MsgEdge::fromJson( const QJsonObject& a_msg )
{
   if ( a_msg.isEmpty() )
   {
      return false;
   }

   id = a_msg[ JSON_ID ].toInt();
   name = a_msg[ JSON_NAME ].toString();
   connected = a_msg[ JSON_CONNECTED ].toBool();
   devices = a_msg[ JSON_DEVICES ].toString().split( ":" );
   return true;



   //   if ( a_msg.contains( JSON_ID ) &&
   //        a_msg.contains( JSON_NAME ) )
   //   {
   //      id = a_msg[ JSON_ID ].toInt();
   //      name = a_msg[ JSON_NAME ].toString();
   //      connected = a_msg[ JSON_CONNECTED ].toBool();
   //      devices = a_msg[ JSON_DEVICES ].toString().split( ":" );
   //      return true;
   //   }
   //   qWarning() << __PRETTY_FUNCTION__ << "Invalid msg.." << a_msg;
   //   return false;
}
