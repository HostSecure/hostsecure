#include <QCoreApplication>

#include "servicehandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServiceHandler sh;
    //sh.listDevices("allow");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");

    return a.exec();
}
