#pragma once
#include "devicemodel.h"
#include <QAbstractListModel>
#include <msg/msgedge.h>
#include <msg/msgdevice.h>

class EdgeModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit EdgeModel( QObject* a_parent = nullptr );

   int rowCount( const QModelIndex& a_parent = QModelIndex() ) const override;
   QVariant data( const QModelIndex& a_index, int a_role = Qt::DisplayRole ) const override;
   QHash< int, QByteArray > roleNames() const override;

   void edgeChanged( const QString& a_edgeId, const MsgEdge& a_sample );
   void edgeRemoved( const QString& a_edgeId );

   void deviceChanged( const QString& a_edgeId, const QString& a_deviceId, const MsgDevice& a_sample );
   void deviceRemoved( const QString& a_edgeId, const QString& a_deviceId );

   enum Roles {
      ID_ROLE = Qt::UserRole + 1,
      IS_ONLINE_ROLE,
      DEVICES_ROLE,
   };

private:
   Q_DISABLE_COPY_MOVE( EdgeModel )

   struct Data
   {
      MsgEdge edge;
      DeviceModel* deviceModel { nullptr };
   };

   std::vector< Data > m_list;
};
