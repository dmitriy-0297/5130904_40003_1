#include "command_processor.hpp"
#include "polygon_operations.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <limits>
#include <iterator>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }

    const char* FILE_NAME = argv[1];

    std::ifstream in{ FILE_NAME };
    if (!in)
    {
        std::cerr << "Error: cannot open file '" << FILE_NAME << "'\n";
        return 1;
    }

    std::vector<vurvaa::Polygon> PolyVector;

    while (!in.eof())
    {
        std::copy(
            std::istream_iterator<vurvaa::Polygon>(in),
            std::istream_iterator<vurvaa::Polygon>(),
            std::back_inserter(PolyVector)
        );

        if (in.fail() && !in.eof())
        {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    vurvaa::CommandProcessor processor(PolyVector);
    processor.run(std::cin, std::cout);
}
