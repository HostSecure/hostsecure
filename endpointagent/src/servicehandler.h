#pragma once

#include <QObject>
#include <QtDBus>
#include <QMap>
#include <QString>

class UplinkHandler;
class ServiceHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServiceHandler(QObject *parent = nullptr);
    ~ServiceHandler();

    void listRules(const QString& label); // out a(us) ruleset
    void removeRule(const uint id); // out
    void listDevices(const QString& query); // out a(us) devices

signals:
    void devicePresenceUpdate(QString device_id, QString device_serial,
                             uint target, QString interface, uint event);

    void devicePolicyUpdate(QString device_id, QString device_serial,
                             uint target, QString interface);

public slots:
    void handleDevicePresenceChanged(const uint id, const uint event, const uint target, const QString& device_rule,
                               const QMap<QString, QString>& attributes);
    void handleDevicePolicyChanged(const uint id, const uint target_old, const uint target_new, const QString& device_rule, const uint rule_id,
                             const QMap<QString, QString>& attributes);
    void appendRule(const QString& device_id, const QString& device_serial, const uint target, const QString& interface);

private:
    QDBusInterface *m_ifaceUSBGuardDevices;
    QDBusInterface *m_ifaceUSBGuardPolicy;
    std::unique_ptr<UplinkHandler> m_uplinkHandler;
};

