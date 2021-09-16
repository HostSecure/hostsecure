#include "devicemodel.h"

//!
//! \brief DeviceModel::DeviceModel
//! \param a_parent
//!
DeviceModel::DeviceModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{ }

//!
//! \brief DeviceModel::rowCount
//! \param a_parent
//! \return How many items are in the model
//!
int DeviceModel::rowCount( const QModelIndex& a_parent ) const
{
   Q_UNUSED( a_parent )
   return m_list.size();
}

//!
//! \brief DeviceModel::data
//! Gets the correct item in the list based on \a a_index and returns
//! the value based on \a a_role
//! \param a_index
//! \param a_role
//! \return QVariant to be displayed in the UI
//!
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

//!
//! \brief DeviceModel::roleNames
//! \return QHash describing roles that are accecable in QML
//!
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

//!
//! \brief DeviceModel::deviceChanged
//! Update the device in the list and notify the UI to reload
//! \param a_deviceId
//! \param a_sample
//!
void DeviceModel::deviceChanged( const QString& a_deviceId, const MsgDevice& a_sample )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_deviceId ]( const auto& a_item ) {
      return a_deviceId == a_item.deviceId;
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

//!
//! \brief DeviceModel::deviceRemoved
//! Remove \a a_deviceId item from list and notify the UI to reload
//! \param a_deviceId
//!
void DeviceModel::deviceRemoved( const QString& a_deviceId )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_deviceId ]( const auto& a_item ) {
      return a_deviceId == a_item.deviceId;
   } );

   if ( iter != std::end( m_list ) )
   {
      const auto index = std::distance( std::begin( m_list ), iter );
      beginRemoveRows( QModelIndex(), index, index );
      m_list.erase( iter );
      endRemoveRows();
   }
}

