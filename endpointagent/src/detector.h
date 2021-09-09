#pragma once
#include <QObject>
#include <sstream>
#include <vector>
#include <string>
#include <map>

/// Purpose:
/// Detect anomalies from parsed log files
/// receives data from Parser class
typedef unsigned char byte;

class Detector : public QObject
{
public:
    Detector();

    int scan(std::string &dataStr, std::string source);
    void updateSignature(std::vector<std::string> sig, std::string source);
    void HandleAnomaly();


private:

    std::map <std::string, std::vector<std::string> > m_sourceSignature;


};

