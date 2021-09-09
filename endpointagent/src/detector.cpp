#include "detector.h"
#include <iostream>
#include <QDebug>


Detector::Detector()
{
    m_sourceSignature["fapolicyd"].push_back("dec=deny_log");
    m_sourceSignature["firewalld"].push_back("filter_IN_drop_DROP");
    m_sourceSignature["firewalld"].push_back("filter_IN_reject_REJECT");
    m_sourceSignature["usbguard"].push_back("usbguard-daemon");

}

int Detector::scan(std::string& dataStr, std::string source)
{
    for (auto& sig : m_sourceSignature[source])
    {
        auto detection = dataStr.find( sig );
        if( detection != std::string::npos )
        {
                std::cout << dataStr.substr(detection, sig.length()) << std::endl
                          << dataStr << std::endl;

        }
    }


    return 0;
}

void Detector::updateSignature(std::vector<std::string> sig, std::string source)
{
    for (auto& e : sig)
    {
        m_sourceSignature[ source ].push_back(e);
    }
}

