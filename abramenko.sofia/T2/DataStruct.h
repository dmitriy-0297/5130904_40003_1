#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>

struct DataStruct
{
    unsigned long long key1 = 0;
    unsigned long long key2 = 0;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool different(const DataStruct& a, const DataStruct& b);

#endif
