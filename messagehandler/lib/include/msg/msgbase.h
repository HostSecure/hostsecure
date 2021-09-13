#pragma once
#include <QJsonObject>

struct MsgBase
{
   virtual QJsonObject toJson() const = 0;
   virtual bool fromJson( const QJsonObject& a_msg ) = 0;

protected:
   MsgBase() = default;
   MsgBase( const MsgBase& ) = default;
   bool operator==( const MsgBase&) const = default;
};
