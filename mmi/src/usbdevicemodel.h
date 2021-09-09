#pragma once
#include <QAbstractListModel>
#include <qqml.h>


class UsbDeviceModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit UsbDeviceModel( QObject* a_parent = nullptr );

   int rowCount( const QModelIndex& a_parent ) const override;
   QVariant data( const QModelIndex& a_index, int a_role ) const override;
   QHash< int, QByteArray > roleNames() const override;

   enum Roles_e {
      NAME_ROLE = Qt::UserRole + 1,
      APPROVED_ROLE,
   };

   struct UsbDevice_t
   {
      int id;
      QString name;
      bool approved;
   };

   void append( const UsbDevice_t& a_usbDevice );
   void update( const UsbDevice_t& a_usbDevice );
   void remove( int a_deviceId );

private:
   Q_DISABLE_COPY_MOVE( UsbDeviceModel )

   std::vector< UsbDevice_t > m_list;
};

