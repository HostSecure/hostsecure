#pragma once

#include <QObject>
#include <QtDBus>
#include <QMap>
#include <QString>


namespace
{
QString DBUS_SERVICE_NAME  =    "org.usbguard1";
QString DBUS_ROOT_INTERFACE =   "org.usbguard1";
QString DBUS_ROOT_PATH      =   "/org/usbguard1";
QString DBUS_POLICY_INTERFACE=  "org.usbguard.Policy1";
QString DBUS_POLICY_PATH     =  "/org/usbguard1/Policy";
QString DBUS_DEVICES_INTERFACE= "org.usbguard.Devices1";
QString DBUS_DEVICES_PATH     = "/org/usbguard1/Devices";
}
class USBServiceHandler : public QObject
{
    Q_OBJECT
public:
    explicit USBServiceHandler(QObject *parent = nullptr);

//    QDBusInterface *ifaceUSBGuardRoot;
    QDBusInterface *ifaceUSBGuardPolicy;
    QDBusInterface *ifaceUSBGuardDevices;


    //USBGUARD_ROOT
//    void getParameter(QString name); // out s value
//    void setParameter(QString name); // out s previous_value
    //USBGUARD_POLICY
    void listRules(QString label); // out a(us) ruleset
    void appendRule(QString rule, uint parent_id, bool temporary); // out u id
    void removeRule(uint id); // out
    //USBGUARD_DEVICE
    void listDevices(QString query); // out a(us) devices
    void applyDevicePolicy(uint id, uint target, bool permanent);


public slots:
    //USBGUARD_ROOT
//    void handlePropertyParameterChanged(QString name, QString value_old, QString value_new);
//    void handleExceptionMessage(QString context, QString object, QString reason);
// TODO
    //USBGUARD_POLICY


    //USBGUARD_DEVICE
    void handleDevicePresenceChanged(uint id, uint event, uint target, QString device_rule,
                               QMap<QString, QString> attributes);
    void handleDevicePolicyChanged(uint id, uint target_old, uint target_new, QString device_rule, uint rule_id,
                             QMap<QString, QString> attributes);
//    void handleDevicePolicyApplied(uint id, uint target_new, uint device_rule, uint rule_id,
//                             QMap < QString, QString  > attributes);
// TODO




};

