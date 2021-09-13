#include <msg/msgedge.h>

namespace
{
   constexpr auto JSON_MAC_ADDRESS = "MACAddress";
   constexpr auto JSON_IS_ONLINE   = "IsOnline";
}

QJsonObject MsgEdge::toJson() const
{
   QJsonObject msg;
   msg[ JSON_MAC_ADDRESS ] = macaddress;
   msg[ JSON_IS_ONLINE ]   = isOnline;
   return msg;
}

bool MsgEdge::fromJson( const QJsonObject& a_msg )
{
   if ( a_msg.isEmpty() )
   {
      return false;
   }

   macaddress = a_msg[ JSON_MAC_ADDRESS ].toString();
   isOnline = a_msg[ JSON_IS_ONLINE ].toBool();
   return true;
}
