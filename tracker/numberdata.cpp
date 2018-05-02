#include <sstream>
#include <iostream>
#include <limits>
#include <string>
#include <cmath>
#include "numberdata.h"

NumberData::NumberData(const string& fn ) :
  numberParser(fn),
  fileName( fn ),
  numberData(numberParser.getXmlData())
{ }

void NumberData::setNumber(int h, std::string type) {
  std::stringstream strm;
  strm << h;
  numberData["numberOf"+type] = strm.str();
}


int NumberData::getNumber(std::string type) const {
  std::string tag("numberOf"+type);
  std::map<std::string, string>::const_iterator ptr = numberData.find(tag);
  if ( ptr == numberData.end() )
    throw std::string("Game: Didn't find integer tag ")+tag+
          std::string(" in number xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    int data;
    strm >> data;
    return data;
  }
}

void NumberData::writeTheXml() const {
  std::ofstream xmlFile;
  xmlFile.open (fileName);
  xmlFile << "<?xml version = \"1.0\"?>\n";
  xmlFile << "<GameData>\n";
  map<std::string, std::string>::const_iterator ptr =
    numberData.begin();
  while ( ptr != numberData.end() ) {
    xmlFile << "  " << "<" << ptr->first << ">";
    xmlFile << ptr->second;
    xmlFile << "</" << ptr->first << ">";
    xmlFile << std::endl;
    ++ptr;
  }
  xmlFile << "</GameData>\n";
  xmlFile.close();
}

void NumberData::displayData() const {
  map<std::string, std::string>::const_iterator ptr =
    numberData.begin();
  while ( ptr != numberData.end() ) {
    std::cout << ptr->first << ", " << ptr->second << std::endl;
    ++ptr;
  }
}


