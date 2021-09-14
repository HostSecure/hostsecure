#pragma once
#include <QObject>
#include <mmimqttclient.h>
#include "edgemodel.h"

class HostSecureCollection : public QObject
{
   Q_OBJECT
   Q_PROPERTY( MmiMqttClient*  mmiMqttClient MEMBER m_mmiMqttClient CONSTANT )
   Q_PROPERTY( EdgeModel*      edgeModel     MEMBER m_edgeModel     CONSTANT )

public:
   explicit HostSecureCollection( QObject* a_parent = nullptr );

public slots:
   void incrementEdgeId();
   void sendEdgeMessage();
   void sendDeviceMessage();

private:
   Q_DISABLE_COPY_MOVE( HostSecureCollection )

   MmiMqttClient* m_mmiMqttClient { nullptr };
   EdgeModel* m_edgeModel { nullptr };
};
