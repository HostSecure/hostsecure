#include "DatabaseHandler.h"

MessageHandler::Gateway::DatabaseHandler::DatabaseHandler(QString& _address, quint16& _port) : MqttInterface(_address, _port)
{
    /* Append all topics to vector in parent class */
    this->topics->append("/edges/new_edge");
}

void MessageHandler::Gateway::DatabaseHandler::new_message(QMqttMessage _payload)
{
    /* Forward payload to virtual method of base class */
    MessageHandler::MqttInterface::new_message(_payload);
    qInfo() << "Virtual hello from DatabaseHandler";
}

void MessageHandler::Gateway::DatabaseHandler::heartbeat_received()
{

}
