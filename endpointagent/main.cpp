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

    ServiceHandler sh;
    //sh.listDevices("allow");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");
    UplinkHandler uh;

    QObject::connect(&sh, &ServiceHandler::devicePresenceUpdate,
            &uh, &UplinkHandler::devicePresenceUpload);
    QObject::connect(&sh, &ServiceHandler::devicePolicyUpdate,
            &uh, &UplinkHandler::devicePolicyUpload);

    //std::cout << uh.getMacAddress().toStdString();

    return a.exec();
}
