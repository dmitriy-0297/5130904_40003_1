#include "polygon_operations.hpp"

namespace
{
    using SequenceState = std::pair<int, int>;

    struct SequenceFold
    {
        const vurvaa::Polygon& target;

        SequenceState operator()(SequenceState st, const vurvaa::Polygon& p) const
        {
            st.first = (p == target) ? st.first + 1 : 0;
            st.second = std::max(st.second, st.first);
            return st;
        }
    };

    using EchoState = std::pair<std::vector<vurvaa::Polygon>, int>;

    struct EchoFold
    {
        const vurvaa::Polygon& pattern;

        EchoState operator()(EchoState st, const vurvaa::Polygon& p) const
        {
            st.first.push_back(p);
            if (p == pattern)
            {
                st.first.push_back(p);
                ++st.second;
            }
            return st;
        }
    };
}

namespace vurvaa
{
    double areaEven(const std::vector<Polygon>& polygons)
    {
        return std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p)
            {
                return IsEven()(p) ? sum + polygonArea(p) : sum;
            }
        );
    }

    double areaOdd(const std::vector<Polygon>& polygons)
    {
        return std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p)
            {
                return IsOdd()(p) ? sum + polygonArea(p) : sum;
            }
        );
    }

    double areaMean(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }

        double total = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p)
            {
                return sum + polygonArea(p);
            }
        );

        return total / polygons.size();
    }

    double areaWithVertices(const std::vector<Polygon>& polygons, size_t num)
    {
        return std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            [num](double sum, const Polygon& p)
            {
                return (p.points.size() == num) ? sum + polygonArea(p) : sum;
            }
        );
    }

    size_t polygonVertexCount(const Polygon& p)
    {
        return p.points.size();
    }

    double maxArea(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }

        auto it = std::max_element(
            polygons.begin(), polygons.end(),
            std::bind(
                std::less<double>(),
                std::bind(polygonArea, std::placeholders::_1),
                std::bind(polygonArea, std::placeholders::_2)
            )
        );
        return polygonArea(*it);
    }

    double minArea(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }

        auto it = std::min_element(
            polygons.begin(), polygons.end(),
            std::bind(
                std::less<double>(),
                std::bind(polygonArea, std::placeholders::_1),
                std::bind(polygonArea, std::placeholders::_2)
            )
        );
        return polygonArea(*it);
    }

    size_t maxVertices(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::invalid_argument("INVALID COMMAND");
        }

        auto it = std::max_element(
            polygons.begin(), polygons.end(),
            std::bind(
                std::less<size_t>(),
                std::bind(polygonVertexCount, std::placeholders::_1),
                std::bind(polygonVertexCount, std::placeholders::_2)
            )
        );
        return polygonVertexCount(*it);
    }

    size_t minVertices(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::invalid_argument("INVALID COMMAND");
        }

        auto it = std::min_element(
            polygons.begin(), polygons.end(),
            std::bind(
                std::less<size_t>(),
                std::bind(polygonVertexCount, std::placeholders::_1),
                std::bind(polygonVertexCount, std::placeholders::_2)
            )
        );
        return polygonVertexCount(*it);
    }

    size_t countEven(const std::vector<Polygon>& polygons)
    {
        return std::count_if(polygons.begin(), polygons.end(), IsEven());
    }

    size_t countOdd(const std::vector<Polygon>& polygons)
    {
        return std::count_if(polygons.begin(), polygons.end(), IsOdd());
    }

    size_t countWithNVertices(const std::vector<Polygon>& polygons, size_t n)
    {
        return std::count_if(polygons.begin(), polygons.end(), HasNVertices(n));
    }

    int maxSeq(const std::vector<Polygon>& polygons, const Polygon& pattern)
    {
        auto result = std::accumulate(
            polygons.begin(), polygons.end(),
            SequenceState{ 0, 0 },
            SequenceFold{ pattern }
        );

        return result.second;
    }

    int echo(std::vector<Polygon>& polygons, const Polygon& pattern)
    {
        EchoState init(EchoState(std::vector<Polygon>(), 0));
        EchoFold folder{ pattern };

        EchoState result = std::accumulate(
            polygons.begin(),
            polygons.end(),
            init,
            folder
        );

        polygons = std::move(result.first);
        return result.second;
    }

    bool IsEven::operator()(const Polygon& p) const
    {
        return p.points.size() % 2 == 0;
    }

    bool IsOdd::operator()(const Polygon& p) const
    {
        return p.points.size() % 2 != 0;
    }

    HasNVertices::HasNVertices(size_t num) : n(num)
    {
    }

    bool HasNVertices::operator()(const Polygon& p) const
    {
        return p.points.size() == n;
    }
}
