#include "command_processor.hpp"

namespace
{
    using CmdPair = std::pair<const char*, int>;

    struct SameCmd
    {
        const std::string& key;
        bool operator()(const CmdPair& p) const
        {
            return key == p.first;
        }
    };
}

namespace vurvaa
{
    CommandProcessor::CommandProcessor(std::vector<Polygon>& v) :
        polygons_(v)
    {
    }

    void CommandProcessor::run(std::istream& in, std::ostream& out)
    {
        const std::vector<CmdPair> cmds
        {
            { "AREA", 0 }, { "COUNT", 1 }, { "MIN", 2 },
            { "MAX", 3 }, { "MAXSEQ", 4 }, { "ECHO", 5 }
        };

        std::string word;
        while (in >> word)
        {
            auto it = std::find_if(cmds.begin(), cmds.end(), SameCmd{ word });
            int id = it != cmds.end() ? it->second : -1;

            switch (id)
            {
            case 0:
                handleArea(in, out);
                break;
            case 1:
                handleCount(in, out);
                break;
            case 2:
                handleMin(in, out);
                break;
            case 3:
                handleMax(in, out);
                break;
            case 4:
                handleMaxSeq(in, out);
                break;
            case 5:
                handleEcho(in, out);
                break;
            default:
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    }

    void CommandProcessor::handleArea(std::istream& in, std::ostream& out)
    {
        std::string arg;
        if (!(in >> arg))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        if (arg == "EVEN")
        {
            out << areaEven(polygons_) << '\n';
        }
        else if (arg == "ODD")
        {
            out << areaOdd(polygons_) << '\n';
        }
        else if (arg == "MEAN")
        {
            if (polygons_.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
            out << areaMean(polygons_) << '\n';
        }
        else
        {
            try
            {
                std::size_t n = std::stoul(arg);
                if (n < 3)
                {
                    std::cout << "<INVALID COMMAND>\n";
                    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return;
                }
                out << areaWithVertices(polygons_, n) << '\n';
            }
            catch (const std::invalid_argument&)
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            catch (const std::out_of_range&)
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    void CommandProcessor::handleCount(std::istream& in, std::ostream& out)
    {
        std::string arg;
        if (!(in >> arg))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        if (arg == "EVEN")
        {
            out << countEven(polygons_) << '\n';
        }
        else if (arg == "ODD")
        {
            out << countOdd(polygons_) << '\n';
        }
        else
        {
            try
            {
                std::size_t n = std::stoul(arg);
                if (n < 3)
                {
                    std::cout << "<INVALID COMMAND>\n";
                    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return;
                }
                out << countWithNVertices(polygons_, n) << '\n';
            }
            catch (const std::invalid_argument&)
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            catch (const std::out_of_range&)
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    void CommandProcessor::handleMin(std::istream& in, std::ostream& out)
    {
        std::string arg;
        if (!(in >> arg))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        if (arg == "AREA")
        {
            if (polygons_.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
            out << minArea(polygons_) << '\n';
        }
        else if (arg == "VERTEXES")
        {
            if (polygons_.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
            out << minVertices(polygons_) << '\n';
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void CommandProcessor::handleMax(std::istream& in, std::ostream& out)
    {
        std::string arg;
        if (!(in >> arg))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        if (arg == "AREA")
        {
            if (polygons_.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
            out << maxArea(polygons_) << '\n';
        }
        else if (arg == "VERTEXES")
        {
            if (polygons_.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
            out << maxVertices(polygons_) << '\n';
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void CommandProcessor::handleMaxSeq(std::istream& in, std::ostream& out)
    {
        Polygon pattern;
        if (!(in >> pattern))
        {
            std::cout << "<INVALID COMMAND>\n";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        out << maxSeq(polygons_, pattern) << '\n';
    }

    void CommandProcessor::handleEcho(std::istream& in, std::ostream& out)
    {
        Polygon pattern;
        if (!(in >> pattern))
        {
            std::cout << "<INVALID COMMAND>\n";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        out << echo(polygons_, pattern) << '\n';
    }
}
