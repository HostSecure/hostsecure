#include "MqttInterface.h"

MessageHandler::MqttInterface::MqttInterface(QString& _address, quint16& _port, QObject* parent) : QObject(parent)
{
    /* Initialize shared pointers */
    broker = QSharedPointer<MessageHandler::Broker>(new MessageHandler::Broker);
    mqtt_client = QSharedPointer<QMqttClient>(new QMqttClient);
    topics = QSharedPointer<QVector<QString>>(new QVector<QString>);

    if (!broker.isNull())
    {
        /* Set broker configuration */
        broker->address = _address;
        broker->port = _port;
        broker->heartbeat = 60;
        broker->qos = 0;
    }
    else qCritical() << "Broker is not initialized properly!";

    if (!mqtt_client.isNull())
    {
        /* Communication bindings */
        QObject::connect(mqtt_client.get(), &QMqttClient::connected, this, &MessageHandler::MqttInterface::broker_connected);
        QObject::connect(mqtt_client.get(), &QMqttClient::disconnected, this, &MessageHandler::MqttInterface::broker_disconnected);
        QObject::connect(mqtt_client.get(), &QMqttClient::stateChanged, this, &MessageHandler::MqttInterface::state_changed);
        QObject::connect(mqtt_client.get(), &QMqttClient::errorChanged, this, &MessageHandler::MqttInterface::client_error);

        qDebug() << "MQTT protocol version:" << mqtt_client->protocolVersion();

        /* Connect to MQTT broker */
        this->connect();
    }
    else qCritical() << "MqttClient is not initialized properly!";
}

void MessageHandler::MqttInterface::connect()
{
    mqtt_client->setHostname(broker->address);
    mqtt_client->setPort(broker->port);

    if (get_state() != QMqttClient::ClientState::Connected)
    {
        qInfo() << "Connecting to" << broker->address;
        mqtt_client->connectToHost();
    }
    else qInfo() << "Already connected to broker";
}

void MessageHandler::MqttInterface::publish(QString& topic, QJsonObject& payload)
{
    QMqttTopicFilter topic_filter;
    topic_filter.setFilter(topic);

    QJsonDocument data(payload);
    mqtt_client->publish(topic, data.toJson());

    qDebug() << "Published a message to" << topic;
}

void MessageHandler::MqttInterface::subscribe(QString& topic)
{
    QMqttTopicFilter topic_filter;
    topic_filter.setFilter(topic);

    QMqttSubscription* subscription_instance = mqtt_client->subscribe(topic_filter, broker->qos);

    if (subscription_instance)
    {
        QObject::connect(subscription_instance, &QMqttSubscription::messageReceived, this, &MessageHandler::MqttInterface::new_message);
        qDebug() << "Subscribed to topic" << topic;
    }
    else qDebug() << "Failed to subscribe to topic" << topic;
}

/*
void Mqtt::Handler::publish()
{
    QString topic_name = "sira/halla";

    if (is_connected())
    {
        if (mqtt_client->publish(topic_name, "Heisann", true) == -1)
        {
            qInfo() << "Could not publish message";
        }
    }
    else qInfo() << "Not connected to broker!";


    auto subscribe = m_client->subscribe(topic_name);
    if (!subscribe)
    {
        qInfo() << "Could not subscribe. Is there a valid connection?";
        qInfo() << "Error:" << m_client->error();
    }

}
*/

QMqttClient::ClientState MessageHandler::MqttInterface::get_state() const
{
    return mqtt_client->state();
}

bool MessageHandler::MqttInterface::is_connected() const
{
    return (mqtt_client->state() == QMqttClient::ClientState::Connected);
}

void MessageHandler::MqttInterface::broker_connected()
{
    qInfo() << "Connected to" << broker->address << "on port" << broker->port;

    /* Subscribe to all pre-defined topics */
    for (auto& topic : *topics)
    {
        this->subscribe(topic);
    }

    /*
    QString topic_name = "sira/halla";
    QJsonObject jobject;
    jobject["AccelerometerX"] = QString::number(10);
    jobject["AccelerometerY"] = QString::number(11);
    jobject["AccelerometerZ"] = QString::number(12);
    jobject["AccelerometerS"] = QString::number(24);

    this->publish(topic_name, jobject);
    //mqtt_client->publish(QString::fromLatin1("qtds/%1").arg(m_id), doc.toJson());
    */
}

void MessageHandler::MqttInterface::new_message(QMqttMessage _payload)
{
    qDebug() << "Received message from topic" <<  _payload.topic().name() << "- Payload:" << _payload.payload();
}

void MessageHandler::MqttInterface::broker_disconnected()
{
    qInfo() << "Disconnected from" << broker->address;
}

void MessageHandler::MqttInterface::state_changed()
{
    qDebug() << "State changed to" << get_state();
}

void MessageHandler::MqttInterface::client_error()
{
    qInfo() << "Error:" << mqtt_client->error();
}
