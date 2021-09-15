#pragma once
#include <QAbstractListModel>
#include <msg/msgdevice.h>

//!
//! \brief The DeviceModel class
//! Exposes a set of devices to QML
//!
class DeviceModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit DeviceModel( QObject* a_parent = nullptr );

   int rowCount( const QModelIndex& a_parent ) const override;
   QVariant data( const QModelIndex& a_index, int a_role ) const override;
   QHash< int, QByteArray > roleNames() const override;

   void deviceChanged( const QString& a_deviceId, const MsgDevice& a_sample );
   void deviceRemoved( const QString& a_deviceId );

   enum Roles_e {
      LAST_HEART_BEAT_ROLE = Qt::UserRole + 1,
      DEVICE_ID_ROLE,
      DEVICE_SERIAL_ROLE,
      TARGET_ROLE,
      EVENT_ROLE,
      INTERFACE_ROLE,
   };

private:
   Q_DISABLE_COPY_MOVE( DeviceModel )
   std::vector< MsgDevice > m_list;
};

