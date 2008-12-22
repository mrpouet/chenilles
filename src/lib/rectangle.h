#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <SDL/SDL_video.h>
#include <point.h>

namespace Chenilles
{
    typedef SDL_Rect rectangle;

    // Typically Rectangle implementation is know
    struct Rectangle
    {
	Sint16 x, y;
	Uint16 w, h;

      public:
	Rectangle (Sint16 x = 0, Sint16 y = 0, Uint16 w = 0, Uint16 h = 0);

	explicit Rectangle (const rectangle * SDLRect);

        inline bool operator== (const void *ptr) const
        {
	  return this == ptr;
        }

	inline SDL_Rect GetSDLRect (void) const
	{
	    SDL_Rect r = { x, y, w, h };
	    return r;
	}

	// Checks whether or not this Rectangle 
	// contains the specified Point
	// @param p The point which to check
	// @return true if p is in this Rectangle, false otherwises.
	inline bool Contains (const Point & p) const
	{
	    return (p.x >= x) && (p.x <= (x + w))
		&& (p.y >= y) && (p.y <= (y + h));
	}

	// Compute the intersection of this and of the specified Rectangle
	// @param B The Rectangle to intersect with "this"
	// @param intersection If non-null this Rectangle'll contain
	// intersection Rectangle (between this two rects) after call.
	// @return true if a intersection exists between this two Rectangles,
	// false otherwises.
	bool Intersect (const Rectangle & B,
			Rectangle * intersection = NULL) const;

	// Translate the Top left corner of this Rectangle, by p Point.
	// @param p The Point from which this Rectangle 'll be translate.
	void Translate (const Point & p)
	{
	    x += p.x;
	    y += p.y;
	}

      private:

	void Constructor (Sint16 x, Sint16 y, Uint16 w, Uint16 h);

    };

};

#endif /*      __RECTANGLE_H__ */
