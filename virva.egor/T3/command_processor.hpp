#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include "score_guard.hpp"
#include "polygon_operations.hpp"

#include <limits>
#include <utility>
#include <string>

namespace vurvaa
{
    class CommandProcessor
    {
    public:
        CommandProcessor(std::vector<Polygon>& v);
        void run(std::istream& in = std::cin, std::ostream& out = std::cout);

    private:
        void handleArea(std::istream& in, std::ostream& out);
        void handleCount(std::istream& in, std::ostream& out);
        void handleMin(std::istream& in, std::ostream& out);
        void handleMax(std::istream& in, std::ostream& out);
        void handleMaxSeq(std::istream& in, std::ostream& out);
        void handleEcho(std::istream& in, std::ostream& out);

        std::vector<Polygon>& polygons_;
    };
}

#endif
