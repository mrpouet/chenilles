#include <math.h>

#include "point.h"

Point::Point()
{
  x = y = 0;
}

Point::Point(int abs, int ord):
  x(abs), y(ord)
{}

inline double Point::Distance(const Point &p)
{
  double dist = (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
  return sqrt(dist);
}

bool
operator!=(const Point& a, const Point& b)
{
  return !(a == b);
}
