#ifndef MQTTINTERFACE_HPP
#define MQTTINTERFACE_HPP

#include <QtMqtt/QtMqtt>
#include <QMqttClient>
#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>

namespace MessageHandler
{
    struct Broker
    {
        QString address;    /* Server hostname */
        quint16 port;       /* Server port */
        quint8 qos;         /* Quality of Service */
        quint8 heartbeat;   /* Ping response (seconds) */
    };

    class MqttInterface : public QObject
    {
        Q_OBJECT

    public:
        explicit MqttInterface(QString&, quint16&, QObject* parent = nullptr);
        void connect();
        void publish(QString&, QJsonObject&);
        void subscribe(QString&);
        bool is_connected() const;
        QMqttClient::ClientState get_state() const;

    protected:
        QSharedPointer<QVector<QString>> topics;

    private:
        QSharedPointer<QMqttClient> mqtt_client;
        QSharedPointer<MessageHandler::Broker> broker;

    public slots:
        virtual void new_message(QMqttMessage);
        void broker_connected();
        void broker_disconnected();
        void state_changed();
        void client_error();
    };
}

#endif // MQTTINTERFACE_HPP
