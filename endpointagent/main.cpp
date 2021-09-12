#include <QCoreApplication>
#include <QObject>
#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <unistd.h>

#include "servicehandler.h"
#include "uplinkhandler.h"
#include "EdgeHandler.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Configure broker (server) */
    QString broker_addr = "test.mosquitto.org";
    quint16 broker_port = 1883;

    /* Initialize a unique pointer to EdgeHandler */
    QScopedPointer<MessageHandler::Gateway::EdgeHandler> edge_handler = QScopedPointer<MessageHandler::Gateway::EdgeHandler>(
        new MessageHandler::Gateway::EdgeHandler(broker_addr, broker_port)
    );


    ServiceHandler sh;
    //sh.listDevices("allow");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");
    UplinkHandler uh;

    QObject::connect(&sh, &ServiceHandler::devicePresenceUpdate,
            &uh, &UplinkHandler::devicePresenceUpload);
    QObject::connect(&sh, &ServiceHandler::devicePolicyUpdate,
            &uh, &UplinkHandler::devicePolicyUpload);

    QObject::connect(&uh, &UplinkHandler::updateEdgeHandler,
            edge_handler.get(),&MessageHandler::Gateway::EdgeHandler::newDevicePresence);

    //std::cout << uh.getMacAddress().toStdString();

    return a.exec();
}
