#include "polygon.hpp"
#include <cmath>

namespace
{
    struct DelimetrIO
    {
        char exp;
    };

    std::istream& operator>>(std::istream& in, DelimetrIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char ch = 0;
        in >> ch;
        if (in && (ch != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }
}

bool vurvaa::operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool vurvaa::operator==(const Polygon& lhs, const Polygon& rhs)
{
    return lhs.points == rhs.points;
}

double vurvaa::polygonArea(const Polygon& poly)
{
    const size_t n = poly.points.size();
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        size_t j = (i + 1) % n;
        sum += poly.points[i].x * poly.points[j].y
            - poly.points[j].x * poly.points[i].y;
    }
    return std::abs(sum) * 0.5;
}

std::istream& vurvaa::operator>>(std::istream& in, Polygon& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    Polygon input{};
    {
        using sep = DelimetrIO;
        size_t num = 0;
        in >> num;
        if (num < 3)
        {
            in.setstate(std::ios::failbit);
        }

        for (size_t i = 0; i < num; ++i)
        {
            if (in.peek() == EOF || in.peek() == '\n')
            {
                in.setstate(std::ios::failbit);
            }

            Point temp{};
            in >> sep{ '(' } >> temp.x;
            in >> sep{ ';' } >> temp.y;
            in >> sep{ ')' };
            input.points.push_back(temp);
        }

        int c = in.peek();
        if (c != '\n' && c != EOF)
        {
            in.setstate(std::ios::failbit);
        }
    }

    if (in)
    {
        dest = input;
    }

    return in;
}
