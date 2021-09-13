#include "uplinkhandler.h"
#include <QDebug>
#include <iostream>

UplinkHandler::UplinkHandler(QObject *parent) : QObject(parent)
{
    m_hardwareAddress = fetchMac();

//    QScopedPointer<MessageHandler::Gateway::EdgeHandler> edge_handler = QScopedPointer<MessageHandler::Gateway::EdgeHandler>(
//        new MessageHandler::Gateway::EdgeHandler(broker_addr, broker_port)
//    );
//    auto connRes = QObject::connect(this->edge_handler.get(), &MessageHandler::Gateway::EdgeHandler::newEdgeServiceRule,
//            this, &UplinkHandler::rulesetDownload);
//    if(!connRes)
//    {
//        qDebug() << "Connection Unsuccessful";
//    }
//    connRes = QObject::connect(this, &UplinkHandler::updateEdgeHandler,
//            this->edge_handler.get(),&MessageHandler::Gateway::EdgeHandler::newDevicePresence);
//    if(!connRes)
//    {
//        qDebug() << "Connection Unsuccessful";
//    }

//    edge_handler->register_new_edge(m_hardwareAddress);
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

QString UplinkHandler::getHardwareAddress()
{
   return this->m_hardwareAddress;
}

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void UplinkHandler::devicePresenceUpload(QString device_id, QString device_serial, uint target, QString interface, uint event)
{
    qDebug() << "Updating EdgeHandler";
    emit updateEdgeHandler(device_id, device_serial, target, event, interface, m_hardwareAddress);
    //edge_handler->newDevicePresence(device_id, device_serial,target,event, interface, m_hardwareAddress);

}

void UplinkHandler::rulesetDownload(QString device_id, QString device_serial, uint target, QString interface)
{
    qDebug() << "New rule from cloud";
    emit appendServiceRule(device_id, device_serial,target,interface);

}
void UplinkHandler::devicePolicyUpload(QString device_id, QString device_serial, uint target, QString interface)
{
    qDebug() << "Updating EdgeHandler";
    emit updateEdgeHandler(device_id, device_serial, target, 0, interface, m_hardwareAddress);
    //edge_handler->newDevicePresence(device_id, device_serial,target,0, interface, m_hardwareAddress);

}



