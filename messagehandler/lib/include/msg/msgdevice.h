#pragma once
#include "msgbase.h"
#include <QString>

//!
//! \brief The MsgDevice struct
//!
struct MsgDevice : public MsgBase
{
   MsgDevice() = default;
   MsgDevice( const MsgDevice& ) = default;

   QJsonObject toJson() const override;
   bool fromJson( const QJsonObject& a_msg ) override;

   QString lastHeartBeat;
   QString deviceId;
   QString deviceSerial;
   int target;
   int event;
   QString interface;
};
