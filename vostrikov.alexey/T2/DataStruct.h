#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <iostream>
#include <vector>

namespace mydatastruct {

    bool isValidDouble(const std::string& str);
    bool isValidChar(const std::string& str);
    bool isValidStr(const std::string& str);

    struct DataStruct
    {
        float key1;
        char key2;
        std::string key3;
        bool valid = true;

        DataStruct();
        explicit DataStruct(const std::string& str);

        friend std::istream& operator>>(std::istream& in, DataStruct& ds);

        friend std::ostream& operator<<(std::ostream& out, const DataStruct& ds);
    };


    void fillvector(std::vector<DataStruct>& vec);

    bool datastructSort(const DataStruct &el1, const DataStruct &el2);
}

#endif
