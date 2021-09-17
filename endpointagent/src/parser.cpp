#include "parser.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <QDebug>


namespace
{
   constexpr uint LINE_LIMIT = 500000;
}

Parser::Parser(std::string& a_filepath, QObject *a_parent) : QObject(a_parent)
{
   initializeSources(a_filepath);
}

void Parser::initializeSources(std::string a_filePath)
{
   std::ifstream ifsStream;
   std::string line;
   std::stringstream dataStream;

   ifsStream.open( a_filePath.c_str() );
   if (ifsStream.is_open() )
   {
      int i = 0;
      std::string tmpSource;
      std::string tmpFilePath;
      while ( std::getline(ifsStream, line))
      {
         dataStream << line;
         dataStream >> tmpSource;
         dataStream >> tmpFilePath;

         m_sourceName[i] = tmpSource;
         m_sourcePath[ m_sourceName[i] ] = tmpFilePath;
         dataStream.clear();
         ++i;
      }
      ifsStream.close();

      for( uint ii = 0; ii < m_sourceName.size(); ++ii)
      {
         std::cout << m_sourceName[ii] << " " << m_sourcePath[ m_sourceName[ii] ] << std::endl;
      }
   }
}

void Parser::readLogSource()
{
   std::ifstream ifsStream;
   std::string rawData;

   for( uint i = 0; i < m_sourceName.size(); ++i )
   {
      qDebug() << "opening ifstream";

      uint64_t lineCount = 0;
      ifsStream.open( m_sourcePath[ m_sourceName[ i ] ].c_str() );
      if( ifsStream.is_open() )
      {
         while( std::getline(ifsStream , rawData ))
         {
            if(lineCount >= m_sourceOffset[ m_sourceName[ i ] ])
            {
               m_DataHandler.scan(rawData, m_sourceName[ i ]);
            }

            if(lineCount > LINE_LIMIT)
            {
               qDebug() << "Line limit reached";
               break;

            }
            ++ lineCount;
         }
         ifsStream.close();
         qDebug() << "Close stream";
         m_sourceOffset[ m_sourceName[ i ] ] = lineCount;
      }
      else
      {
         qDebug() << "could not open ifstream";
      }
   }
}
