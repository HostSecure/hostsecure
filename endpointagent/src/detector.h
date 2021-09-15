#pragma once
#include <QObject>
#include <sstream>
#include <vector>
#include <string>
#include <map>

//! Purpose:
//! Detect anomalies from parsed log files
//! receives data from Parser class
class Detector : public QObject
{
public:
   Detector();

   int scan(std::string& a_dataStr, std::string a_source);
   void updateSignature(std::vector<std::string> a_sig, std::string a_source);
   void HandleAnomaly();

private:
   std::map <std::string, std::vector<std::string> > m_sourceSignature;
};

