#include "EdgeHandler.h"

MessageHandler::Gateway::EdgeHandler::EdgeHandler(QString& _address, quint16& _port) : MqttInterface(_address, _port)
{
    /* Append all topics to vector in parent class */
    this->topics->append("/virushash/#");
}

/**
 * @brief Register a new edge computer
 * @param mac_address
 *
 * This method is called by new edge computers on the network
 */
void MessageHandler::Gateway::EdgeHandler::register_new_edge(QString& mac_address)
{
    QJsonObject payload;
    payload["MACAddress"] = mac_address;
    payload["IsOnline"] = true;
    payload["LastHeartbeat"] = QDateTime::currentDateTime().toString();

    QString topic = "/edges/new_edge";
    this->publish(topic, payload);
}

void MessageHandler::Gateway::EdgeHandler::heartbeat_received()
{

}

void MessageHandler::Gateway::EdgeHandler::new_message(QMqttMessage _payload)
{
    /* Forward payload to virtual method of base class */
    MessageHandler::MqttInterface::new_message(_payload);
    qInfo() << "Virtual hello from EdgeHandler";
}
