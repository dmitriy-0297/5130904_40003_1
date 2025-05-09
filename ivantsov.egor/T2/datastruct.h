#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <iosfwd>

struct DataStruct_t
{
  double key1_ = 0.0;
  unsigned long long key2_ = 0;
  std::string key3_;
};

std::ostream& operator<<(std::ostream& out, const DataStruct_t& data);
std::istream& operator>>(std::istream& in, DataStruct_t& data);

bool dataStructComparator(const DataStruct_t& lhs, const DataStruct_t& rhs);

#endif // DATASTRUCT_H
