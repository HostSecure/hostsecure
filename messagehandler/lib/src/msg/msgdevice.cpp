#include <msg/msgdevice.h>

namespace
{
   constexpr auto JSON_LAST_HEARTBEAT = "LastHeartbeat";
   constexpr auto JSON_DEVICE_ID      = "device_id";
   constexpr auto JSON_DEVICE_SERIAL  = "device_serial";
   constexpr auto JSON_TARGET         = "target";
   constexpr auto JSON_EVENT          = "event";
   constexpr auto JSON_INTERFACE      = "interface";
}

QJsonObject MsgDevice::toJson() const
{
   QJsonObject msg;
   msg[ JSON_LAST_HEARTBEAT ] = lastHeartBeat;
   msg[ JSON_DEVICE_ID ]      = deviceId;
   msg[ JSON_DEVICE_SERIAL ]  = deviceSerial;
   msg[ JSON_TARGET ]         = target;
   msg[ JSON_EVENT ]          = event;
   msg[ JSON_INTERFACE ]      = interface;
   return msg;
}

bool MsgDevice::fromJson( const QJsonObject& a_msg )
{
   if ( a_msg.isEmpty() )
   {
      return false;
   }

   lastHeartBeat = a_msg[ JSON_LAST_HEARTBEAT ].toString();
   deviceId = a_msg[ JSON_DEVICE_ID ].toString();
   deviceSerial = a_msg[ JSON_DEVICE_SERIAL ].toString();
   target = a_msg[ JSON_TARGET ].toInt();
   event = a_msg[ JSON_EVENT ].toInt();
   interface = a_msg[ JSON_INTERFACE ].toString();
   return true;
}
