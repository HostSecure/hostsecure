#pragma once
#include <QJsonObject>

//!
//! \brief The MsgBase struct
//!
struct MsgBase
{
   virtual QJsonObject toJson() const = 0;
   virtual bool fromJson( const QJsonObject& a_msg ) = 0;

protected:
   MsgBase() = default;
   MsgBase( const MsgBase& ) = default;
};
