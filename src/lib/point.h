#ifndef __POINT_H__
#define __POINT_H__

#include <SDL/SDL_types.h>

namespace Chenilles
{

    struct Point
    {

	int x, y;

	  Point (int abs = 0, int ord = 0);

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

	// Compute the distance between 2 Points
	// @param p A Point
	// @return: | *this - p |
	inline double Distance (const Point & p);

    };

};

#endif /*      __POINT_H__     */
