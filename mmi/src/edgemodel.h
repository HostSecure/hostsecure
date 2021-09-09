#pragma once
#include <QAbstractListModel>
#include <qqml.h>

class EdgeModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit EdgeModel( QObject* a_parent = nullptr );

   int rowCount( const QModelIndex& a_parent = QModelIndex() ) const override;
   QVariant data( const QModelIndex& a_index, int a_role = Qt::DisplayRole ) const override;
   QHash< int, QByteArray > roleNames() const override;

   enum Roles {
      ID_ROLE = Qt::UserRole + 1,
      NAME_ROLE,
      STATUS_ROLE,
      USB_ROLE,
   };

   struct EdgeData_t {
      int id;
      QString name;
      QString stauts;
      QStringList usb;
   };

private:
   Q_DISABLE_COPY_MOVE( EdgeModel )

   std::vector< EdgeData_t > m_list;
};
