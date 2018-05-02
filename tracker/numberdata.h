#ifndef NUMBERDATA__H
#define NUMBERDATA__H

#include <string>
#include <map>
#include "vector2f.h"
#include "parseXML.h"

class NumberData {
public:
  NumberData(const std::string&);
  //static NumberData& getInstance();
  ~NumberData() {};
  void displayData() const;

  int  getNumber(std::string) const;
  void setNumber(int h, std::string);
  void writeTheXml() const;

private:
  ParseXML numberParser;
  const std::string fileName;
  map<std::string, std::string> numberData;

  NumberData(const NumberData&);
  NumberData& operator=(const NumberData&);
};
#endif
