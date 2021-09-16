#include "edgemodel.h"

//!
//! \brief EdgeModel::EdgeModel
//! \param a_parent
//!
EdgeModel::EdgeModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{
}

//!
//! \brief EdgeModel::rowCount
//! \param a_parent
//! \return Returns how many items are in the model
//!
int EdgeModel::rowCount( const QModelIndex &a_parent ) const
{
   Q_UNUSED( a_parent )
   return m_list.size();
}

//!
//! \brief EdgeModel::data
//! //! Gets the correct item in the list based on \a a_index and returns
//! the value based on \a a_role
//! \param a_index
//! \param a_role
//! \return QVariant to be displayed in the UI
//!
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

//!
//! \brief EdgeModel::roleNames
//! \return QHash describing roles that are accecable in QML
//!
QHash< int, QByteArray > EdgeModel::roleNames() const
{
   static QHash< int, QByteArray > roles = {
      { ID_ROLE,        "id" },
      { IS_ONLINE_ROLE, "isOnline" },
      { DEVICES_ROLE,   "deviceModel" },
   };
   return roles;
}

//!
//! \brief EdgeModel::edgeChanged
//! Add or update an edge and notify the UI to reload
//! \param a_edgeId
//! \param a_sample
//!
void EdgeModel::edgeChanged( const QString& a_edgeId, const MsgEdge& a_sample )
{
   qDebug() << "Edge updated: " << a_edgeId << a_sample.macaddress;

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

//!
//! \brief EdgeModel::edgeRemoved
//! Remove an edge and notify the UI to reload
//! \param a_edgeId
//!
void EdgeModel::edgeRemoved( const QString& a_edgeId )
{
   qDebug() << "Edge removed: " << a_edgeId;

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

//!
//! \brief EdgeModel::deviceChanged
//! Find the edge based on \a a_edgeId then pass the received \a a_deviceId and \a a_sample
//! to the associated edge.
//! \param a_edgeId
//! \param a_deviceId
//! \param a_sample
//!
void EdgeModel::deviceChanged( const QString& a_edgeId, const QString& a_deviceId, const MsgDevice& a_sample )
{
   qDebug() << "Device updated: " << a_edgeId << a_deviceId << a_sample.deviceSerial;

   auto iter = std::find_if( m_list.begin(),
                             m_list.end(),
                             [ a_edgeId ]( const auto& a_item ) {
      return a_item.edge.macaddress == a_edgeId;
   } );

   if ( iter != m_list.end() )
   {
      iter->deviceModel->deviceChanged( a_deviceId, a_sample );
   }
   else
   {
      qInfo() << __PRETTY_FUNCTION__ << "Device added before for unknown edge..";
   }
}

//!
//! \brief EdgeModel::deviceRemoved
//! Find the edge based on \a a_edgeId then pass the received \a a_deviceId and
//! to the associated edge to be removed.
//! \param a_edgeId
//! \param a_deviceId
//!
void EdgeModel::deviceRemoved( const QString& a_edgeId, const QString& a_deviceId )
{
   qDebug() << "Device removed: " << a_edgeId << a_deviceId;

   auto iter = std::find_if( m_list.begin(),
                             m_list.end(),
                             [ a_edgeId ]( const auto& a_item ) {
      return a_item.edge.macaddress == a_edgeId;
   } );

   if ( iter != m_list.end() )
   {
      iter->deviceModel->deviceRemoved( a_deviceId );
   }
   else
   {
      qInfo() << __PRETTY_FUNCTION__ << "Device not found..";
   }
}
