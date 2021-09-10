#pragma once

#include <QObject>
#include <string>
#include <QtNetwork/QHostAddress>
#include <QtMqtt/QtMqtt>



class UplinkHandler : public QObject
{
    Q_OBJECT
public:
    explicit UplinkHandler(QObject *parent = nullptr);




    QString fetchMac();

private:
    QString m_hardwareAddress;


signals:

public slots:

    void devicePolicyUpload(QString device_id, QString device_serial, uint target, QString interface);
    void devicePresenceUpload(QString device_id, QString device_serial, uint target, QString interface, uint event);
//    void rulesetUpload(QString device_id, QString device_serial, uint target, QString interface);
 //   void rulesetDownload(QString device_id, QString device_serial, uint target, QString interface);
  //  void currentDevicesUpload(QString device_id, QString device_serial, uint target, QString interface);



};

