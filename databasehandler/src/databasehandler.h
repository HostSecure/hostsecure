#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>

class QSqlQuery;

class DatabaseHandler
{
public:
    DatabaseHandler(const QString& databasePath);

    // All functions should be assumed to throw std::exceptions upon failure

    // Edge node
    struct EdgeNode
    {
        QString macAddress = "";
        bool isOnline = false;
        QString lastHeartbeat = "";
    };
    void registerEdgeNode(const QString& macAddress, bool isOnline, const QString& lastHeartbeatTimestamp) const;
    bool getEdgeNode(EdgeNode& edgeNode, const QString& macAddress) const;
    void getAllEdgeNodeKeys(QVector<QString>& macAddresses) const;
    void getAllEdgeNodes(std::vector<std::unique_ptr<EdgeNode>>& edgeNodes) const;
    void setEdgeNodeOnlineStatus(const QString& macAddress, bool isOnline, const QString& lastHeartbeatTimestamp = "");
    void getOnlineEdgeNodes(QVector<QString>& macAddresses) const;

    //Device
    struct Device
    {
        QString vendorId = "";
        QString productId = "";
        QString serialNumber = "";
    };
    void registerDevice(const QString& productId, const QString& vendorId, const QString& serialNumber) const;
    bool getDevice(Device& device, const QString& productId, const QString& vendorId, const QString& serialNumber) const;
    void getAllDevices(std::vector<std::unique_ptr<Device>>& devices) const;
    void setDeviceBlacklisted(const QString& productId, const QString& vendorId, const QString& serialNumber) const;
    bool isDeviceBlackListed(const QString& productId, const QString& vendorId, const QString& serialNumber) const;
    void setDeviceWhitelisted(const QString& productId, const QString& vendorId, const QString& serialNumber) const;
    bool isDeviceWhiteListed(const QString& productId, const QString& vendorId, const QString& serialNumber) const;

    // Connected devices
    struct ConnectedDevice
    {
        QString connectedEdgeNodeMacAddress = "";
        QString deviceProductId = "";
        QString deviceVendorId = "";
        QString deviceSerialNumber = "";
    };
    void registerConnectedDevice(const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber, const QString& timestamp);
    void unregisterConnectedDevicesOnEdgeNode(const QString& edgeNodeMacAddress);
    void unregisterConnectedDevice(const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber);
    void getAllConnectedDevices(std::vector<std::unique_ptr<ConnectedDevice>>& connectedDevices);


    // ProductVendor
    struct ProductVendor
    {
        QString productId = "";
        QString productName = "";
        QString vendorId = "";
        QString vendorName = "";
    };
    void registerProductVendor(const QString& productId, const QString& productName, const QString& vendorId, const QString& vendorName);
    bool getProductVendor(ProductVendor& productVendor, const QString& productId, const QString vendorId);
    void getAllProductVendors(std::vector<std::unique_ptr<ProductVendor>>& productVendors);

    // Virus
    struct VirusHash
    {
        QString virusHash = "";
        QString description = "";
    };
    void registerVirusHash(const QString& virusHash, const QString& description);
    bool getVirusHash(VirusHash& vHash, const QString& virusHash) const;
    void getAllVirusHashKeys(QVector<QString>& virusHashes) const;
    void getAllVirusHashes(std::vector<std::unique_ptr<VirusHash>>& virusHashes) const ;
    bool isHashInVirusDatabase(const QString& hash) const;

    // Event logging
    struct LogEvent
    {
        QString edgeNodeMacAddress = "";
        QString deviceProductId = "";
        QString deviceVendorId = "";
        QString deviceSerialNumber = "";
        QString timestamp = "";
        QString eventDescription = "";
    };
    void logEvent(const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber, const QString& timestamp, const QString& eventDescription);
    bool getLoggedEvent(LogEvent& logEvent, const QString& edgeNodeMacAddress, const QString& deviceProductId, const QString& deviceVendorId, const QString& deviceSerialNumber, const QString& timestamp) const;
    void getAllLoggedEvents(std::vector<std::unique_ptr<LogEvent>>& loggedEvents) const;

private:
    void getKeysFromTable(const QString keyName, const QString& tableName, QVector<QString>& result) const;
    void setDeviceStatus(const QString& productId, const QString& vendorId, const QString& serialNumber, const QString& status) const;
    bool checkDeviceStatus(const QString& productId, const QString& vendorId, const QString& serialNumber, const QString& status) const;
};

#endif // DATABASEHANDLER_H
