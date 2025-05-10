#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <cmath>
#include <set>

namespace
{
    constexpr int MIN_POINTS_FOR_AREA = 3;
    constexpr int POINT_TOKEN_MIN_LENGTH = 5;
    constexpr int INVALID_COMMAND = -1;
    constexpr int EVEN = -2;
    constexpr int ODD = -3;
    constexpr int MEAN = -4;
    constexpr int AREA = -5;
    constexpr int VERTEXES = -6;

    const std::string INVALID_COMMAND_MSG = "<INVALID COMMAND>\n";
}

using namespace std::placeholders;

struct Point
{
    int x = 0;
    int y = 0;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

struct Polygon
{
    std::vector<Point> points;

    bool operator==(const Polygon& other) const
    {
        if (points.size() != other.points.size())
        {
            return false;
        }
        std::multiset<Point> thisPoints(points.begin(), points.end());
        std::multiset<Point> otherPoints(other.points.begin(), other.points.end());

        return thisPoints == otherPoints;
    }
};

int getIntParam(const std::string& option);
double calculateArea(const Polygon& polygon);
bool isNumber(const std::string& str);
bool parsePoint(const std::string& token, Point& point);
bool parseToken(std::istringstream& iss, std::vector<Point>& points, size_t count, bool isFirst);
bool parsePolygon(std::istringstream& iss, Polygon& polygon);
bool isInside(const Polygon& poly, const Point& p);
bool polygonsIntersect(const Polygon& a, const Polygon& b);
void handleLessArea(const std::vector<Polygon>& polygons, std::istringstream& iss);
void handleIntersections(const std::vector<Polygon>& polygons, std::istringstream& iss);
void handleArea(const std::vector<Polygon>& polygons, std::istringstream& iss);
void handleMaxMin(const std::vector<Polygon>& polygons, const std::string& command, std::istringstream& iss);
void handleCount(const std::vector<Polygon>& polygons, std::istringstream& iss);
void processFileInput(std::vector <Polygon>& polygons,
    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, std::istringstream&)>> handlers,
    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, const std::string&, std::istringstream&)>> maxMinHandlers,
    std::string& line);


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open())
    {
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(input, line))
    {
        std::istringstream iss(line);
        Polygon polygon;
        if (parsePolygon(iss, polygon))
        {
            polygons.push_back(std::move(polygon));
        }
    }

    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, std::istringstream&)>> handlers =
    {
        {"LESSAREA", handleLessArea},
        {"INTERSECTIONS", handleIntersections},
        {"AREA", handleArea},
        {"COUNT", handleCount}
    };

    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, const std::string&, std::istringstream&)>> maxMinHandlers =
    {
        {"MAX", handleMaxMin},
        {"MIN", handleMaxMin}
    };

    std::cout << std::fixed << std::setprecision(1);

    processFileInput(polygons, handlers, maxMinHandlers, line);

    return 0;
}


double calculateArea(const Polygon& polygon) {
    if (polygon.points.size() < 3)
    {
        return 0.0;
    }

    std::vector<std::pair<Point, Point>> coordsVec;

    auto getArea = [](double sum, const std::pair<Point, Point>& coords) {
        const Point& i = coords.first;
        const Point& j = coords.second;
        return sum + (i.x * j.y) - (j.x * i.y);
        };

    if (!polygon.points.empty())
    {
        std::transform(polygon.points.begin(), std::prev(polygon.points.end()),
            std::next(polygon.points.begin()), std::back_inserter(coordsVec),
            [](const Point& point, const Point& otherPoint) { return std::make_pair(point, otherPoint); });

        coordsVec.push_back(std::make_pair(polygon.points.back(), polygon.points.front()));
    }

    return std::abs(std::accumulate(coordsVec.begin(), coordsVec.end(), 0.0, getArea)) / 2.0;
}

bool isNumber(const std::string& str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool parsePoint(const std::string& token, Point& point)
{
    if (token.size() < POINT_TOKEN_MIN_LENGTH || token.front() != '(' || token.back() != ')')
    {
        return false;
    }

    std::string inner = token.substr(1, token.size() - 2);
    std::replace(inner.begin(), inner.end(), ';', ' ');
    std::istringstream iss(inner);
    return (iss >> point.x && iss >> point.y);
}

bool parseToken(std::istringstream& iss, std::vector<Point>& points, size_t count, bool isFirst = true)
{
    if (count < 3 && isFirst)
    {
        return false;
    }

    if (count < 1)
    {
        return (iss).eof();
    }
    std::string token;
    Point point;

    if (!(iss >> token) || !parsePoint(token, point))
    {
        return false;
    }

    points.push_back(point);
    return parseToken(iss, points, --count, false);
}

bool parsePolygon(std::istringstream& iss, Polygon& polygon) {
    std::string countStr;
    if (!(iss >> countStr) || !isNumber(countStr))
    {
        return false;
    }

    size_t count = static_cast<size_t>(std::stoul(countStr));
    std::vector<Point> points;
    points.reserve(count);

    if (!parseToken(iss, points, count, true))
    {
        return false;
    }

    polygon.points = std::move(points);
    return true;
}


int getIntParam(const std::string& option)
{
    if (option == "EVEN")
    {
        return EVEN;
    }
    else if (option == "ODD")
    {
        return ODD;
    }
    else if (option == "MEAN")
    {
        return MEAN;
    }
    else if (option == "AREA")
    {
        return AREA;
    }
    else if (option == "VERTEXES")
    {
        return VERTEXES;
    }
    else
    {
        return INVALID_COMMAND;
    }
}

void handleLessArea(const std::vector<Polygon>& polygons, std::istringstream& iss)
{
    if (polygons.size() < 1)
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }
    Polygon cmpPolygon;
    if (!parsePolygon(iss, cmpPolygon))
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }

    double cmpArea = calculateArea(cmpPolygon);
    size_t count = std::count_if(polygons.begin(), polygons.end(),
        [cmpArea](const Polygon& p) { return calculateArea(p) < cmpArea; });
    std::cout << count << '\n';
}


bool isInside(const Polygon& poly, const Point& p)
{
    int count = 0;
    auto checkRay = [&](const Point& a, const Point& b)
        {
            if ((a.y > p.y) != (b.y > p.y))
            {
                double atX = (b.x - a.x) * (p.y - a.y) / (b.y - a.y + 0.0) + a.x;
                if (p.x < atX)
                    ++count;
            }
        };

    std::function<void(size_t)> recurse;
    recurse = [&](size_t i)
        {
            if (i >= poly.points.size())
            {
                return;
            }
            const Point& a = poly.points[i];
            const Point& b = poly.points[(i + 1) % poly.points.size()];
            checkRay(a, b);
            recurse(i + 1);
    };
    recurse(0);

    return count % 2 == 1;
}

bool polygonsIntersect(const Polygon& a, const Polygon& b)
{
    auto hasCommonPoint = std::any_of(a.points.begin(), a.points.end(), [&](const Point& p1)
    {
        return std::any_of(b.points.begin(), b.points.end(), [&](const Point& p2)
        {
            return p1 == p2;
        });
    });

    bool insideMask = hasCommonPoint || std::any_of(b.points.begin(), b.points.end(), std::bind(isInside, a, _1));
    return insideMask;
}

void handleIntersections(const std::vector<Polygon>& polygons, std::istringstream& iss)
{
    Polygon target;
    if (polygons.empty() || !parsePolygon(iss, target))
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }

    auto isIntersecting = std::bind(polygonsIntersect, _1, std::cref(target));
    size_t count = std::count_if(polygons.begin(), polygons.end(), isIntersecting);
    std::cout << count << '\n';
}


void handleArea(const std::vector<Polygon>& polygons, std::istringstream& iss)
{
    std::string param;
    if (!(iss >> param))
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }

    auto areaAccumulator = [](double sum, const Polygon& poly)
    {
        return sum + calculateArea(poly);
    };

    if (isNumber(param))
    {
        if (std::stoi(param) < 3)
        {
            std::cout << INVALID_COMMAND_MSG;
            return;
        }
        size_t vtxCount = static_cast<size_t>(std::stoul(param));
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [vtxCount](double sum, const Polygon& poly) {
                return poly.points.size() == vtxCount ? sum + calculateArea(poly) : sum;
            });
        std::cout << std::fixed << std::setprecision(1) << total << '\n';
    }
    else
    {
        int option = getIntParam(param);

        switch (option)
        {
        case EVEN:
        {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double sum, const Polygon& poly) {
                    return poly.points.size() % 2 == 0 ? sum + calculateArea(poly) : sum;
                });
            std::cout << std::fixed << std::setprecision(1) << total << '\n';
            break;
        }

        case ODD:
        {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double sum, const Polygon& poly) {
                    return poly.points.size() % 2 != 0 ? sum + calculateArea(poly) : sum;
                });
            std::cout << std::fixed << std::setprecision(1) << total << '\n';
            break;
        }

        case MEAN:
        {
            if (polygons.empty())
            {
                std::cout << INVALID_COMMAND_MSG;
                return;
            }
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaAccumulator);
            std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << '\n';
            break;
        }

        default:
            std::cout << INVALID_COMMAND_MSG;
        }
    }
}

void handleMaxMin(const std::vector<Polygon>& polygons, const std::string& command, std::istringstream& iss)
{
    if (polygons.size() < 1)
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }
    std::string param;
    if (!(iss >> param) || polygons.empty())
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }

    int option = getIntParam(param);

    switch (option)
    {
    case AREA:
    {
        auto compareAreas = [](const Polygon& a, const Polygon& b) {
            return calculateArea(a) < calculateArea(b);
            };
        auto it = command == "MAX" ? std::max_element(polygons.begin(), polygons.end(), compareAreas)
            : std::min_element(polygons.begin(), polygons.end(), compareAreas);
        std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << '\n';
        break;
    }

    case VERTEXES:
    {
        auto compareVtxCount = [](const Polygon& a, const Polygon& b) {
            return a.points.size() < b.points.size();
            };
        auto it = command == "MAX" ? std::max_element(polygons.begin(), polygons.end(), compareVtxCount)
            : std::min_element(polygons.begin(), polygons.end(), compareVtxCount);
        std::cout << it->points.size() << '\n';
        break;
    }

    default:
        std::cout << INVALID_COMMAND_MSG;
    }
}

void handleCount(const std::vector<Polygon>& polygons, std::istringstream& iss)
{
    std::string param;
    if (!(iss >> param))
    {
        std::cout << INVALID_COMMAND_MSG;
        return;
    }

    if (isNumber(param))
    {
        if (std::stoi(param) < 3)
        {
            std::cout << INVALID_COMMAND_MSG;
            return;
        }
        size_t vtxCount = static_cast<size_t>(std::stoul(param));
        size_t count = std::count_if(polygons.begin(), polygons.end(),
            [vtxCount](const Polygon& poly) { return poly.points.size() == vtxCount; });
        std::cout << count << '\n';
        return;
    }

    int option = getIntParam(param);
    switch (option)
    {
    case EVEN:
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& poly) { return poly.points.size() % 2 == 0; });
        std::cout << count << '\n';
        break;
    }
    case ODD:
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& poly) { return poly.points.size() % 2 != 0; });
        std::cout << count << '\n';
        break;
    }
    default:
        std::cout << INVALID_COMMAND_MSG;
    }
}

void processFileInput(std::vector <Polygon>& polygons,
    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, std::istringstream&)>> handlers,
    std::unordered_map<std::string, std::function<void(const std::vector<Polygon>&, const std::string&, std::istringstream&)>> maxMinHandlers,
    std::string& line)
{
    if (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command))
        {
            std::cout << INVALID_COMMAND_MSG;
            return processFileInput(polygons, handlers, maxMinHandlers, line);
        }

        auto it = handlers.find(command);
        if (it != handlers.end())
        {
            it->second(polygons, iss);
            return processFileInput(polygons, handlers, maxMinHandlers, line);
        }

        auto maxMinIt = maxMinHandlers.find(command);
        if (maxMinIt != maxMinHandlers.end())
        {
            maxMinIt->second(polygons, command, iss);
            return processFileInput(polygons, handlers, maxMinHandlers, line);
        }
        std::cout << INVALID_COMMAND_MSG;
        processFileInput(polygons, handlers, maxMinHandlers, line);
    }
}
