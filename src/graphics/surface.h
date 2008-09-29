#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <string>
#include <new>
#include <SDL/SDL_video.h>

#include <tools/point.h>
#include <tools/rectangle.h>
#include <tools/base.h>

#include "color.h"

typedef SDL_Rect rectangle;

class Surface
{

  public:

    // Default constructor
    Surface ();

    // Copy constructor
    Surface (const Surface & s);

    // Create a generic RGB Surface
      Surface (const Rectangle & rect, int dept = 32,
	       Uint32 flags = SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE
	       | SDL_DOUBLEBUF);

    // Create a Surface from SDL_Surface object
    explicit Surface (SDL_Surface * surface);

    // Create a Surface from "filename" image
    explicit Surface (const string& filename);

     ~Surface ();

     // Assigment operator need by STL containers
      inline Surface& operator= (const Surface & s)
	{
	  Clone(s);
	  return *this;
	}

      // Severals Surface copied from the same source
      // share the same SDL_Surface *, so we can compare them.
      inline bool operator==(const Surface& s)
      {
	return surface == s.surface;
      }

    // Lock the current Surface.
    void Lock (void);
    // Unlock the current Surface.
    void Unlock (void);

    inline void LockFree(void) 
    {
      lockfree = true;
    }
    // Blit 'src' Surface from 'srcrect' to 'dstrect' 
    // into the current Surface. 
    void Blit (const Surface & src, rectangle * dstrect = NULL,
	       rectangle * srcrect = NULL);

    // Blit 'src' Surface to 'dstpt' Point into the current
    // Surface.
    void Blit (const Surface & src, const Point & dstpt);

    inline void UpdateRect(const Rectangle dstrect)
    {
      SDL_UpdateRect(surface, dstrect.x, dstrect.y, dstrect.w, dstrect.h);
    }

    inline void UpdateRects(int numrects, rectangle *rects)
    {
      if (numrects)
	SDL_UpdateRects(surface, numrects, rects);
    }

    // Flip the current Surface, using the double buffering
    // if the current system supports it, else does exactly the same
    // as SDL_UpdateRect(0, 0, 0, 0)
    void Flip (void);

    // Get the Color of pixel 'px' in the current Surface
    Color GetRGBA (const Point & px) const;

    // Dig the pixel 'px' by settings his alpha channel
    // to 'alpha' value.
    void Dig (const Point & px, Uint8 alpha);

    // Returns Width/Height of the current surface
    inline int GetWidth (void) const
    {
	return surface->w;
    }

    inline int GetHeight (void) const
    {
	return surface->h;
    }

    inline Uint8 GetDept (void) const
    {
	return surface->format->BytesPerPixel;
    }

    // Sets the color key, transparent in the current surface.
    void SetColorKey (const Color & key, Uint32 flag =
		      SDL_SRCCOLORKEY | SDL_SRCALPHA | SDL_RLEACCEL);

    // Replace ALL occurences of ALL colors, from 'begin'
    // (which is the pixel ID not the color), for each colors
    // found in 'colors', during 'ncolors' pixels.
    int SetColors (SDL_Color * colors, int begin, int ncolors);

    // Converts the current Surface to the PixelFormat
    // of 's' Surface.
    // 'flag' is the same that SDL_CreateRGBSurface.
    void ConvertSurface (const Surface & s, Uint32 flag =
			 SDL_HWSURFACE | SDL_RLEACCEL | SDL_DOUBLEBUF);

    void DisplayFormat (void);

    void DisplayFormatAlpha(void);

    // Set/Unset alpha blending on the current surface
    // 'flag' : SDL_SRCALPHA, SDL_RLEACCEL.
    // Note: if 'flag' is 0, alpha blending is disable.
    void SetAlpha (Uint8 alpha, Uint32 flag = SDL_SRCALPHA | SDL_RLEACCEL);

    inline Uint8 GetAlpha (void) const
    {
	return surface->format->alpha;
    }
    // Fill the given rectangle with the given Color
    // and clip it to the current surface.
    void FillRect (const Color & c, rectangle * destrect = NULL);

    void FillRect (const Point & p1, const Point & p2, const Color & c);

    // Sets the clipping rectangle for the current surface
    // in a blit.
    bool SetClipRect (const rectangle * dstrect);

    // Draw Anti Aliased Line from 'begin' to 'end' with the given Color
    // on the current Surface.
    void AALine (const Point & begin, const Point & end, const Color & c);

    // Draw rectangle borders with the following Color
    // on the current Surface.
    void AARect (const Point & p1, const Point & p2, const Color & c);

    // Draw AACircle with following Color (border color)
    // on current Surface
    void AACircle (const Point & center, Sint16 r, const Color & c);

    void AAFillCircle (const Point & center, Sint16 r, const Color & c);

  private:

    SDL_Surface * surface;
    bool lockfree;

    inline Uint32 MapRGB (const Color & c) const
    {
	return SDL_MapRGB (surface->format, c.GetR (), c.GetG (), c.GetB ());
    }

    inline Uint8 *GetPixel (const Point & px) const
    {
	return static_cast < Uint8 * >(surface->pixels)
	    + surface->pitch * px.y + px.x * surface->format->BytesPerPixel;
    }

    void Clone (const Surface & s);

    void validPtr (void) const;
};

#endif /*      __SURFACE_H__   */
