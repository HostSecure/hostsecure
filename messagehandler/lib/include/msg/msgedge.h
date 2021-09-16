#pragma once
#include "msgbase.h"
#include <QString>

//!
//! \brief The MsgEdge struct
//!
struct MsgEdge : public MsgBase
{
   MsgEdge() = default;
   MsgEdge( const MsgEdge& ) = default;

   QJsonObject toJson() const override;
   bool fromJson( const QJsonObject& a_msg ) override;

   QString macaddress;
   bool isOnline;
};
