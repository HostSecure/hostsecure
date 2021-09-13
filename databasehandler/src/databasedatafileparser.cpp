#include "databasedatafileparser.h"
#include "databasehandler.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>


// The code is based on parsing csv files to make it easy to create your own config files and add your own data
// In reality, the parser wouldd be configured to parse data from predefined sources, which is most likely not in the csv format.

DatabaseDataFileParser::DatabaseDataFileParser()
{

}

// A proper database to look into can be found here: http://www.linux-usb.org/usb-ids.html
void DatabaseDataFileParser::parseDeviceProductVendor(DatabaseHandler &dbHandler)
{
    const char* dataDir = getenv("HOSTSECURE_DATA_DIR");
    if(dataDir == nullptr)
    {
        dataDir = ".";
    }

    QString path(dataDir);
    path.append("/productvendors.txt");
    QFile file(path);

    if(file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                QStringList productVendor = line.split(',');
                if(productVendor.size() != 4)
                {
                    qWarning() << "Read incomplete line in productvendor file: " << line;
                }
                else
                {
                    dbHandler.registerProductVendor(productVendor[0], productVendor[1], productVendor[2], productVendor[3]);
                }
            }
            file.close();
        }
        else
        {
            qCritical() << "Failed to open file " << QFileInfo(file).absoluteFilePath() << " while adding productvendor data: " << file.errorString();
        }
    }
    else
    {
        qCritical() << "Could not find a productvendor file. Expected to find one here: " << QFileInfo(file).absoluteFilePath();
    }
}

void DatabaseDataFileParser::parseVirusHash(DatabaseHandler &dbHandler)
{
    const char* dataDir = getenv("HOSTSECURE_DATA_DIR");
    if(dataDir == nullptr)
    {
        dataDir = ".";
    }

    QString path(dataDir);
    path.append("/virushashes.txt");

    QFile file(path);
    if(file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                QStringList virusHash = line.split(',');
                if(virusHash.size() != 2)
                {
                    qWarning() << "Read incomplete line in virushash file: " << line;
                }
                else
                {
                    dbHandler.registerVirusHash(virusHash[0], virusHash[1]);
                }
            }
            file.close();
        }
        else
        {
            qCritical() << "Failed to open file " << QFileInfo(file).absoluteFilePath() << " while adding virushash data: " << file.errorString();
        }
        }
    else
    {
        qCritical() << "Could not find a virushash file. Expected to find one here: " << QFileInfo(file).absoluteFilePath();
    }
}
