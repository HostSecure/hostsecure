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
class UplinkHandler;
class ServiceHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServiceHandler(QObject *parent = nullptr);
    ~ServiceHandler();

//    QDBusInterface *m_ifaceUSBGuardRoot;
    QDBusInterface *m_ifaceUSBGuardPolicy;
    QDBusInterface *m_ifaceUSBGuardDevices;


    //USBGUARD_ROOT
//    void getParameter(QString name); // out s value
//    void setParameter(QString name); // out s previous_value
    //USBGUARD_POLICY
    void listRules(QString label); // out a(us) ruleset
    void removeRule(uint id); // out
    //USBGUARD_DEVICE
    void listDevices(QString query); // out a(us) devices
    void applyDevicePolicy(uint id, uint target, bool permanent);

signals:
    void devicePresenceUpdate(QString device_id, QString device_serial,
                             uint target, QString interface, uint event);

    void devicePolicyUpdate(QString device_id, QString device_serial,
                             uint target, QString interface);

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
    void appendRule(QString device_id,QString device_serial, uint target, QString interface);
//    void handleDevicePolicyApplied(uint id, uint target_new, uint device_rule, uint rule_id,
//                             QMap < QString, QString  > attributes);
// TODO

private:
    std::unique_ptr<UplinkHandler> m_uplinkHandler;
};

