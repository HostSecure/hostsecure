#pragma once
#include <QObject>
#include "edgemodel.h"
#include "usbdevicemodel.h"

class HostSecureCollection : public QObject
{
    Q_OBJECT
    Q_PROPERTY( EdgeModel*      edgeModel      MEMBER m_edgeModel      CONSTANT )
    Q_PROPERTY( UsbDeviceModel* usbDeviceModel MEMBER m_usbDeviceModel CONSTANT )

public:
    explicit HostSecureCollection( QObject* a_parent = nullptr );

private:
    Q_DISABLE_COPY_MOVE( HostSecureCollection )

    EdgeModel* m_edgeModel { nullptr };
    UsbDeviceModel* m_usbDeviceModel { nullptr };
};
