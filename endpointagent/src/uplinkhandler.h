#pragma once

#include <QObject>
#include <QtNetwork>
#include "EdgeHandler.h"



class UplinkHandler : public QObject
{
    Q_OBJECT
public:
    explicit UplinkHandler(QObject *parent = nullptr);

    QString fetchMac();
    QString getHardwareAddress();

private:
    QString m_hardwareAddress;
    /* Configure broker (server) */
//    QString broker_addr = "test.mosquitto.org";
//    quint16 broker_port = 1883;

    /* Initialize a unique pointer to EdgeHandler */
//    QScopedPointer<MessageHandler::Gateway::EdgeHandler> edge_handler;


signals:
    void updateEdgeHandler(QString device_id, QString device_serial, uint target, uint event, QString interface, QString machine_address);
    void appendServiceRule(QString device_id,QString device_serial ,uint target,QString interface);

public slots:

    void devicePolicyUpload(QString device_id, QString device_serial, uint target, QString interface);
    void devicePresenceUpload(QString device_id, QString device_serial, uint target, QString interface, uint event);
//    void rulesetUpload(QString device_id, QString device_serial, uint target, QString interface);
    void rulesetDownload(QString device_id, QString device_serial, uint target, QString interface);
  //  void currentDevicesUpload(QString device_id, QString device_serial, uint target, QString interface);



};

