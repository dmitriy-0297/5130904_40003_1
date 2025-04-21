#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

// (:key1 89ull:key2 0b1000101:key3 "Data":)

struct DataStruct {
    unsigned long long key1 = 0;
    unsigned long long key2 = 0;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data) {
    // (:key1 89ull:key2 0b1000101:key3 "Data":)

    std::string row;
    if (!std::getline(in, row)) {
        return in;
    }

    size_t pos = row.find("key1");
    if (pos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    size_t begin = row.find(' ', pos) + 1;
    size_t end = row.find('u', begin);
    std::string str1 = row.substr(begin, end - begin);
    try {
        data.key1 = std::stoull(str1);
    }
    catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }

    pos = row.find("key2");
    if (pos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    begin = row.find(' ', pos) + 1;
    end = row.find(':', begin);
    std::string str2 = row.substr(begin + 2, end - (begin + 2));
    try {
        data.key2 = std::stoull(str2, nullptr, 2);
    }
    catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }

    pos = row.find("key3");
    if (pos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    begin = row.find('"', pos) + 1;
    end = row.find('"', begin);
    std::string str3 = row.substr(begin, end - begin);
    data.key3 = str3;

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    // (:key1 89ull:key2 0b1000101:key3 "Data":)
    out << "(:key1 " << data.key1 << "ull";
    out << ":key2 0b";

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
    out << ":key3 \"" << data.key3 << "\":)";

    return out;
}

bool different(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.size() < b.key3.size();
}

int main() {
    // (:key1 89ull:key2 0b1000101:key3 "Data":)

    std::vector<DataStruct> data;

    std::istringstream iss(
        "(:key1 89ull:key2 0b1000101:key3 \"Data\":)\n"
        "(:key2 0b110:key1 42ull:key3 \"Another Data\":)\n"
        "(:key2 0b110:key1 89ull:key3 \"Another Data\":)\n"
        "(:key1 42ull:key2 0b110:key3 \"Data\":)\n"
    );

    std::copy(
        std::istream_iterator<DataStruct>(iss),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), different);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
