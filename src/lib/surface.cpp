#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <surface.h>
#include <game_exception.h>
#include <tools/base.h>
#include <cstdio>
namespace
{

    class SDLException:public GameException
    {
      public:

	SDLException () throw ():GameException (SDL_GetError ())
        {}

	~SDLException () throw () {};

	const char *what (void) const throw ()
	{
	    return "SDL exception (SDLException)";
	}

    };

    inline void validPtr (void *ptr)
    {
	if (!ptr)
	    throw SDLException ();
    }

};

Surface::Surface ()
{
    surface = NULL;
}

// Share the same surface pointer,
// so we need to use the refcount SDL_Surface counter,
//  to free
void
Surface::Clone (const Surface & s)
{
    surface = s.surface;

    if (surface)
	surface->refcount++;
}

Surface::Surface (const Surface & s)
{
    Clone (s);
}

Surface::Surface (SDL_Surface * s)
{
    validPtr (s);
    surface = s;
}

Surface Surface::CreateRGB (const Rectangle & rect, int dept, Uint32 flags)
{
    Surface surface = Surface (SDL_CreateRGBSurface (flags, rect.w, rect.h, 
						     dept, 0, 0, 0, 0));
    return surface;
}

Surface Surface::CreateFromFile (const std::string & path)
{
    Surface surface = Surface (IMG_Load (path.c_str ()));

    return surface;
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
	throw SDLException ();
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
	throw SDLException ();
}

void
Surface::Blit (const Surface & src, const Point & dstpt)
{
    rectangle dstrect = { dstpt.x, dstpt.y, 0, 0 };
    Blit (src, &dstrect, NULL);
}

void
Surface::Resize (int width, int height)
{
    SDL_Surface *tmp = SDL_CreateRGBSurface (surface->flags, width, height,
					     surface->format->BitsPerPixel,
					     0, 0, 0, 0);
    validPtr (tmp);

    SDL_BlitSurface (surface, NULL, tmp, NULL);
    SDL_FreeSurface (surface);
    surface = tmp;

}

void
Surface::Flip (void)
{
    if (SDL_Flip (surface))
	throw SDLException ();
}

// Thanks for this very great example taken from SDLwiki
Uint32 Surface::GetPixel(const Point& px) const
{
  int bpp = surface->format->BytesPerPixel;
  Uint8 *pixel = (Uint8 *)surface->pixels + px.y * surface->pitch + px.x * bpp;

  switch(bpp) 
    {
    case 1:
      return *pixel;
    case 2:
      return *(Uint16 *)pixel;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
      else
	return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
    case 4:
      return *(Uint32 *)pixel;
    default:
      SDL_SetError("Unknow BytesPerPixel (bpp) size");
      throw SDLException();
    }
  return 0; /* fix gcc warning */
}

Color
Surface::GetPixColor (const Point & px) const
{
  Color c;
  Uint32 pixel = GetPixel(px);
  Uint8 r,g,b, a = 255;

  if (surface->format->BytesPerPixel == 4)
    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
  else if (surface->format->BytesPerPixel == 3)
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
  c.SetColor(r, g, b, a);
  return c;
}

void
Surface::Dig (const Point & px, Uint8 alpha)
{
  SDL_SetError("Surface::Dig not implemented yet");
  throw SDLException();
}

void
Surface::SetColorKey (const Color & key, Uint32 flag)
{
    if (SDL_SetColorKey (surface, flag, MapRGB (key)))
	throw SDLException ();
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
    SDL_FreeSurface (surface);
    surface = tmp;
    validPtr (surface);
}

void
Surface::vfunc_DisplayFormat (SDL_Surface * (*func) (SDL_Surface *))
{
    SDL_Surface *tmp = NULL;

    tmp = func (surface);
    validPtr(tmp);
    SDL_FreeSurface (surface);
    surface = tmp;
}

void
Surface::SetAlpha (Uint8 alpha, Uint32 flag)
{
    if (SDL_SetAlpha (surface, flag, alpha))
	throw SDLException ();
}

void
Surface::FillRect (const Color & c, rectangle * destrect)
{
    if (SDL_FillRect (surface, destrect, MapRGB (c)))
	throw SDLException ();
}

void
Surface::FillRect (const Point & p1, const Point & p2, const Color & c)
{
    if (boxColor (surface, p1.x, p1.y, p2.x, p2.y, c.GetRGBAColor ()))
	throw SDLException ();
}

bool
Surface::SetClipRect (const rectangle * dstrect)
{
    return SDL_SetClipRect (surface, dstrect);
}

void
Surface::AALine (const Point & begin, const Point & end, const Color & c)
{
    if (aalineColor
	(surface, begin.x, begin.y, end.x, end.y, c.GetRGBAColor ()))
	throw SDLException ();
}

void
Surface::AARect (const Point & p1, const Point & p2, const Color & c)
{
    if (rectangleColor (surface, p1.x, p1.y, p2.x, p2.y, c.GetRGBAColor ()))
	throw SDLException ();
}

void
Surface::AACircle (const Point & center, Sint16 r, const Color & c)
{
    if (aacircleColor (surface, center.x, center.y, r, c.GetRGBAColor ()))
	throw SDLException ();
}

void
Surface::AAFillCircle (const Point & center, Sint16 r, const Color & c)
{
    if (filledCircleColor (surface, center.x, center.y, r, c.GetRGBAColor ()))
	throw SDLException ();
}
