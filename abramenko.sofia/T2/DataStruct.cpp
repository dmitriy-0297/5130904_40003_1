#include "DataStruct.h"
#include <algorithm>

std::istream& operator>>(std::istream& in, DataStruct& data) {
    while (true) {
        std::string row;
        if (!std::getline(in, row)) return in;
        if (row.empty()) {
            in.setstate(std::ios::failbit);
            return in;
        }

        size_t pos = row.find("key1");
        if (pos == std::string::npos) {
            continue;
        }
        size_t begin = row.find(' ', pos) + 1;
        size_t end = row.find("ull", begin);
        std::string str1 = row.substr(begin, end - begin);

        if (end == std::string::npos) {
            continue;
        }

        try {
            data.key1 = std::stoull(str1);
        }
        catch (...) {
            continue;
        }

        pos = row.find("key2");
        if (pos == std::string::npos) {
            continue;
        }
        begin = row.find(' ', pos) + 1;
        end = row.find(':', begin);
        std::string str2 = row.substr(begin, end - begin);

        if (str2.size() < 2 || (str2.substr(0, 2) != "0b" && str2.substr(0, 2) != "0B")) {
            continue;
        }

        size_t firstOne = str2.find_first_not_of('0', 2);
        int leadingZeros = 0;
        if (firstOne == std::string::npos) {
            leadingZeros = str2.size();
        }
        else {
            leadingZeros = firstOne - 2;
        }

        try {
            data.key2 = std::stoull(str2.substr(2), nullptr, 2);
        }
        catch (...) {
            continue;
        }

        pos = row.find("key3");
        if (pos == std::string::npos) {
            continue;
        }
        begin = row.find('"', pos) + 1;
        end = row.find('"', begin);
        std::string str3 = row.substr(begin, end - begin);
        data.key3 = std::to_string(leadingZeros) + "/" + str3;

        return in;
    }
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1 << "ull";

    size_t slash = data.key3.find('/');
    int leadingZeros = 0;
    std::string leadingZerosStr;
    std::string key3 = data.key3;

    if (slash != std::string::npos) {
        leadingZerosStr = data.key3.substr(0, slash);
        leadingZeros = std::stoi(leadingZerosStr);
        key3 = data.key3.substr(slash + 1);
    }

    out << ":key2 0b" << std::string(leadingZeros, '0');

    unsigned long long a = data.key2;
    if (a == 0) {
        out << "0";
    }
    else {
        int binary[32];
        int i = 0;
        while (a > 0) {
            binary[i] = a % 2;
            a = a / 2;
            i++;
        }
        for (int j = i - 1; j >= 0; j--) {
            out << binary[j];
        }
    }

    out << ":key3 \"" << key3 << "\":)";

    return out;
}

bool different(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;

    size_t slash_a = a.key3.find('/');
    size_t slash_b = b.key3.find('/');

    std::string aStr;
    std::string bStr;

    if (slash_a != std::string::npos) {
        aStr = a.key3.substr(slash_a + 1);
    }
    else {
        aStr = a.key3;
    }

    if (slash_b != std::string::npos) {
        bStr = b.key3.substr(slash_b + 1);
    }
    else {
        bStr = b.key3;
    }

    return aStr.size() < bStr.size();
}
