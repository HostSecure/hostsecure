#include "hostsecurecollection.h"


HostSecureCollection::HostSecureCollection( QObject* a_parent )
   : QObject { a_parent }
   , m_edgeModel { new EdgeModel( this ) }
   , m_usbDeviceModel { new UsbDeviceModel( this ) }
{
}


