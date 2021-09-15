#pragma once

#include <QObject>
#include <QtNetwork>
#include <mqttclientbase.h>



class UplinkHandler : public MqttClientBase
{
    Q_OBJECT
public:
    explicit UplinkHandler(QObject *parent = nullptr);
    ~UplinkHandler();


signals:
    void appendServiceRule(QString device_id,QString device_serial ,uint target,QString interface);

public slots:

    void devicePolicyUpload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface);
    void devicePresenceUpload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface, const uint event);
    void rulesetDownload(const QString& device_id, const QString& device_serial, const uint target, const QString& interface);


private:
    QString fetchMac();
    void brokerConnected() override;

    QString m_hardwareAddress;

    /* Configure broker (server) */
//    QString broker_addr = "test.mosquitto.org";
//    quint16 broker_port = 1883;

    /* Initialize a unique pointer to EdgeHandler */
//    QScopedPointer<MessageHandler::Gateway::EdgeHandler> edge_handler;





};

