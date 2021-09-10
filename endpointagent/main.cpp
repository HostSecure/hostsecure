#include <QCoreApplication>
#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <unistd.h>

#include "USBServiceHandler.h"
#include "uplinkhandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    USBServiceHandler sh;
    sh.listRules("");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");

    return a.exec();
}
