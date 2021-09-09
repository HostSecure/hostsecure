#include "USBServiceHandler.h"
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusArgument>
#include <QDBusMetaType>

USBServiceHandler::USBServiceHandler(QObject *parent) : QObject(parent)
{

    qDBusRegisterMetaType<QMap<QString,QString>>();

    ifaceUSBGuardDevices = new QDBusInterface(DBUS_SERVICE_NAME, DBUS_DEVICES_PATH, DBUS_DEVICES_INTERFACE,
                                                  QDBusConnection::systemBus(), this);
    ifaceUSBGuardPolicy = new QDBusInterface(DBUS_SERVICE_NAME, DBUS_POLICY_PATH, DBUS_POLICY_INTERFACE,
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

}

void USBServiceHandler::listRules(QString label)
{
    QDBusMessage ruleset = ifaceUSBGuardPolicy->call("listRules",label);

    qDebug() << ruleset; //a(us)

}

void USBServiceHandler::appendRule(QString rule, uint parent_id, bool temporary)
{

    QDBusMessage id = ifaceUSBGuardPolicy->call("appendRule",rule,parent_id,temporary);
    qDebug() << id;
}

void USBServiceHandler::removeRule(uint id)
{

    ifaceUSBGuardDevices->call("removeRule",id);
}

void USBServiceHandler::listDevices(QString query)
{
    QDBusMessage devices = ifaceUSBGuardDevices->call("listDevices",query);
    qDebug() << devices; //a(us)

}

///SLOTS

/// Target 0 = allow, 1 = block, 2 = reject, 3 = match, 4 = unknown, 5 = device, 6 = empty, 7 = invalid
/// Event  0 = alreadyPresent, 1 = Insert, 2 = Update, 3 = Remove
void USBServiceHandler::handleDevicePresenceChanged(uint id, uint event, uint target, QString device_rule, QMap<QString, QString>  attributes)
{
    qDebug() << "presence";
    qDebug() << id << " " << event << " " << target << " " << device_rule << " " << attributes;

}

void USBServiceHandler::handleDevicePolicyChanged(uint id, uint target_old, uint target_new, QString device_rule, uint rule_id, QMap<QString, QString> attributes)
{
    qDebug() << "policy";
    qDebug() << id << " " << target_old << " " << target_new << " " << device_rule << rule_id << " " << attributes;

}

