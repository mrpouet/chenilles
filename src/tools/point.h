#ifndef __POINT_H__
#define __POINT_H__

#include <SDL/SDL_types.h>

class Point
{

  public:
    // Typicaly Cartesien Point implements is know
    int x, y;
      Point ();
      Point (int abs, int ord);

    inline Point & operator+= (const Point & p)
    {
	x += p.x;
	y += p.y;

	return *this;
    }

    inline bool operator== (const Point & b) const
    {
	return (x == b.x) && (y == b.y);
    }

    inline Point operator+ (const Point & b)
    {
	return Point (x + b.x, y + b.y);
    }

    // Distance between p and current object
    inline double Distance (const Point & p);

};

#endif /*      __POINT_H__     */
