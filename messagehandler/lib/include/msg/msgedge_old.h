#pragma once
#include "msgbase.h"
#include <QString>

struct MsgEdge : public MsgBase
{
   MsgEdge( int a_id = 0,
            const QString a_name = "",
            bool a_connected = false,
            QStringList a_devices = QStringList() );
   MsgEdge( const MsgEdge& ) = default;
   bool operator==( const MsgEdge&) const = default;

   QJsonObject toJson() const override;
   bool fromJson( const QJsonObject& a_msg ) override;

   int id { 0 };
   QString name;
   bool connected;
   QStringList devices;
};
