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

int Detector::scan(std::string& a_dataStr, std::string a_source)
{
   for (auto& sig : m_sourceSignature[a_source])
   {
      auto detection = a_dataStr.find( sig );
      if( detection != std::string::npos )
      {
         std::cout << a_dataStr.substr(detection, sig.length()) << std::endl
                   << a_dataStr << std::endl;
      }
   }

   return 0;
}

void Detector::updateSignature(std::vector<std::string> a_sig, std::string a_source)
{
   for (const auto& e : a_sig)
   {
      m_sourceSignature[ a_source ].push_back( e );
   }
}

