#include "usbdevicemodel.h"


UsbDeviceModel::UsbDeviceModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{
   for ( int i = 0; i < 5; ++i ) {
      UsbDevice_t dsa {
         i,
               QString( "Device %1" ).arg( i ),
               false
      };
      m_list.push_back( dsa );
   }
}

int UsbDeviceModel::rowCount( const QModelIndex& a_parent ) const
{
   Q_UNUSED( a_parent )
   return m_list.size();
}

QVariant UsbDeviceModel::data( const QModelIndex& a_index, int a_role ) const
{
   if ( !a_index.isValid() )
      return QVariant();

   const auto& item = m_list.at( a_index.row() );

   switch ( a_role ) {
   case UsbDeviceModel::NAME_ROLE:     return item.name;
   case UsbDeviceModel::APPROVED_ROLE: return item.approved;
   }
   return QVariant();
}

QHash< int, QByteArray > UsbDeviceModel::roleNames() const
{
   static QHash< int, QByteArray > roles = {
      { NAME_ROLE,     "name" },
      { APPROVED_ROLE, "approved" },
   };
   return roles;
}

void UsbDeviceModel::append( const UsbDevice_t& a_usbDevice )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ &a_usbDevice ]( const auto& a_item ) {
      return a_usbDevice.id == a_item.id;
   } );

   if ( iter != std::end( m_list ) )
   {
      update( a_usbDevice );
   }
   else
   {
      beginInsertRows( QModelIndex(), m_list.size(), m_list.size() );
      m_list.push_back( a_usbDevice );
      endInsertRows();
   }
}

void UsbDeviceModel::update( const UsbDevice_t& a_usbDevice )
{
   Q_UNUSED( a_usbDevice )
   qInfo() << __PRETTY_FUNCTION__;
}

void UsbDeviceModel::remove( int a_deviceId )
{
   const auto iter = std::find_if( std::begin( m_list ),
                                   std::end( m_list ),
                                   [ a_deviceId ]( const auto& a_item ) {
      return a_deviceId == a_item.id;
   } );

   if ( iter != std::end( m_list ) )
   {
      const auto index = std::distance( std::begin( m_list ), iter );
      beginRemoveRows( QModelIndex(), index, index );
      m_list.erase( iter );
      endRemoveRows();
   }
}

