#pragma once
#include <QObject>
#include <vector>
#include <string>
#include <map>
#include "detector.h"

//!
//! \brief The Parser class (Halted)
//! Purpose is for parsing local service log files, and initial config files
//! correlate and detect data anomalies
//!
class Parser : public QObject
{
   Q_OBJECT
public:
   explicit Parser( std::string& a_filepath , QObject* a_parent = nullptr);
   void initializeSources(std::string a_filePath);
   void readLogSource();

private:
   int m_SourceCount;
   Detector m_DataHandler;
   std::map <int, std::string> m_sourceName;
   std::map <std::string, std::string> m_sourcePath;
   std::map <std::string, uint64_t> m_sourceOffset;
};

