#include <cmath>
#include <cstdio>
#include <point.h>
#include <tools/base.h>

Point::Point (int abs, int ord)
{
    x = abs;
    y = ord;
}

inline double
Point::Distance (const Point & p)
{
    double dist = (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    return sqrt (dist);
}

// Note:
// int x,y;
// (double)x / y => double.
double
Point::Tangente (const Point & p) const
{
    return atan ((double) (p.y - y) / (p.x - x));
}

bool Chenilles::operator!= (const Point & a, const Point & b)
{
    return !(a == b);
}
