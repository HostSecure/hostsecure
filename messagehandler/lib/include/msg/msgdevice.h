#pragma once
#include "msgbase.h"
#include <QString>

struct MsgDevice : public MsgBase
{
   MsgDevice() = default;
   MsgDevice( const MsgDevice& ) = default;
   bool operator==( const MsgDevice& ) const = default;

   QJsonObject toJson() const override;
   bool fromJson( const QJsonObject& a_msg ) override;

   QString lastHeartBeat;
   QString deviceId;
   QString deviceSerial;
   int target;
   int event;
   QString interface;
};
