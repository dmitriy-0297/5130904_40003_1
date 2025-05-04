#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

namespace vurvaa
{
    struct Point
    {
        int x;
        int y;
    };

    struct Polygon
    {
        std::vector<Point> points;
    };

    bool operator==(const Point& lhs, const Point& rhs);
    bool operator==(const Polygon& lhs, const Polygon& rhs);
    double polygonArea(const Polygon& poly);

    std::istream& operator>>(std::istream& in, Polygon& dest);
}

#endif
