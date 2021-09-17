#pragma once
#include <QObject>
#include <QtDBus>
#include <QMap>
#include <QString>
#include "uplinkhandler.h"
//!
//! \brief The ServiceHandler class
//! Mediates communication between local services
//! And up to cloud, manages events from/to local services
//!

class ServiceHandler : public QObject
{
   Q_OBJECT
public:
   explicit ServiceHandler(QObject* a_parent = nullptr);

   void listRules(const QString& a_label); // out a(us) ruleset
   void removeRule(const uint a_id); // out
   void listDevices(const QString& a_query); // out a(us) devices

signals:
   void devicePresenceUpdate(QString device_id, QString device_serial,
                             uint target, QString interface, uint event);

   void devicePolicyUpdate(QString device_id, QString device_serial,
                           uint target, QString interface);

public slots:
   void handleDevicePresenceChanged(const uint a_id, const uint a_event, const uint a_target, const QString& a_device_rule,
                                    const QMap<QString, QString>& a_attributes);
   void handleDevicePolicyChanged(const uint a_id, const uint a_target_old, const uint a_target_new, const QString& a_device_rule, const uint a_rule_id,
                                  const QMap<QString, QString>& a_attributes);
   void appendRule(const QString& a_device_id, const QString& a_device_serial, const uint a_target, const QString& a_interface);

private:
   QDBusInterface* m_ifaceUSBGuardDevices { nullptr };
   QDBusInterface* m_ifaceUSBGuardPolicy { nullptr };
   std::unique_ptr<UplinkHandler> m_uplinkHandler;
};

