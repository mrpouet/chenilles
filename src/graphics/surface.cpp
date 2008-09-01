#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

#include "surface.h"

Surface::Surface ()
{
    surface = NULL;
}

void
Surface::Clone (const Surface & s)
{
    surface = s.surface;
    if (surface)
	surface->refcount++;
}

// Share the same surface
// so we need to use refcount to free
// the surface ONLY is the last object which 
// uses it was destroyed.
// Note: Same theory in assigment operator.
Surface::Surface (const Surface & s)
{
    Clone (s);
}

//FIXME: Replace ALL std:exception later by SDLException
// (for example)

void
Surface::validPtr (void) const throw(bad_alloc)
{
    if (!surface)
	throw bad_alloc ();
}

Surface::Surface (const Rectangle & rect, int dept, Uint32 flag)
{
    surface = SDL_CreateRGBSurface (flag, rect.w, rect.h, dept, 0, 0, 0, 0);
    validPtr ();
}

Surface::Surface (SDL_Surface * s)
{
    surface = s;
    validPtr ();
}

Surface::Surface (const char *filename)
{
    surface = IMG_Load (filename);
    validPtr ();
}

Surface & Surface::operator= (const Surface & s)
{
    Clone (s);
    return *this;
}

Surface::~Surface ()
{
    if (surface)
	SDL_FreeSurface (surface);
}

void
Surface::Lock ()
{
    if (SDL_LockSurface (surface))
	throw exception ();
}

void
Surface::Unlock ()
{
    SDL_UnlockSurface (surface);
}

void
Surface::Blit (const Surface & src, rectangle * dstrect, rectangle * srcrect)
{
    if (SDL_BlitSurface (src.surface, srcrect, surface, dstrect))
	throw exception ();
}

void
Surface::Blit (const Surface & src, const Point & dstpt)
{
    rectangle dstrect = { dstpt.x, dstpt.y, 0, 0 };
    Blit (src, &dstrect, NULL);
}

void
Surface::Flip (void)
{
    if (SDL_Flip (surface))
	throw exception ();
}

/*
Color Surface::GetRGBA (const Point & px) const
{
    Uint8 r,g,b,a;  
    Uint32 pixel = *static_cast<Uint32 *>(GetPixel(px));

    SDL_GetRGBA (pixel, surface->format, &r, &g, &b, &a);

    return Color (r, g, b, a);
}
*/

void 
Surface::Dig (const Point& px, Uint8 alpha)
{
  Uint8 *pixel = NULL;

  // This is not a RGBA surface
  if (surface->format->BytesPerPixel != 4)
    return;
  pixel = GetPixel(px);
  pixel[3] = alpha;
}

void
Surface::SetColorKey (const Color & key, Uint32 flag)
{
    if (SDL_SetColorKey (surface, flag, MapRGB (key)))
	throw exception ();
}

int
Surface::SetColors (SDL_Color * colors, int begin, int ncolors)
{
    return SDL_SetColors (surface, colors, begin, ncolors);
}

void
Surface::ConvertSurface (const Surface & s, Uint32 flag)
{
    SDL_Surface *tmp = NULL;

    tmp = SDL_ConvertSurface (surface, s.surface->format, flag);
    validPtr ();
    SDL_FreeSurface (surface);
    surface = tmp;
}

void
Surface::DisplayFormat (void)
{
    SDL_Surface *tmp = NULL;

    tmp = SDL_DisplayFormat (surface);
    validPtr ();
    SDL_FreeSurface (surface);
    surface = tmp;
}

void
Surface::SetAlpha (Uint8 alpha, Uint32 flag)
{
    if (SDL_SetAlpha (surface, flag, alpha))
	throw exception ();
}

void
Surface::FillRect (rectangle * dstrect, const Color & c)
{
    if (SDL_FillRect (surface, dstrect, MapRGB (c)))
	throw exception ();
}

void
Surface::FillRect (const Point & p1, const Point & p2, const Color & c)
{
    if (boxColor (surface, p1.x, p1.y, p2.x, p2.y, c.GetRGBAColor ()))
	throw exception ();
}

bool
Surface::SetClipRect (const rectangle * dstrect)
{
    return SDL_SetClipRect (surface, dstrect);
}

//TODO: Really needed ? (slow...)
void
Surface::rotozoom (double angle, double zoom, int smooth)
{
    SDL_Surface *tmp = NULL;

    tmp = rotozoomSurface (surface, angle, zoom, smooth);

    validPtr ();
    SDL_FreeSurface (surface);
    surface = tmp;
}

void
Surface::AALine (const Point & begin, const Point & end, const Color & c)
{
    if (aalineColor
	(surface, begin.x, begin.y, end.x, end.y, c.GetRGBAColor ()))
	throw exception ();
}

void
Surface::AARect (const Point & p1, const Point & p2, const Color & c)
{
    if (rectangleColor (surface, p1.x, p1.y, p2.x, p2.y, c.GetRGBAColor ()))
	throw exception ();
}

void
Surface::AACircle (const Point & center, Sint16 r, const Color & c)
{
    if (aacircleColor (surface, center.x, center.y, r, c.GetRGBAColor ()))
	throw exception ();
}

void
Surface::AAFillCircle (const Point & center, Sint16 r, const Color & c)
{
    if (filledCircleColor (surface, center.x, center.y, r, c.GetRGBAColor ()))
	throw exception ();
}
