#include <QCoreApplication>
#include "servicehandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServiceHandler sh;

    return a.exec();
}
