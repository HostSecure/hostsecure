#include <QCoreApplication>
#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <unistd.h>

#include "USBServiceHandler.h"

namespace
{
    int ARG_LIMIT = 255;
    int ARGC_LIMIT = 10;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    USBServiceHandler sh;
    sh.listRules("");
    //sh.appendRule("allow id *:* with-interface 03:00:00",1,0);
    //sh.listDevices("block");

    std::string exec_name = argv[0];
    std::string input_arg;

    return a.exec();
}
