#include "databasehandler.h"
#include "databasedatafileparser.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace
{
const QString DEVICE_STATUS_UNKNOWN = "U";
const QString DEVICE_STATUS_WHITELISTED = "W";
const QString DEVICE_STATUS_BLACKLISTED = "B";

//const QString CREATE = "CREATE TABLE ";
//const QString INSERT = "INSERT INTO ";
//const QString PRIMARY = "PRIMARY KEY";
//const QString FOREIGN = "FOREIGN KEY";

//const QString EDGENODE = "edgenode";
//const QString EDGENODEID = "macaddress";
//const QString EDGENODEIDTYPE = "VARCHAR(8)";
//const QString EDGENODEDESCRIPTTION = "nodedescription";
//const QString EDGENODEDESCRIPTIONTYPE = "VARCHAR(50)";

//const QString VENDOR = "vendor";
//const QString VENDORID = "vendorid";
//const QString VENDORIDTYPE = "VARCHAR(4)";
//const QString VENDORNAME = "vendorname";
//const QString VENDORNAMETYPE = "VARCHAR(30)";
}

DatabaseHandler::DatabaseHandler(const QString& databasePath)
{
    //TODO: Handle differently, e.g. by using a SELECT call on some table to make it support other db types (e.g. MariaDB/MySQL)
    bool exists = QFile::exists(databasePath);
    if(!exists)
    {
        QDir dir = QFileInfo(databasePath).absoluteDir();
        if(!dir.exists())
        {
            if(!dir.mkpath(dir.absolutePath()))
            {
                qCritical() << "Failed to create database path: " << dir.absolutePath();
                return;
            }
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if(!db.open())
    {
        qFatal("Failed to open database: %s", db.lastError().text().toStdString().c_str());
    }
    else
    {
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");

        if(!exists)
        {
            if(!query.exec("CREATE TABLE edgenode(macaddress VARCHAR(8) PRIMARY KEY, isonline BIT NOT NULL, lastheartbeat TIMESTAMP NOT NULL)"))
            {
                qFatal("Failed to create edgenode table: %s", query.lastError().text().toStdString().c_str());
            }

            if(!query.exec("CREATE TABLE virushash(hashkey VARCHAR(32) PRIMARY KEY, description VARCHAR(100))"))
            {
                qFatal("Failed to create virushash table: %s", query.lastError().text().toStdString().c_str());
            }

            if(!query.exec("CREATE TABLE productvendor(productid VARCHAR(4), vendorid VARCHAR(4), productname VARCHAR(30), vendorname VARCHAR(30), "
                                                      "PRIMARY KEY(productid, vendorid))"))
            {
                qFatal("Failed to create productvendor table: %s", query.lastError().text().toStdString().c_str());
            }

            if(!query.exec("CREATE TABLE device(id INTEGER PRIMARY KEY AUTOINCREMENT, productid VARCHAR(4), vendorid VARCHAR(4), serialnumber VARCHAR(8), status CHAR(1) NOT NULL, "
                       "UNIQUE(productid, vendorid, serialnumber), "
                       "FOREIGN KEY(productid, vendorid) REFERENCES productvendor(productid, vendorid))"))
            {
                qFatal("Failed to create device table: %s", query.lastError().text().toStdString().c_str());
            }

            if(!query.exec("CREATE TABLE connecteddevice(edgenodemacaddress VARCHAR(8), deviceid INTEGER, connecttime TIMESTAMP, "
                           "FOREIGN KEY (edgenodemacaddress) REFERENCES edgenode(macaddress), "
                           "FOREIGN KEY (deviceid) REFERENCES device(id), "
                           "PRIMARY KEY(edgenodemacaddress, deviceid))"))
            {
                qFatal("Failed to create connecteddevice table: %s", query.lastError().text().toStdString().c_str());
            }

            if(!query.exec("CREATE TABLE log(edgenodemacaddress VARCHAR(8), "
                       "deviceid INTEGER, "
                       "logtime TIMESTAMP, "
                       "loginfo VARCHAR(100), "
                       "FOREIGN KEY (edgenodemacaddress) REFERENCES edgenode(macaddress), "
                       "FOREIGN KEY (deviceid) REFERENCES device(id), "
                       "PRIMARY KEY(edgenodemacaddress, deviceid, logtime))"))
            {
                qFatal("Failed to create log table: %s", query.lastError().text().toStdString().c_str());
            }

            DatabaseDataFileParser::parseDeviceProductVendor(*this);
            DatabaseDataFileParser::parseVirusHash(*this);
        }
    }
}

void DatabaseHandler::registerEdgeNode(const QString &macaddress, bool isOnline, const QString& lastHeartbeatTimestamp) const
{
    QSqlQuery query;
    // NOT an UPSERT, but it's ok as we update every attribute of the table and we don't use an auto id.
    query.prepare("INSERT OR REPLACE INTO edgenode(macaddress, isonline, lastheartbeat)"
                  "VALUES(?, ?, ?)");
    query.bindValue(0, macaddress);
    query.bindValue(1, (isOnline ? 1 : 0));
    query.bindValue(2, lastHeartbeatTimestamp);

    if(!query.exec())
    {
        qWarning() << __PRETTY_FUNCTION__ << "Failed to register edge node: " << query.lastError();
        throw std::runtime_error("Failed to register edge node");
    }
}

bool DatabaseHandler::getEdgeNode(EdgeNode &edgeNode, const QString &macAddress) const
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT * FROM edgenode WHERE macaddress = ?");
    query.bindValue(0, macAddress);

    if(query.exec())
    {
        if(query.next())
        {
            edgeNode.macAddress = query.value(0).toString();
            edgeNode.isOnline = query.value(1).toBool();
            edgeNode.lastHeartbeat = query.value(2).toString();
            success = true;
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get edge node: " << query.lastError();
        throw std::runtime_error("Failed to get edge node");
    }

    return success;
}

void DatabaseHandler::getAllEdgeNodeKeys(QVector<QString> &macAddresses) const
{
    try
    {
        getKeysFromTable("macaddress", "edgenode",  macAddresses);
    }
    catch(std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e;
    }
}

void DatabaseHandler::getAllEdgeNodes(std::vector<std::unique_ptr<EdgeNode> > &edgeNodes) const
{
    QSqlQuery query;
    if(query.exec("SELECT * FROM edgenode"))
    {
        while(query.next())
        {
            std::unique_ptr<EdgeNode> node =  std::make_unique<EdgeNode>();
            node->macAddress = query.value(0).toString();
            node->isOnline = query.value(1).toBool();
            node->lastHeartbeat = query.value(2).toString();
            edgeNodes.push_back(std::move(node));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all edge nodes: " << query.lastError();
        throw std::runtime_error("Failed to get all edge nodes");
    }
}

void DatabaseHandler::setEdgeNodeOnlineStatus(const QString &macAddress, bool isOnline, const QString &lastHeartbeatTimestamp)
{
    QSqlQuery query;
    query.prepare("UPDATE edgenode SET isonline = ?, lastheartbeat = ? WHERE macAddress = ?");
    query.bindValue(0, (isOnline ? 1 : 0));
    query.bindValue(1, lastHeartbeatTimestamp);
    query.bindValue(2, macAddress);

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to set edge node online status: " << query.lastError();
        throw std::runtime_error("Failed to set edge node online status");
    }
}

void DatabaseHandler::getOnlineEdgeNodes(QVector<QString> &macAddresses) const
{
    QSqlQuery query;

    if(query.exec("SELECT macaddress FROM edgenode WHERE isonline = 1"))
    {
        while (query.next())
        {
           macAddresses.push_back( query.value(0).toString());
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to set edge node online status: " << query.lastError();
        throw std::runtime_error("Failed to set edge node online status");
    }
}

void DatabaseHandler::registerDevice(const QString &productId, const QString &vendorId, const QString &serialNumber ) const
{
    QSqlQuery query;

    // Ignore if it exists
    query.prepare("INSERT INTO device(productid, vendorid, serialnumber, status) "
                  "SELECT ?, ?, ?, ? "
                  "WHERE NOT EXISTS(SELECT * "
                                   "FROM device "
                                   "WHERE productid = ? AND vendorid = ? AND serialnumber = ?)");
    query.bindValue(0, productId);
    query.bindValue(1, vendorId);
    query.bindValue(2, serialNumber);
    query.bindValue(3, DEVICE_STATUS_UNKNOWN);
    query.bindValue(4, productId);
    query.bindValue(5, vendorId);
    query.bindValue(6, serialNumber);

    if(!query.exec())
    {
        qWarning() << __PRETTY_FUNCTION__ << "Failed to register device: " << query.lastError();
        throw std::runtime_error("Failed to register device");
    }
}

bool DatabaseHandler::getDevice(Device &device, const QString &productId, const QString &vendorId, const QString &serialNumber) const
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT productid, vendorid, serialnumber FROM device WHERE productid = ? AND vendorid = ? AND serialnumber = ?");
    query.bindValue(0, productId);
    query.bindValue(1, vendorId);
    query.bindValue(2, serialNumber);

    if(query.exec())
    {
        if(query.next())
        {
            device.productId = query.value(0).toString();
            device.vendorId = query.value(1).toString();
            device.serialNumber = query.value(2).toString();
            success = true;
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get device: " << query.lastError();
        throw std::runtime_error("Failed to get device");
    }

    return success;
}

void DatabaseHandler::getAllDevices(std::vector<std::unique_ptr<Device> > &devices) const
{
    QSqlQuery query;
    if(query.exec("SELECT productid, vendorid, serialnumber FROM device"))
    {
        while(query.next())
        {
            std::unique_ptr<Device> device =  std::make_unique<Device>();
            device->productId = query.value(0).toString();
            device->vendorId = query.value(1).toString();
            device->serialNumber = query.value(2).toString();
            devices.push_back(std::move(device));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all devices: " << query.lastError();
        throw std::runtime_error("Failed to get all devices");
    }
}

void DatabaseHandler::setDeviceBlacklisted(const QString& productId, const QString& vendorId, const QString& serialNumber) const
{
    try
    {
        setDeviceStatus(productId, vendorId, serialNumber, DEVICE_STATUS_BLACKLISTED);
    }
    catch (std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e; // Forward the exception so it can be handled correctly by the caller";
    }
}

bool DatabaseHandler::isDeviceBlackListed(const QString& productId, const QString& vendorId, const QString& serialNumber) const
{
    bool retVal = true; // Assume the worst
    try
    {
        retVal = checkDeviceStatus(productId, vendorId, serialNumber, DEVICE_STATUS_BLACKLISTED);
    }
    catch(std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e; // Forward the exception so it can be handled correctly by the caller";
    }

    return retVal;
}

void DatabaseHandler::setDeviceWhitelisted(const QString& productId, const QString& vendorId, const QString& serialNumber) const
{
    try
    {
        setDeviceStatus(productId, vendorId, serialNumber, DEVICE_STATUS_WHITELISTED);
    }
    catch (std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e; // Forward the exception so it can be handled correctly by the caller";
    }
}

bool DatabaseHandler::isDeviceWhiteListed(const QString& productId, const QString& vendorId, const QString& serialNumber) const
{
    bool retVal = false;
    try
    {
        retVal = checkDeviceStatus(productId, vendorId, serialNumber, DEVICE_STATUS_WHITELISTED);
    }
    catch(std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e; // Forward the exception so it can be handled correctly by the caller";
    }

    return retVal;
}

void DatabaseHandler::registerConnectedDevice(const QString &edgeNodeMacAddress, const QString &deviceProductId, const QString &deviceVendorId, const QString &deviceSerialNumber, const QString &timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO connecteddevice(edgenodemacaddress, deviceid, connecttime) "
                  "SELECT ?, device.id, ? "
                  "FROM device "
                  "WHERE device.productid = ? AND device.vendorid = ? AND device.serialnumber = ?");
    query.bindValue(0, edgeNodeMacAddress);
    query.bindValue(1, timestamp);
    query.bindValue(2, deviceProductId);
    query.bindValue(3, deviceVendorId);
    query.bindValue(4, deviceSerialNumber);

//    "INSERT INTO log(edgenodemacaddress, deviceid, logtime, loginfo) "
//                      "SELECT ?, device.id, ?, ? "
//                      "FROM device "
//                      "WHERE device.productid = ? AND device.vendorid = ? AND device.serialnumber = ?"

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to register connected device: " << query.lastError();
        throw std::runtime_error("Failed to register connected device");
    }
}

void DatabaseHandler::unregisterConnectedDevice(const QString &edgeNodeMacAddress, const QString &deviceProductId, const QString &deviceVendorId, const QString &deviceSerialNumber)
{
    QSqlQuery query;
    query.prepare("DELETE FROM connecteddevice "
                  "WHERE edgenodemacaddress = ? AND deviceid = (SELECT id FROM device WHERE productid = ? AND vendorid = ? AND serialnumber = ?)");
    query.bindValue(0, edgeNodeMacAddress);
    query.bindValue(1, deviceProductId);
    query.bindValue(2, deviceVendorId);
    query.bindValue(3, deviceSerialNumber);

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to unregister connected device: " << query.lastError();
        throw std::runtime_error("Failed to unregister connected device");
    }
}

void DatabaseHandler::getAllConnectedDevices(std::vector<std::unique_ptr<ConnectedDevice> > &connectedDevices)
{
    QSqlQuery query;
    if(query.exec("SELECT edgenodemacaddress, productid, vendorid, serialnumber "
                  "FROM connecteddevice "
                  "INNER JOIN device ON device.id = connecteddevice.deviceid"))
    {
        while(query.next())
        {
            std::unique_ptr<ConnectedDevice> connectedDevice =  std::make_unique<ConnectedDevice>();
            connectedDevice->connectedEdgeNodeMacAddress = query.value(0).toString();
            connectedDevice->deviceProductId = query.value(1).toString();
            connectedDevice->deviceVendorId = query.value(2).toString();
            connectedDevice->deviceSerialNumber = query.value(3).toString();
            connectedDevices.push_back(std::move(connectedDevice));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all connected devices: " << query.lastError();
        throw std::runtime_error("Failed to get all connected devices");
    }
}

void DatabaseHandler::registerProductVendor(const QString &productId, const QString &productName, const QString &vendorId, const QString &vendorName)
{
    QSqlQuery query;
    query.prepare("INSERT INTO productvendor(productid, vendorid, productname, vendorname)"
                  "VALUES(?, ?, ?, ?)");
    query.bindValue(0, productId);
    query.bindValue(1, vendorId);
    query.bindValue(2, productName);
    query.bindValue(3, vendorName);

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to register productvendor: " << query.lastError();
        throw std::runtime_error("Failed to register productvendor");
    }
}

bool DatabaseHandler::getProductVendor(ProductVendor& productVendor, const QString &productId, const QString vendorId)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT * FROM productvendor WHERE productid = ? AND vendorid = ?");
    query.bindValue(0, productId);
    query.bindValue(1, vendorId);

    if(query.exec())
    {
        if(query.next())
        {
            productVendor.productId = query.value(0).toString();
            productVendor.vendorId = query.value(1).toString();
            productVendor.productName = query.value(2).toString();
            productVendor.vendorName = query.value(3).toString();
            success = true;
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get productvendor: " << query.lastError();
        throw std::runtime_error("Failed to get productvendor");
    }

    return success;
}

void DatabaseHandler::getAllProductVendors(std::vector<std::unique_ptr<ProductVendor> >& productVendors)
{
    QSqlQuery query;
    if(query.exec("SELECT * FROM productvendor"))
    {
        while(query.next())
        {
            std::unique_ptr<ProductVendor> productVendor =  std::make_unique<ProductVendor>();
            productVendor->productId = query.value(0).toString();
            productVendor->vendorId = query.value(1).toString();
            productVendor->productName = query.value(2).toString();
            productVendor->vendorName = query.value(3).toString();
            productVendors.push_back(std::move(productVendor));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all productvendors: " << query.lastError();
        throw std::runtime_error("Failed to get all productvendors");
    }
}

void DatabaseHandler::registerVirusHash(const QString &virusHash, const QString &description)
{
    QSqlQuery query;
    query.prepare("INSERT INTO virushash(hashkey, description)"
                  "VALUES(?, ?)");
    query.bindValue(0, virusHash);
    query.bindValue(1, description);

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to register virus hash: " << query.lastError();
        throw std::runtime_error("Failed to register virus hash");
    }
}

bool DatabaseHandler::getVirusHash(VirusHash &vHash, const QString &virusHash) const
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT * FROM virushash WHERE hashkey = ?");
    query.bindValue(0, virusHash);

    if(query.exec())
    {
        if(query.next())
        {
            vHash.virusHash = query.value(0).toString();
            vHash.description = query.value(1).toString();
            success = true;
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get virus hash: " << query.lastError();
        throw std::runtime_error("Failed to get virus hash");
    }

    return success;
}

void DatabaseHandler::getAllVirusHashKeys(QVector<QString> &virusHashes) const
{
    try
    {
        getKeysFromTable("hashkey", "virushash",  virusHashes);
    }
    catch(std::exception& e)
    {
        qCritical() <<  __PRETTY_FUNCTION__ << e.what();
        throw e;
    }
}

void DatabaseHandler::getAllVirusHashes(std::vector<std::unique_ptr<VirusHash> > &virusHashes) const
{
    QSqlQuery query;
    if(query.exec("SELECT * FROM virushash"))
    {
        while(query.next())
        {
            std::unique_ptr<VirusHash> virusHash =  std::make_unique<VirusHash>();
            virusHash->virusHash = query.value(0).toString();
            virusHash->description = query.value(1).toString();
            virusHashes.push_back(std::move(virusHash));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all virus hashes: " << query.lastError();
        throw std::runtime_error("Failed to get all virus hashes");
    }
}

bool DatabaseHandler::isHashInVirusDatabase(const QString &hash) const
{
    bool found = true; // Assume the worst
    QSqlQuery query;
    query.prepare("SELECT * FROM virushash WHERE hashkey = ?");
    query.bindValue(0, hash);

    if(query.exec())
    {
        if(!query.next())
        {
            found = false;
        }
    }
    else
    {
       qCritical() <<  __PRETTY_FUNCTION__ << "Failed to check virus hash: " << query.lastError();
       throw std::runtime_error("Failed to check virus hash");
    }

    return found;
}

void DatabaseHandler::logEvent(const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber, const QString& timestamp, const QString& eventDescription)
{
    QSqlQuery query;
    query.prepare("INSERT INTO log(edgenodemacaddress, deviceid, logtime, loginfo) "
                  "SELECT ?, device.id, ?, ? "
                  "FROM device "
                  "WHERE device.productid = ? AND device.vendorid = ? AND device.serialnumber = ?");
    query.bindValue(0, edgeNodeMacAddress);
    query.bindValue(1, timestamp);
    query.bindValue(2, eventDescription);
    query.bindValue(3, deviceProductId);
    query.bindValue(4, deviceVendorId);
    query.bindValue(5, deviceSerialNumber);

    if(!query.exec())
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to log event: " << query.lastError();
        throw std::runtime_error("Failed to log event");
    }
}

bool DatabaseHandler::getLoggedEvent(LogEvent& logEvent, const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber, const QString& timestamp) const
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT edgenodemacaddress, productid, vendorid, serialnumber, logtime, loginfo "
                  "FROM log "
                  "INNER JOIN device ON device.id = log.deviceid "
                  "WHERE edgenodemacaddress = ? "
                    "AND deviceid = (SELECT id "
                                    "FROM device "
                                    "WHERE productid = ? AND vendorid = ? AND serialnumber = ?) "
                    "AND logtime = ? ");
    query.bindValue(0, edgeNodeMacAddress);
    query.bindValue(1, deviceProductId);
    query.bindValue(2, deviceVendorId);
    query.bindValue(3, deviceSerialNumber);
    query.bindValue(4, timestamp);

    if(query.exec())
    {
        if(query.next())
        {
            logEvent.edgeNodeMacAddress = query.value(0).toString();
            logEvent.deviceProductId = query.value(1).toString();
            logEvent.deviceVendorId = query.value(2).toString();
            logEvent.deviceSerialNumber = query.value(3).toString();
            logEvent.timestamp = query.value(4).toString();
            logEvent.eventDescription = query.value(5).toString();
            success = true;
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get log event: " << query.lastError();
        throw std::runtime_error("Failed to get log event");
    }

    return success;
}

void DatabaseHandler::getAllLoggedEvents(std::vector<std::unique_ptr<LogEvent> >& loggedEvents) const
{
    QSqlQuery query;
    if(query.exec("SELECT edgenodemacaddress, productid, vendorid, serialnumber, logtime, loginfo "
                  "FROM log "
                  "INNER JOIN device ON device.id = log.deviceid"))
    {
        while(query.next())
        {
            std::unique_ptr<LogEvent> logEvent =  std::make_unique<LogEvent>();
            logEvent->edgeNodeMacAddress = query.value(0).toString();
            logEvent->deviceProductId = query.value(1).toString();
            logEvent->deviceVendorId = query.value(2).toString();
            logEvent->deviceSerialNumber = query.value(3).toString();
            logEvent->timestamp = query.value(4).toString();
            logEvent->eventDescription = query.value(5).toString();
            loggedEvents.push_back(std::move(logEvent));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Failed to get all logged events: " << query.lastError();
        throw std::runtime_error("Failed to get all logged events");
    }
}

void DatabaseHandler::getKeysFromTable(const QString keyName, const QString &tableName, QVector<QString> &result) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT %1 FROM %2").arg(keyName, tableName));

    if(query.exec())
    {
        while (query.next())
        {
           result.push_back( query.value(0).toString());
        }
    }
    else
    {
       throw std::runtime_error("Failed to get keys from table: " + query.lastError().text().toStdString());
    }
}

void DatabaseHandler::setDeviceStatus(const QString& productId, const QString& vendorId, const QString& serialNumber, const QString& status) const
{
    QSqlQuery query;
    query.prepare("UPDATE device SET status = ? WHERE productid = ? AND vendorid = ? AND serialnumber = ?");
    query.bindValue(0, status);
    query.bindValue(1, productId);
    query.bindValue(2, vendorId);
    query.bindValue(3, serialNumber);

    if(!query.exec())
    {
        throw std::runtime_error("Failed to set device status: " + query.lastError().text().toStdString());
    }
}

bool DatabaseHandler::checkDeviceStatus(const QString& productId, const QString& vendorId, const QString& serialNumber, const QString& status) const
{
    bool retVal = false;
    QSqlQuery query;
    query.prepare("SELECT * FROM device WHERE productId = ? AND vendorid = ? AND serialnumber = ? AND status = ?");
    query.bindValue(0, productId);
    query.bindValue(1, vendorId);
    query.bindValue(2, serialNumber);
    query.bindValue(3, status);

    if(query.exec())
    {
        if(query.next())
        {
            retVal = true;
        }
    }
    else
    {
        throw std::runtime_error("Failed to check device status: " + query.lastError().text().toStdString());
    }

    return retVal;
}
