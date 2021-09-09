#pragma once

#include <QObject>
#include <QByteArray>
#include <string>
#include <QtNetwork/QHostAddress>


typedef unsigned char byte;

class UplinkHandler : public QObject
{
    Q_OBJECT
public:
    explicit UplinkHandler(QObject *parent = nullptr);

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove


void deviceStatusUpload( std::string device_id, uint device_serial,  uint event, uint target, uint endpoint);
void deviceStatusDownload(std::string device_id, uint device_serial,  uint event, uint target, uint endpoint);
void rulesetUpload(std::string device_id, uint device_serial, uint target, std::string interface, uint endpoint);
void rulesetDownload(std::string device_id, uint device_serial, uint target, std::string interface, uint endpoint);
void currentDevicesUpload(std::string device_id, uint device_serial, uint target, std::string interface, uint endpoint);



signals:


};

