#include "ds_comparator.hpp"
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>
#include <iostream>

int main()
{
    std::vector<vurvaa::DataStruct> dataStructV;

    while (!std::cin.eof())
    {
        std::copy(
            std::istream_iterator<vurvaa::DataStruct>(std::cin),
            std::istream_iterator<vurvaa::DataStruct>(),
            std::back_inserter(dataStructV)
        );

        if (std::cin.fail() && !std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(dataStructV.begin(), dataStructV.end(), vurvaa::compare);

    std::copy(
        std::begin(dataStructV),
        std::end(dataStructV),
        std::ostream_iterator<vurvaa::DataStruct>(std::cout, "\n")
    );

    return 0;
}
