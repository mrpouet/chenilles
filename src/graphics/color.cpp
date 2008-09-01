#include <SDL/SDL_types.h>

#include "color.h"

void
Color::SetColor (Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

// Only Needed by Surface module to use geometric method
// in a object.
// Due to SDL_gfx API which uses 'Big endian' as type of endianness 

Uint32 Color::GetRGBAColor (void) const
{
  return ((Uint32)red << 24) | ((Uint32)green << 16)
    | ((Uint32)blue << 8) | (Uint32)alpha;
}
