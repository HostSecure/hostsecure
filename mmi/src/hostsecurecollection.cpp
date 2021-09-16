#include "hostsecurecollection.h"

//!
//! \brief HostSecureCollection::HostSecureCollection
//! Create objects and setup connections
//! \param a_parent
//!
HostSecureCollection::HostSecureCollection( QObject* a_parent )
   : QObject { a_parent }
   , m_mmiMqttClient { new MmiMqttClient( this ) }
   , m_edgeModel { new EdgeModel( this ) }
{
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::edgeChanged, m_edgeModel, &EdgeModel::edgeChanged );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::edgeRemoved, m_edgeModel, &EdgeModel::edgeRemoved );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::deviceChanged, m_edgeModel, &EdgeModel::deviceChanged );
   QObject::connect( m_mmiMqttClient, &MmiMqttClient::deviceRemoved, m_edgeModel, &EdgeModel::deviceRemoved );
}
