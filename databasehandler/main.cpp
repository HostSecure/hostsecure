#include <QCoreApplication>
#include <QDir>

#include "loghandler.h"
#include "databasehandler.h"
#include "testhandler.h"

int main(int argc, char *argv[])
{
    bool test = true;
    LogHandler logger;

    QCoreApplication a(argc, argv);

    // TODO: Nobody likes hardcoded paths
    if(test)
    {
        TestHandler testHandler("testcases.db");
        testHandler.testCaseAll();
    }
    else
    {
        DatabaseHandler dbHandler("test.db");
    }

    return a.exec();
}
