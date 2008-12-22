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

	inline Point operator- (void)
	{
	    return Point (-x, -y);
	}

	inline Point operator- (const Point & b)
	{
	    return Point (x - b.x, y - b.y);
	}

	inline Point operator* (int scale)
	{
	    return Point (x * scale, y * scale);
	}

	// Compute the distance between 2 Points
	// @param p A Point
	// @return: Distance from @p to "this".
	double Distance (const Point & p);


	// Compute the tangente between this and @p
	// @param p A Point
	// @return The tangente between this and @p in radians.
	double Tangente (const Point & p) const;

    };

    bool operator!= (const Point & a, const Point & b);

};

#endif /*      __POINT_H__     */
