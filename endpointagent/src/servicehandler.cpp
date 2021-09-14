#include "servicehandler.h"
#include "uplinkhandler.h"
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusArgument>
#include <QDBusMetaType>
#include <sstream>
#include <string>

ServiceHandler::ServiceHandler(QObject *parent)
    : QObject(parent)
    , m_uplinkHandler(new UplinkHandler(parent))
{

    qDBusRegisterMetaType<QMap<QString,QString>>();

    m_ifaceUSBGuardDevices = new QDBusInterface(DBUS_SERVICE_NAME, DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                  QDBusConnection::systemBus(), this);
    m_ifaceUSBGuardPolicy = new QDBusInterface(DBUS_SERVICE_NAME, DBUS_POLICY_PATH, DBUS_POLICY_INTERFACE,
                                                  QDBusConnection::systemBus(), this);

    QDBusConnection system = QDBusConnection::systemBus();
    if(!system.isConnected())
    {
        qDebug() << "Not connected to bus";
    }
    auto result0 =   system.systemBus().connect(DBUS_SERVICE_NAME,DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                         "DevicePresenceChanged", this,
                                                         SLOT(handleDevicePresenceChanged(uint,uint,uint,QString,QMap<QString,QString>)));
    auto result1 =   system.systemBus().connect(DBUS_SERVICE_NAME,DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                         "DevicePolicyChanged", this,
                                                         SLOT(handleDevicePolicyChanged(uint,uint,uint,QString,uint,QMap<QString,QString>)));
    if(!result0)
    {
        qDebug() << "connection0 failed";
        qPrintable(QDBusConnection::systemBus().lastError().message());
    }
    if(!result1)
    {
        qDebug() << "connection1 failed";
        qPrintable(QDBusConnection::systemBus().lastError().message());
    }

    listDevices("allow");
    listDevices("block");

    connect(m_uplinkHandler.get(), &UplinkHandler::appendServiceRule, this, &ServiceHandler::appendRule);
}

ServiceHandler::~ServiceHandler()
{

}

void ServiceHandler::listRules(QString label)
{
    QDBusMessage ruleset = m_ifaceUSBGuardPolicy->call("listRules",label);

    qDebug() << ruleset; //a(us)

}

void ServiceHandler::appendRule(QString device_id,QString device_serial, uint target, QString interface)
{
    uint parent_id = 1;
    bool temporary = 0;
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

void ServiceHandler::removeRule(uint id)
{

    m_ifaceUSBGuardDevices->call("removeRule",id);
}

void ServiceHandler::listDevices(QString query)
{
    QDBusMessage devices = m_ifaceUSBGuardDevices->call("listDevices",query);
   // qDebug() << devices.arguments().at(0)[0]; //a(us)
   // for( auto e : devices.arguments())


}

///SLOTS

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void ServiceHandler::handleDevicePresenceChanged(uint id, uint event, uint target,
                                                 QString device_rule, QMap<QString, QString>  attributes)
{
  //  qDebug() << "presence";
 //   qDebug() << id << " " << event << " " << target << " " << device_rule << " " << attributes;
    Q_UNUSED(id);
    Q_UNUSED(device_rule);
    m_uplinkHandler->devicePresenceUpload(attributes["id"], attributes["serial"], target, attributes["with-interface"], event);
}

void ServiceHandler::handleDevicePolicyChanged(uint id, uint target_old, uint target_new,
                                               QString device_rule, uint rule_id, QMap<QString, QString> attributes)
{
    Q_UNUSED(id);
    Q_UNUSED(device_rule);
    Q_UNUSED(rule_id);
    Q_UNUSED(target_old);
  //  qDebug() << "policy";
   // qDebug() << id << " " << target_old << " " << target_new << " " << device_rule << rule_id << " " << attributes;
    m_uplinkHandler->devicePolicyUpload(attributes["id"], attributes["serial"], target_new, attributes["with-interface"]);
}

