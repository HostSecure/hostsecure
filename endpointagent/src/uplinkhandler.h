#pragma once
#include <QObject>
#include <QtNetwork>
#include <mqttclientbase.h>


class UplinkHandler : public MqttClientBase
{
   Q_OBJECT
public:
   explicit UplinkHandler(QObject* a_parent = nullptr);
   ~UplinkHandler();

signals:
   void appendServiceRule(QString device_id,QString device_serial ,uint target,QString interface);

public slots:
   void devicePolicyUpload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface);
   void devicePresenceUpload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface, const uint a_event);
   void rulesetDownload(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface);

private:
   void brokerConnected() override;
   QString m_hardwareAddress;
   QString m_hardwareAddressTopic;
};

