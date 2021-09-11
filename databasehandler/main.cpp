#include <QCoreApplication>

#include "loghandler.h"
#include "databasehandler.h"
#include "testhandler.h"

int main(int argc, char *argv[])
{
    bool test = true;
    LogHandler logger;

    QCoreApplication a(argc, argv);

    const char* dataDir = getenv("HOSTSECURE_DATA_DIR");
    if(dataDir == nullptr)
    {
        dataDir = ".";
    }

    if(test)
    {
        TestHandler testHandler(QString(dataDir).append("/Databases/testcases.db"));
        testHandler.testCaseAll();
    }
    else
    {
        DatabaseHandler dbHandler(QString(dataDir).append("/Databases/test.db"));
    }

    return a.exec();
}
