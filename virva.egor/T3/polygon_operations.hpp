#ifndef POLYGON_OPERATIONS_HPP
#define POLYGON_OPERATIONS_HPP

#include "polygon.hpp"
#include <functional>
#include <numeric>
#include <algorithm>

namespace vurvaa
{
    struct IsEven
    {
        bool operator()(const Polygon& p) const;
    };

    struct IsOdd
    {
        bool operator()(const Polygon& p) const;
    };

    struct HasNVertices
    {
        size_t n;

        HasNVertices(size_t num);
        bool operator()(const Polygon& p) const;
    };

    double areaEven(const std::vector<Polygon>& polygons);
    double areaOdd(const std::vector<Polygon>& polygons);
    double areaMean(const std::vector<Polygon>& polygons);
    double areaWithVertices(const std::vector<Polygon>& polygons, size_t num);

    double maxArea(const std::vector<Polygon>& polygons);
    double minArea(const std::vector<Polygon>& polygons);
    size_t maxVertices(const std::vector<Polygon>& polygons);
    size_t minVertices(const std::vector<Polygon>& polygons);

    size_t countEven(const std::vector<Polygon>& polygons);
    size_t countOdd(const std::vector<Polygon>& polygons);
    size_t countWithNVertices(const std::vector<Polygon>& polygons, size_t n);

    int maxSeq(const std::vector<Polygon>& polygons, const Polygon& pattern);
    int echo(std::vector<Polygon>& polygons, const Polygon& pattern);
}

#endif
