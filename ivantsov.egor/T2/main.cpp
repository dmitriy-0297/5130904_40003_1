#include "datastruct.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

const std::string FATAL_ERROR = "Achtung, 3.14zdez";

int main()
{
  try {
    std::vector<DataStruct_t> data;

    while (true) {
      DataStruct_t temp;
      std::cin >> temp;

      if (!std::cin.good()) {
        if (std::cin.eof()) {
          break;
        }
        std::cin.clear();
        continue;
      }

      data.push_back(temp);
    }

    if (data.empty()) {
      throw std::runtime_error(FATAL_ERROR);
    }

    std::sort(data.begin(), data.end(), dataStructComparator);

    for (const auto& item : data) {
      std::cout << item << "\n";
    }
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
