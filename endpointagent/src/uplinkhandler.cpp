#include "uplinkhandler.h"
#include <msg/msgedge.h>
#include <msg/msgdevice.h>
#include <QDebug>
#include <iostream>

UplinkHandler::UplinkHandler(QObject *parent) : MqttClientBase(parent)
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

UplinkHandler::~UplinkHandler()
{
    // Worth a shot...
    MsgEdge edge;
    edge.macaddress = m_hardwareAddress;
    edge.isOnline = false;
    publish(QMqttTopicName(QString("edges/%1").arg(m_hardwareAddress)), edge);
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

void UplinkHandler::brokerConnected()
{
    qInfo() << "Broker connected!";

    MsgEdge edge;
    edge.macaddress = m_hardwareAddress;
    edge.isOnline = true;

    publish(QMqttTopicName(QString("edges/%1").arg(m_hardwareAddress)), edge);
}

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void UplinkHandler::devicePresenceUpload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface, const uint event)
{
    qDebug() << "Updating devicePresenceUpload";
    if(event == 3)
    {
        publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, device_id )), QByteArray());
    }
    else
    {
        MsgDevice msg;
        msg.deviceId = device_id;
        msg.deviceSerial = device_serial;
        msg.target = target;
        msg.interface = interface;
        msg.event = event;
        msg.lastHeartBeat = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss.zzz");
        publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, device_id ) ), msg);
    }
}

void UplinkHandler::rulesetDownload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface)
{
    qDebug() << "New rule from cloud";
    emit appendServiceRule(device_id, device_serial,target,interface);

}

void UplinkHandler::devicePolicyUpload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface)
{
    qDebug() << "Updating devicePolicyUpload";
    MsgDevice msg;
    msg.deviceId = device_id;
    msg.deviceSerial = device_serial;
    msg.target = target;
    msg.interface = interface;
    msg.event = 0;
    msg.lastHeartBeat = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss.zzz");
    publish(QMqttTopicName( QString( "edges/%1/%2" ).arg( m_hardwareAddress, device_id ) ), msg);
}



