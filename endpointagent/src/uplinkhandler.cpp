#include "uplinkhandler.h"
#include <QDebug>
#include <iostream>

UplinkHandler::UplinkHandler(QObject *parent) : QObject(parent)
{
    m_hardwareAddress = fetchMac();
}

QString UplinkHandler::fetchMac()
{
    QString mac = "";
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if(interface.type() == QNetworkInterface::InterfaceType::Wifi )
           // || interface.type() == QNetworkInterface::InterfaceType::Ethernet)
        {
            mac = interface.hardwareAddress();
        }
    }
    return mac;
}

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void UplinkHandler::devicePresenceUpload(QString device_id, QString device_serial, uint target, QString interface, uint event)
{
    qDebug() << "Device Status Update:" <<
                "\nDevice ID " << device_id <<
                "\nDevice Serial " << device_serial <<
                "\nWith-Interface " << interface <<
                "\nEvent code " << event <<
                "\nTarget code " << target <<
                "\nEdge node " << m_hardwareAddress << "\n";

}
void UplinkHandler::devicePolicyUpload(QString device_id, QString device_serial, uint target, QString interface)
{
    qDebug() << "Device Status Update:" <<
                "\nDevice ID " << device_id <<
                "\nDevice Serial " << device_serial <<
                "\nWith-Interface " << interface <<
                "\nTarget code " << target <<
                "\nEdge node " << m_hardwareAddress << "\n" ;

}



