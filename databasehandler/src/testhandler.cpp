#include "testhandler.h"

#include "databasehandler.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

#include <stdlib.h>

// The test code produces 3 instances of everything necessary.
// This is to make testing more predictable and simpler to code,
// decreasing the complexity and the potential of bugs in the test code

TestHandler::TestHandler(const QString &databasePath)
{
    QFile file(databasePath);
    if(file.exists())
    {
        file.remove();
    }
    m_DBHandler = new DatabaseHandler(databasePath);
}

void TestHandler::testCaseEdgeNode()
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM edgenode");

        DatabaseHandler::EdgeNode node;
        m_DBHandler->registerOrUpdateEdgeNode("ABCD", true, "2021-08-27 09:19:00.000");
        m_DBHandler->registerOrUpdateEdgeNode("EFGH", false, "2011-04-15 17:33:04.372");
        m_DBHandler->registerOrUpdateEdgeNode("IJKL", true, "2016-04-16 07:36:03.987");
        Q_ASSERT(m_DBHandler->getEdgeNode(node, "IJKL"));
        Q_ASSERT(node.lastHeartbeat == "2016-04-16 07:36:03.987");
        m_DBHandler->registerOrUpdateEdgeNode("IJKL", true, "2016-04-16 07:36:03.988");
        Q_ASSERT(m_DBHandler->getEdgeNode(node, "IJKL"));
        Q_ASSERT(node.lastHeartbeat == "2016-04-16 07:36:03.988");

        QVector<QString> allKeys;
        m_DBHandler->getAllEdgeNodeKeys(allKeys);
        Q_ASSERT(allKeys.size() == 3);
        Q_ASSERT(checkString(allKeys[0], "ABCD", "EFGH", "IJKL"));

        std::vector<std::unique_ptr<DatabaseHandler::EdgeNode>> edges;
        m_DBHandler->getAllEdgeNodes(edges);
        Q_ASSERT(edges.size() == 3);
        Q_ASSERT(checkString((*(edges[2])).macAddress, "ABCD", "EFGH", "IJKL"));

        QVector<QString> allOnlineEdges;
        m_DBHandler->getOnlineEdgeNodes(allOnlineEdges);
        Q_ASSERT(allOnlineEdges.size() == 2);

        Q_ASSERT(m_DBHandler->getEdgeNode(node, "ABCD"));
        Q_ASSERT(node.isOnline  == true);
        m_DBHandler->setEdgeNodeOnlineStatus("ABCD", false);
        Q_ASSERT(m_DBHandler->getEdgeNode(node, "ABCD"));
        Q_ASSERT(node.isOnline == false);

        allOnlineEdges.clear();
        m_DBHandler->getOnlineEdgeNodes(allOnlineEdges);
        Q_ASSERT(allOnlineEdges.size() == 1);
        Q_ASSERT(allOnlineEdges[0] == "IJKL");

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testCaseEdgeNode failed with exception = %s", e.what());
    }
}

void TestHandler::testVirus()
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM virushash");

        DatabaseHandler::VirusHash virusHash;
        m_DBHandler->registerVirusHash("UVUUNNU", "Totally");
        m_DBHandler->registerVirusHash("YUCWZXB", "not a");
        m_DBHandler->registerVirusHash("OPMIMOIBTV", "virus");

        Q_ASSERT(m_DBHandler->getVirusHash(virusHash, "UVUUNNU"));
        Q_ASSERT(virusHash.description == "Totally");

        QVector<QString> allKeys;
        m_DBHandler->getAllVirusHashKeys(allKeys);
        Q_ASSERT(allKeys.size() == 3);
        Q_ASSERT(checkString(allKeys[0], "UVUUNNU", "YUCWZXB", "OPMIMOIBTV"));

        std::vector<std::unique_ptr<DatabaseHandler::VirusHash>> virusHashes;
        m_DBHandler->getAllVirusHashes(virusHashes);
        Q_ASSERT(virusHashes.size() == 3);
        Q_ASSERT(checkString((*(virusHashes[2])).description, "Totally", "not a", "virus"));

        Q_ASSERT(m_DBHandler->isHashInVirusDatabase("OPMIMOIBTV"));
        Q_ASSERT(!(m_DBHandler->isHashInVirusDatabase("NO HASH HERE")));

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testVirus failed with exception = %s", e.what());
    }
}

void TestHandler::testCaseProductVendor()
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM productvendor");

        DatabaseHandler::ProductVendor productvendor;
        m_DBHandler->registerProductVendor("QWER", "Make", "DCBA", "Sabaton");
        m_DBHandler->registerProductVendor("TYUI", "Pepsi Twist", "HGFE", "Babymetal");
        m_DBHandler->registerProductVendor("ASDF", "Again", "LKJI", "Nightwish");

        Q_ASSERT(m_DBHandler->getProductVendor(productvendor, "TYUI", "HGFE"));
        Q_ASSERT(productvendor.productName == "Pepsi Twist");
        Q_ASSERT(productvendor.vendorName == "Babymetal");

        std::vector<std::unique_ptr<DatabaseHandler::ProductVendor>> productvendors;
        m_DBHandler->getAllProductVendors(productvendors);
        Q_ASSERT(productvendors.size() == 3);
        Q_ASSERT(checkString((*(productvendors[2])).productName, "Make", "Pepsi Twist", "Again"));
        Q_ASSERT(checkString((*(productvendors[2])).vendorName, "Sabaton", "Babymetal", "Nightwish"));

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testCaseProductVendor failed with exception = %s", e.what());
    }
}

void TestHandler::testCaseDevice(bool requiredDataExists)
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM device");

        if(!requiredDataExists)
        {
            testCaseProductVendor();
        }

        std::vector<std::unique_ptr<DatabaseHandler::ProductVendor>> productVendors;

        m_DBHandler->getAllProductVendors(productVendors);
        Q_ASSERT(productVendors.size() == 3);

        for(uint i = 0; i < productVendors.size(); ++i)
        {
            m_DBHandler->registerDevice(productVendors[i]->productId, productVendors[i]->vendorId, QString::number(i + 1000));
        }

        m_DBHandler->registerDevice(productVendors[0]->productId, productVendors[0]->vendorId, QString::number(1000)); //Should be ignored

        // Test device with serial number
        DatabaseHandler::Device device;
        Q_ASSERT(m_DBHandler->getDevice(device, productVendors[0]->productId, productVendors[0]->vendorId, QString::number(1000)));
        Q_ASSERT(device.productId == productVendors[0]->productId);
        Q_ASSERT(device.vendorId == productVendors[0]->vendorId);
        Q_ASSERT(device.serialNumber == QString::number(1000));

        std::vector<std::unique_ptr<DatabaseHandler::Device>> devices;
        m_DBHandler->getAllDevices(devices);
        Q_ASSERT(devices.size() == 3);
        Q_ASSERT(checkString((*(devices[0])).productId, productVendors[0]->productId, productVendors[1]->productId, productVendors[2]->productId));

        Q_ASSERT(!m_DBHandler->isDeviceBlackListed(device.productId, device.vendorId, device.serialNumber));
        Q_ASSERT(!m_DBHandler->isDeviceWhiteListed(device.productId, device.vendorId,device.serialNumber));
        m_DBHandler->setDeviceWhitelisted(device.productId, device.vendorId,device.serialNumber);
        Q_ASSERT(!m_DBHandler->isDeviceBlackListed(device.productId, device.vendorId,device.serialNumber));
        Q_ASSERT(m_DBHandler->isDeviceWhiteListed(device.productId, device.vendorId,device.serialNumber));
        m_DBHandler->setDeviceBlacklisted(device.productId, device.vendorId,device.serialNumber);
        Q_ASSERT(m_DBHandler->isDeviceBlackListed(device.productId, device.vendorId,device.serialNumber));
        Q_ASSERT(!m_DBHandler->isDeviceWhiteListed(device.productId, device.vendorId,device.serialNumber));

        Q_ASSERT(!m_DBHandler->isDeviceBlackListed(device.productId, device.vendorId, "1234"));
        Q_ASSERT(!m_DBHandler->isDeviceWhiteListed(device.productId, device.vendorId, "1234"));

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testCaseDevice failed with exception = %s", e.what());
    }
}

void TestHandler::testCaseConnectedDevice(bool requiredDataExists)
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM connecteddevice");

        if(!requiredDataExists)
        {
            testCaseEdgeNode();
            testCaseDevice(false);
        }

        QVector<QString> edgeKeys;
        std::vector<std::unique_ptr<DatabaseHandler::Device>> devices;

        m_DBHandler->getAllEdgeNodeKeys(edgeKeys);
        m_DBHandler->getAllDevices(devices);
        Q_ASSERT(edgeKeys.size() == devices.size());
        Q_ASSERT(edgeKeys.size() == 3);

        for(int i = 0; i < edgeKeys.size(); ++i)
        {
            m_DBHandler->registerConnectedDevice(edgeKeys[i], devices[i]->productId, devices[i]->vendorId, devices[i]->serialNumber, "2021:09:09 22:36:00:00" + QString::number(i));
        }

        std::vector<std::unique_ptr<DatabaseHandler::ConnectedDevice>> connectedDevices;
        m_DBHandler->getAllConnectedDevices(connectedDevices);
        Q_ASSERT(connectedDevices.size() == 3);
        Q_ASSERT(checkString((*(connectedDevices[0])).connectedEdgeNodeMacAddress, edgeKeys[0], edgeKeys[1], edgeKeys[2]));
        Q_ASSERT(checkString((*(connectedDevices[1])).deviceSerialNumber, "1000", "1001", "1002"));

        m_DBHandler->unregisterConnectedDevice(edgeKeys[1], devices[1]->productId, devices[1]->vendorId, devices[1]->serialNumber);

        connectedDevices.clear();
        m_DBHandler->getAllConnectedDevices(connectedDevices);
        Q_ASSERT(connectedDevices.size() == 2);
        for(const std::unique_ptr<DatabaseHandler::ConnectedDevice>& cd: connectedDevices)
        {
            Q_ASSERT(cd->connectedEdgeNodeMacAddress != edgeKeys[1]);
            Q_ASSERT(cd->deviceProductId != devices[1]->productId);
            Q_ASSERT(cd->deviceSerialNumber != devices[1]->serialNumber);
        }

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testCaseLog failed with exception = %s", e.what());
    }
}

void TestHandler::testCaseLog(bool requiredDataExists)
{
    try
    {
        QSqlQuery query;
        query.exec("DELETE FROM log");

        if(!requiredDataExists)
        {
            testCaseEdgeNode();
            testCaseDevice(false);
        }

        QVector<QString> edgeKeys;
        std::vector<std::unique_ptr<DatabaseHandler::Device>> devices;

        m_DBHandler->getAllEdgeNodeKeys(edgeKeys);
        m_DBHandler->getAllDevices(devices);
        Q_ASSERT(edgeKeys.size() == devices.size());
        Q_ASSERT(edgeKeys.size() == 3);

        for(int i = 0; i < edgeKeys.size(); ++i)
        {
            m_DBHandler->logEvent(edgeKeys[i], devices[i]->productId, devices[i]->vendorId, devices[i]->serialNumber, "2021:09:09 22:36:00:00" + QString::number(i), "Number " + QString::number(i));
        }

        DatabaseHandler::LogEvent logEvent;
        Q_ASSERT(m_DBHandler->getLoggedEvent(logEvent, edgeKeys[2], devices[2]->productId, devices[2]->vendorId, devices[2]->serialNumber, "2021:09:09 22:36:00:002"));

        std::vector<std::unique_ptr<DatabaseHandler::LogEvent>> logEvents;
        m_DBHandler->getAllLoggedEvents(logEvents);
        Q_ASSERT(logEvents.size() == 3);
        Q_ASSERT(checkString((*(logEvents[0])).edgeNodeMacAddress, edgeKeys[0], edgeKeys[1], edgeKeys[2]));
        Q_ASSERT(checkString((*(logEvents[1])).timestamp, "2021:09:09 22:36:00:000", "2021:09:09 22:36:00:001", "2021:09:09 22:36:00:002"));

        qCritical() << __PRETTY_FUNCTION__ << "Completed successfully";
    }
    catch(std::exception& e)
    {
        qFatal("testCaseLog failed with exception = %s", e.what());
    }
}

void TestHandler::testCaseAll()
{
    testCaseEdgeNode();
    testVirus();
    testCaseProductVendor();
    testCaseDevice(true);
    testCaseConnectedDevice(true);
    testCaseLog(true);
}

bool TestHandler::checkString(const QString &query, const QString &target1, const QString &target2, const QString &target3)
{
    if((query == target1) ||
        (query == target2) ||
        (query == target3))
    {
        return true;
    }
    else
    {
        return false;
    }
}
