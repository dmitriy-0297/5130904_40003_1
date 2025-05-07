#include "DataStruct.h"
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    setlocale(LC_ALL, "rus");
    using mynsp::DataStruct;

    std::vector<DataStruct> data;

    while (!std::cin.eof()) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
        } else {
            if (std::cin.eof()) break; // Проверка на пустой ввод
            std::cin.clear(); // сбрасываем failbit, хотим продолжать
            std::string skip;
            std::getline(std::cin, skip);
        }
    }

    std::sort(data.begin(), data.end(), mynsp::compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
