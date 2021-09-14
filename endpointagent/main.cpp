#include <QCoreApplication>
#include <QObject>
#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <unistd.h>

#include "servicehandler.h"
#include "uplinkhandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Configure broker (server) */
//    QString broker_addr = "test.mosquitto.org";
//    quint16 broker_port = 1883;

    ServiceHandler sh;
    //sh.listDevices("allow");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");

    return a.exec();
}
