#ifndef __COLOR_H__
#define __COLOR_H__

#include <SDL/SDL_video.h>

class Color
{

  public:
    Color (Uint8 r, Uint8 g, Uint8 b, Uint8 a = 254)
    {
	SetColor (r, g, b, a);
    }

    Color (const SDL_Color & c)
    {
	SetColor (c.r, c.g, c.b, c.unused);
    }

    bool operator== (const Color& c) const
    {
	return (red == c.red) && (green == c.green)
	    && (blue == c.blue) && (alpha == c.alpha);
    }

    inline Uint8 GetR (void) const
    {
	return red;
    }
    inline Uint8 GetG (void) const
    {
	return green;
    }
    inline Uint8 GetB (void) const
    {
	return blue;
    }

    inline Uint8 GetA (void) const
    {
	return alpha;
    };

    void SetColor (Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 254);
    void SetColor (Uint32 key);

    Uint32 GetRGBAColor (void) const;

    inline SDL_Color GetSDLColor (void) const
    {
	SDL_Color c = { red, green, blue, alpha };
	return c;
    }

  private:
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;

};

#endif /*      __COLOR_H__     */
