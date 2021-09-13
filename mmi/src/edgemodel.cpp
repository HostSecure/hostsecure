#include "edgemodel.h"

EdgeModel::EdgeModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{
}

int EdgeModel::rowCount( const QModelIndex &a_parent ) const
{
   Q_UNUSED( a_parent )
   return m_list.size();
}

QVariant EdgeModel::data( const QModelIndex &a_index, int a_role ) const
{
   if ( !a_index.isValid() )
      return QVariant();

   const auto& item = m_list.at( a_index.row() );

   switch ( a_role ) {
   case ID_ROLE:        return item.edge.macaddress;
   case IS_ONLINE_ROLE: return item.edge.isOnline;
   case DEVICES_ROLE:   return QVariant::fromValue( item.deviceModel );
   }
   return QVariant();
}

QHash< int, QByteArray > EdgeModel::roleNames() const
{
   static QHash< int, QByteArray > roles = {
      { ID_ROLE,        "id" },
      { IS_ONLINE_ROLE, "isOnline" },
      { DEVICES_ROLE,   "deviceModel" },
   };
   return roles;
}

void EdgeModel::edgeChanged( const QString& a_edgeId, const MsgEdge& a_sample )
{
   qInfo() << "Edge updated: " << a_edgeId << a_sample.macaddress;

   auto iter = std::find_if( m_list.begin(),
                             m_list.end(),
                             [ a_edgeId ]( const auto& a_item ) {
      return a_item.edge.macaddress == a_edgeId;
   } );

   if ( iter != m_list.end() )
   {
      const auto idx = std::distance( std::begin( m_list ), iter );
      m_list[ idx ].edge = a_sample;
      emit dataChanged( index( idx ), index( idx ) );
   }
   else
   {
      beginInsertRows( QModelIndex(), m_list.size(), m_list.size() );
      Data data;
      data.edge = a_sample;
      data.deviceModel = new DeviceModel( this );
      m_list.push_back( data );
      endInsertRows();
   }
}

void EdgeModel::edgeRemoved( const QString& a_edgeId )
{
   qInfo() << "Edge removed: " << a_edgeId;

   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_edgeId ]( const auto& a_item ) {
      return a_edgeId == a_item.edge.macaddress;
   } );

   if ( iter != std::end( m_list ) )
   {
      // Don't delete the edge just tag it as offline
      const auto idx = std::distance( std::begin( m_list ), iter );
      m_list[ idx ].edge.isOnline = false;
      emit dataChanged( index( idx ), index( idx ) );
   }
}

void EdgeModel::deviceChanged( const QString& a_edgeId, const QString& a_deviceId, const MsgDevice& a_sample )
{
   qInfo() << "Device updated: " << a_edgeId << a_deviceId << a_sample.deviceSerial;

   auto iter = std::find_if( m_list.begin(),
                             m_list.end(),
                             [ a_edgeId ]( const auto& a_item ) {
      return a_item.edge.macaddress == a_edgeId;
   } );

   if ( iter != m_list.end() )
   {
      const auto idx = std::distance( std::begin( m_list ), iter );
      m_list[ idx ].deviceModel->deviceChanged( a_deviceId, a_sample );
   }
   else
   {
      qInfo() << __PRETTY_FUNCTION__ << "Device added before for unknown edge..";
   }
}

void EdgeModel::deviceRemoved( const QString& a_edgeId, const QString& a_deviceId )
{
   qInfo() << "Device removed: " << a_edgeId << a_deviceId;

   auto iter = std::find_if( m_list.begin(),
                             m_list.end(),
                             [ a_edgeId ]( const auto& a_item ) {
      return a_item.edge.macaddress == a_edgeId;
   } );

   if ( iter != m_list.end() )
   {
      const auto idx = std::distance( std::begin( m_list ), iter );
      m_list[ idx ].deviceModel->deviceRemoved( a_deviceId );
   }
   else
   {
      qInfo() << __PRETTY_FUNCTION__ << "Device not found..";
   }
}
