#include "DataStruct.h"

#include <vector>
#include <iterator>
#include <algorithm>

int main() {
    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    if (data.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    } else if (data.size() == 1) {
        std::cout << "Atleast one supported record type\n";
    }

    std::sort(data.begin(), data.end(), different);

    if (data.size() > 1) {
        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<DataStruct>(std::cout, "\n")
        );
    }

    return 0;
}
