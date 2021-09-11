#ifndef DATABASEDATAFILEPARSER_H
#define DATABASEDATAFILEPARSER_H


class DatabaseHandler;
class DatabaseDataFileParser
{
public:
    static void parseDeviceProductVendor(DatabaseHandler* dbHandler);
    static void parseVirusHash(DatabaseHandler* dbHandler);

private:
    DatabaseDataFileParser();
};

#endif // DATABASEDATAFILEPARSER_H
