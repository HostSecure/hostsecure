#include "servicehandler.h"
#include "uplinkhandler.h"
#include <QDBusMetaType>
#include <sstream>

namespace
{
constexpr auto DBUS_SERVICE_NAME  =    "org.usbguard1";
constexpr auto DBUS_ROOT_INTERFACE =   "org.usbguard1";
constexpr auto DBUS_ROOT_PATH      =   "/org/usbguard1";
constexpr auto DBUS_POLICY_INTERFACE=  "org.usbguard.Policy1";
constexpr auto DBUS_POLICY_PATH     =  "/org/usbguard1/Policy";
constexpr auto DBUS_DEVICES_INTERFACE= "org.usbguard.Devices1";
constexpr auto DBUS_DEVICES_PATH     = "/org/usbguard1/Devices";
}

ServiceHandler::ServiceHandler(QObject *parent)
    : QObject(parent)
    , m_ifaceUSBGuardDevices(new QDBusInterface(DBUS_SERVICE_NAME, DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                  QDBusConnection::systemBus(), this))
    , m_ifaceUSBGuardPolicy(new QDBusInterface(DBUS_SERVICE_NAME, DBUS_POLICY_PATH, DBUS_POLICY_INTERFACE,
                                                  QDBusConnection::systemBus(), this))
    , m_uplinkHandler(new UplinkHandler(parent))
{
    qDBusRegisterMetaType<QMap<QString,QString>>();

    QDBusConnection system = QDBusConnection::systemBus();
    if(!system.isConnected())
    {
        qWarning() << "Not connected to System Bus";
    }
    auto resultDeviceIface =   system.systemBus().connect(DBUS_SERVICE_NAME,DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                         "DevicePresenceChanged", this,
                                                         SLOT(handleDevicePresenceChanged(uint,uint,uint,QString,QMap<QString,QString>)));
    auto resultPolicyIface =   system.systemBus().connect(DBUS_SERVICE_NAME,DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                         "DevicePolicyChanged", this,
                                                         SLOT(handleDevicePolicyChanged(uint,uint,uint,QString,uint,QMap<QString,QString>)));
    if(!resultDeviceIface)
    {
        qWarning() << "Connection USBGuard Device interface failed";
        qPrintable(QDBusConnection::systemBus().lastError().message());
    }
    if(!resultPolicyIface)
    {
        qWarning() << "Connection USBGuard Policy interface failed";
        qPrintable(QDBusConnection::systemBus().lastError().message());
    }

    connect(m_uplinkHandler.get(), &UplinkHandler::appendServiceRule, this, &ServiceHandler::appendRule);
}

ServiceHandler::~ServiceHandler()
{

}

void ServiceHandler::listRules(const QString& label)
{
    QDBusMessage ruleset = m_ifaceUSBGuardPolicy->call("listRules",label);
    qDebug() << ruleset; //a(us)
}

void ServiceHandler::appendRule(const QString& device_id, const QString& device_serial, const uint target, const QString& interface)
{
    uint parent_id = 1; // Add new rule to top of ruleset
    bool temporary = 0; // Add persistent rule
    std::stringstream sstr_rule;
    std::string rule_target = target ?  "block id " : "allow id ";
    sstr_rule << rule_target << device_id.toStdString() << " serial \"" << device_serial.toStdString() <<
            "\" with-interface { " << interface.toStdString() << " }";
    QString rule;
    rule = rule.fromStdString(sstr_rule.str());
    qDebug() << rule;

    QDBusMessage id = m_ifaceUSBGuardPolicy->call("appendRule", rule , parent_id,temporary );
    qDebug() << "New USB rule : " << id;
}

void ServiceHandler::removeRule(const uint id)
{

    m_ifaceUSBGuardDevices->call("removeRule",id);
}

void ServiceHandler::listDevices(const QString& query)
{
    QDBusMessage devices = m_ifaceUSBGuardDevices->call("listDevices",query);
    // qDebug() << devices.arguments().at(0)[0]; //a(us)
    // for( auto e : devices.arguments())
}

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void ServiceHandler::handleDevicePresenceChanged(const uint id, const uint event, const uint target,
                                                 const QString& device_rule, const QMap<QString, QString>& attributes)
{
    Q_UNUSED(id);
    Q_UNUSED(device_rule);

    m_uplinkHandler->devicePresenceUpload(attributes["id"], attributes["serial"], target, attributes["with-interface"], event);
}

void ServiceHandler::handleDevicePolicyChanged(const uint id, const uint target_old, const uint target_new,
                                               const QString& device_rule, const uint rule_id, const QMap<QString, QString>& attributes)
{
    Q_UNUSED(id);
    Q_UNUSED(device_rule);
    Q_UNUSED(rule_id);
    Q_UNUSED(target_old);
    m_uplinkHandler->devicePolicyUpload(attributes["id"], attributes["serial"], target_new, attributes["with-interface"]);
}

