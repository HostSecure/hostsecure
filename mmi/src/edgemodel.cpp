#include "edgemodel.h"

EdgeModel::EdgeModel( QObject* a_parent )
   : QAbstractListModel { a_parent }
{
   EdgeData_t edge1 {
      1,
      "Edge 1",
      "Connected",
      { "Corsair", "Kingston" }
   };

   EdgeData_t edge2 {
      1,
      "Edge 1",
      "Connected",
      { "Corsair", "Kingston" }
   };

   EdgeData_t edge3 {
      1,
      "Edge 1",
      "Connected",
      { "Corsair", "Kingston" }
   };

   m_list = {
      edge1, edge2, edge3
   };
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
   case ID_ROLE:     return item.id;
   case NAME_ROLE:   return item.name;
   case STATUS_ROLE: return item.stauts;
   case USB_ROLE:    return item.usb;
   }
   return QVariant();
}

QHash< int, QByteArray > EdgeModel::roleNames() const
{
   static QHash< int, QByteArray > roles = {
      { ID_ROLE,     "id" },
      { NAME_ROLE,   "name" },
      { STATUS_ROLE, "status" },
      { USB_ROLE,    "usb" },
   };
   return roles;
}
