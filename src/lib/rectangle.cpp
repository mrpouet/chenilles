#include <rectangle.h>
#include <tools/base.h>

namespace
{
  inline void ComputeInterRect(Sint16 Amin, Sint16 Amax, 
			       Sint16 Bmin, Sint16 Bmax, 
			       Sint16 & inter_coord, 
			       Uint16 &inter_spacing)
  {
    Amax = (Amax < Bmax) ? Amax : Bmax;
    inter_coord = (Amin > Bmin) ? Amin : Bmin;
    Amax -= inter_coord;
    inter_spacing = (Amax > 0) ? Amax : 0;
  }
};

void
Rectangle::Constructor (Sint16 x, Sint16 y, Uint16 w, Uint16 h)
{
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h; 
}

Rectangle::Rectangle (Sint16 x, Sint16 y, Uint16 w, Uint16 h)
{
  Constructor(x, y, w, h);  
}

Rectangle::Rectangle (const rectangle *SDLRect)
{
  x = SDLRect->x;
  y = SDLRect->y;
  w = SDLRect->w;
  h = SDLRect->h;
}

bool
Rectangle::Intersect (const Rectangle& B, Rectangle *intersection) const
{
  Rectangle inter;
  // A is "this"
  Sint16 Amax = x + w;
  Sint16 Bmax = B.x + B.w;

  // Computing Horizontal intersection
  ComputeInterRect(x, Amax, B.x, Bmax, inter.x, inter.w);

  // Computing Vertical intersection
  Bmax = B.y + B.h;
  Amax = y + h;

  ComputeInterRect(y, Amax, B.y, Bmax, inter.y, inter.h);

  if (intersection)
    *intersection = inter;
  return (inter.w && inter.h);
}

