#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <string>
#include <iostream>


struct DataStruct
{
  double key1{};
  long long key2{};
  std::string key3;
};

class StreamGuard
{
public:
  explicit StreamGuard(std::basic_ios<char>& s);
  ~StreamGuard();

private:
  std::basic_ios<char>& s_;
  std::streamsize precision_;
  std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);

bool compareDataStructs(const DataStruct& a, const DataStruct& b);

std::string convertDoubleToScientificString(double value);

#endif //DATASTRUCT_H
