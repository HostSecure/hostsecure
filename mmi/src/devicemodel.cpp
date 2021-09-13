#include "devicemodel.h"

DeviceModel::DeviceModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{ }

int DeviceModel::rowCount( const QModelIndex& a_parent ) const
{
   Q_UNUSED( a_parent )
   return m_list.size();
}

QVariant DeviceModel::data( const QModelIndex& a_index, int a_role ) const
{
   if ( !a_index.isValid() )
      return QVariant();

   const auto& item = m_list.at( a_index.row() );

   switch ( a_role ) {
   case LAST_HEART_BEAT_ROLE: return item.lastHeartBeat;
   case DEVICE_ID_ROLE:       return item.deviceId;
   case DEVICE_SERIAL_ROLE:   return item.deviceSerial;
   case TARGET_ROLE:          return item.target;
   case EVENT_ROLE:           return item.event;
   case INTERFACE_ROLE:       return item.interface;
   }
   return QVariant();
}

QHash< int, QByteArray > DeviceModel::roleNames() const
{
   static QHash< int, QByteArray > roles = {
      { LAST_HEART_BEAT_ROLE, "lastHeartBeat" },
      { DEVICE_ID_ROLE,       "deviceId" },
      { DEVICE_SERIAL_ROLE,   "deviceSerial" },
      { TARGET_ROLE,          "target" },
      { EVENT_ROLE,           "event" },
      { INTERFACE_ROLE,       "interface" },
   };
   return roles;
}

void DeviceModel::deviceChanged( const QString& a_deviceId, const MsgDevice& a_sample )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_deviceId ]( const auto& a_item ) {
      return a_deviceId == a_item.deviceSerial;
   } );

   if ( iter != std::end( m_list ) )
   {
      const auto idx = std::distance( std::begin( m_list ), iter );
      m_list[ idx ] = a_sample;
      emit dataChanged( index( idx ), index( idx ) );
   }
   else
   {
      beginInsertRows( QModelIndex(), m_list.size(), m_list.size() );
      m_list.push_back( a_sample );
      endInsertRows();
   }
}

void DeviceModel::deviceRemoved( const QString& a_deviceId )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_deviceId ]( const auto& a_item ) {
      return a_deviceId == a_item.deviceSerial;
   } );

   if ( iter != std::end( m_list ) )
   {
      const auto index = std::distance( std::begin( m_list ), iter );
      beginRemoveRows( QModelIndex(), index, index );
      m_list.erase( iter );
      endRemoveRows();
   }
}

