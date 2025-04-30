#include "DataStruct.h"
#include <algorithm>

std::string toBinary(unsigned long long a) {
    if (a == 0) {
        return "0";
    } else {
        int binary[64];
        int i = 0;
        while (a > 0) {
            binary[i] = a % 2;
            a = a / 2;
            i++;
        }

        std::string res;
        for (int j = i - 1; j >= 0; j--) {
            res += (binary[j] + '0');
        }

        return res;
    }
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    while (true) {
        std::string row;
        if (!std::getline(in, row)) {
            return in;
        }
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
        } catch (...) {
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
            leadingZeros = 0;
        } else {
            leadingZeros = firstOne - 2;
        }

        try {
            data.key2 = std::stoull(str2.substr(2), nullptr, 2);
        } catch (...) {
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
    } else {
        key3 = data.key3;
    }

    out << ":key2 0b" << std::string(leadingZeros, '0') << toBinary(data.key2);
    out << ":key3 \"" << key3 << "\":)";

    return out;
}

bool different(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    size_t slash_a = a.key3.find('/');
    int leadingZeros_a = 0;
    std::string leadingZerosStr_a;
    std::string key3_a = a.key3;

    if (slash_a != std::string::npos) {
        leadingZerosStr_a = a.key3.substr(0, slash_a);
        leadingZeros_a = std::stoi(leadingZerosStr_a);
        key3_a = a.key3.substr(slash_a + 1);
    } else {
        key3_a = a.key3;
    }

    size_t slash_b = b.key3.find('/');
    int leadingZeros_b = 0;
    std::string leadingZerosStr_b;
    std::string key3_b = b.key3;

    if (slash_b != std::string::npos) {
        leadingZerosStr_b = b.key3.substr(0, slash_b);
        leadingZeros_b = std::stoi(leadingZerosStr_b);
        key3_b = b.key3.substr(slash_b + 1);
    } else {
        key3_b = b.key3;
    }

    std::string binary_a = std::string(leadingZeros_a, '0') + toBinary(a.key2);
    std::string binary_b = std::string(leadingZeros_b, '0') + toBinary(b.key2);

    if (binary_a != binary_b) {
        return binary_a < binary_b;
    }

    return key3_a.size() < key3_b.size();
}
