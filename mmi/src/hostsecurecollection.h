#pragma once
#include <QObject>
#include <mmimqttclient.h>
#include "edgemodel.h"

//!
//! \brief The HostSecureCollection class
//! Collection class to connect all depended classes with signals and slots
//!
class HostSecureCollection : public QObject
{
   Q_OBJECT
   Q_PROPERTY( MmiMqttClient*  mmiMqttClient MEMBER m_mmiMqttClient CONSTANT )
   Q_PROPERTY( EdgeModel*      edgeModel     MEMBER m_edgeModel     CONSTANT )

public:
   explicit HostSecureCollection( QObject* a_parent = nullptr );

private:
   Q_DISABLE_COPY_MOVE( HostSecureCollection )

   MmiMqttClient* m_mmiMqttClient { nullptr };
   EdgeModel* m_edgeModel { nullptr };
};
