#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

class MsgEdge;
class MsgDevice;
class DatabaseHandler;
class DatabaseMqttClient;
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString& databaseName, QObject *parent = nullptr);

signals:

private slots:
    void edgeChanged( const QString& edgeId, const MsgEdge& sample );
    void edgeRemoved( const QString& edgeId );

    void deviceChanged( const QString& edgeId, const QString& deviceId, const MsgDevice& sample );
    void deviceRemoved( const QString& edgeId, const QString& deviceId, const QString& deviceSerial );

private:
    std::shared_ptr<DatabaseHandler> m_DatabaseHandler;
    std::shared_ptr<DatabaseMqttClient> m_MqttCient;
};

#endif // DATABASEMANAGER_H
