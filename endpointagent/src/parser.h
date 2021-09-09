#pragma once

#include <QObject>
#include <vector>
#include <string>
#include <map>

#include "detector.h"

typedef unsigned char byte;
/// Purpose:
/// Parses log files from argument-passed filepaths upon start
/// fresh data is sent to detector

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(std::string &filepath , QObject *parent = nullptr);
    void initializeSources(std::string filePath);
    void readLogSource();



private:

    int m_SourceCount;
    Detector m_DataHandler;
    std::map <int, std::string> m_sourceName;
    std::map <std::string, std::string> m_sourcePath;
    std::map <std::string, uint64_t> m_sourceOffset;


signals:

};

