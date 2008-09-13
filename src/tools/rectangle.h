#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <SDL/SDL_video.h>
#include <tools/point.h>

class Rectangle
{
  public:
    Sint16 x, y;
    Uint16 w, h;

  public:
      Rectangle (Sint16 x = 0, Sint16 y = 0, Uint16 w = 0, Uint16 h = 0);

    inline SDL_Rect GetSDLRect (void) const
    {
	SDL_Rect r = { x, y, w, h };
	  return r;
    }

    inline bool IsInRect (const Point & at) const
    {
	return (at.x >= x) && (at.x <= (x + w))
	    && (at.y >= y) && (at.y <= (y + h));
    }

};

#endif /*      __RECTANGLE_H__ */
