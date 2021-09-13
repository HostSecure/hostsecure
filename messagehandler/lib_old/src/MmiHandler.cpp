#include "MmiHandler.h"

MessageHandler::Gateway::MmiHandler::MmiHandler(QString& _address, quint16& _port) : MqttInterface(_address, _port)
{

}

void MessageHandler::Gateway::MmiHandler::heartbeat_received()
{

}
