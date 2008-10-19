#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <string>
#include <new>
#include <SDL/SDL_video.h>

#include <point.h>
#include <rectangle.h>
#include <color.h>

namespace Chenilles
{
    typedef SDL_Rect rectangle;

    class Surface
    {

      public:

	Surface ();

	Surface (const Surface & s);

	// Instance a Surface from SDL_Surface object
	// (rarely used).
	explicit Surface (SDL_Surface * surface);

	// Create a empty RGB/RGBA Surface at the following format
	// @throw exception
	// @param rect  The dimension Rectangle
	// @param dept  The dept of the Surface.
	// @param flags The Surface SDL flags
	// @return A newly instancied Surface
	static Surface CreateRGB (const Rectangle & rect, int dept = 32,
				  Uint32 flags = SDL_HWSURFACE | SDL_HWACCEL
				  | SDL_HWPALETTE | SDL_DOUBLEBUF);

	// Create a Surface from a image.
	// All known image format are autorized.
	// @throw exception
	// @param path The absolute path of the image to load
	// @return A newly instancied Surface.
	static Surface CreateFromFile (const std::string & path);

	 ~Surface ();

	inline Surface & operator= (const Surface & s)
	{
	    Clone (s);
	    return *this;
	}

	// Copied Surfaces share the same SDL_Surface pointer, 
	// so we can compare them (using arithmetic pointer).
	// @return true if the two Surfaces are equals, false otherwises.
	inline bool operator== (const Surface & s)
	{
	    return surface == s.surface;
	}

	// Lock the current Surface.
	void Lock (void);

	// Unlock the current Surface.
	void Unlock (void);

	// Blit 'src' Surface from 'srcrect' to 'dstrect' 
	// into the current Surface.
	// @param src The source Surface which will be blit
	// @param dstrect The Rectangle destination
	// @param srcrect The Rectangle source (of src Surface).
	void Blit (const Surface & src, rectangle * dstrect = NULL,
		   rectangle * srcrect = NULL);

	// Blit 'src' Surface to 'dstpt' Point into the current
	// Surface.
	// @param src The source Surface
	// @param dstpt The top-left-corner of src Surface when
	// blit was done.
	void Blit (const Surface & src, const Point & dstpt);

	// Refresh the following rectangle on Surface
	// @param dstrect The rectangle to refresh
	inline void UpdateRect (const rectangle * dstrect)
	{
	    SDL_UpdateRect (surface, dstrect->x, dstrect->y, dstrect->w,
			    dstrect->h);
	}

	// Same as last method except Rectangle type
	// (Chenilles::Rectangle)
	inline void UpdateRect (const Rectangle & dstrect)
	{
	    rectangle r = dstrect.GetSDLRect ();
	    UpdateRect (&r);
	}

	// Refresh followings rectangles on the current Surface
	// @param numrects The number of rectangle contained in rects
	// @param rects a pointer from to rectangles to refresh
	inline void UpdateRects (int numrects, rectangle * rects)
	{
	    if (numrects)
		SDL_UpdateRects (surface, numrects, rects);
	}

	// Resize the current Surface to width x height
	// @param width The new width
	// @param height The new height
	void Resize (int width, int height);

	// Flip the current Surface, using the double buffering
	// if the current system supports it, else does exactly the same
	// as SDL_UpdateRect(0, 0, 0, 0)
	void Flip (void);

	// Get the Color of the following pixel from  the current Surface.
	// @param px The pixel coordinates in cartesien plan
	// @return The Color of this pixel from the current Surface
	Color GetPixColor (const Point & px) const;

	// 'Dig' the following pixel on the current Surface,
	// by change it alpha value to 'alpha'
	// @param px The pixel coordinates
	// @param alpha The new alpha value of 'px' pixel.
	void Dig (const Point & px, Uint8 alpha);

	// Get Width of the current Surface.
	// Read only.
	// @return The Surface width
	inline int GetWidth (void) const
	{
	    return surface->w;
	}

	// Get Height of the current Surface.
	// Read only.
	// @return The Surface height
	inline int GetHeight (void) const
	{
	    return surface->h;
	}

	// Get Dept of the current Surface
	// Read only.
	// @return The Surface dept
	inline Uint8 GetDept (void) const
	{
	    return surface->format->BytesPerPixel;
	}

	// Sets all occurences of color key 'key', 
	// transparent in the current surface.
	// @param key The color key to replace
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

	// Explicit convertion to Display PixelsFormat
	inline void DisplayFormat (void)
	{
	  vfunc_DisplayFormat(SDL_DisplayFormat);
	}

	// Explicit convertion to Display PixelsFormat Alpha
	inline void DisplayFormatAlpha (void)
	{
	  vfunc_DisplayFormat(SDL_DisplayFormatAlpha);
	}

	// Set/Unset alpha blending on the current surface
	// 'flag' : SDL_SRCALPHA, SDL_RLEACCEL.
	// Note: if 'flag' is 0, alpha blending is disable.
	void SetAlpha (Uint8 alpha, Uint32 flag =
		       SDL_SRCALPHA | SDL_RLEACCEL);

	// Get the alpha-surface value.
	// Read only.
	// @return The alpha value of the current Surface.
	inline Uint8 GetAlpha (void) const
	{
	    return surface->format->alpha;
	}

	// Fill the given rectangle with the given Color
	// and clip it to the current surface.
	// @param c The fill color
	// @param destrect The rectangle to fill on the current Surface
	void FillRect (const Color & c, rectangle * destrect = NULL);

	void FillRect (const Point & p1, const Point & p2, const Color & c);

	// Sets the clipping rectangle for the current surface,
	// in a blit.
	bool SetClipRect (const rectangle * dstrect);

	// Draw Anti Aliased Line from 'begin' to 'end' with the given Color
	// on the current Surface.
	void AALine (const Point & begin, const Point & end, const Color & c);

	// Draw rectangle borders with the following Color
	// on the current Surface.
	void AARect (const Point & p1, const Point & p2, const Color & c);

	// Draw AACircle borders with following Color.
	// @param center The circle center Point
	// @param r The circle radius
	// @param c The borders colors.
	void AACircle (const Point & center, Sint16 r, const Color & c);

	// Same as last method except than circle is fill,
	// with the following color.
	void AAFillCircle (const Point & center, Sint16 r, const Color & c);

	// Accessor to underlying C SDL object
	// In Read only.
	inline const SDL_Surface *cobj (void) const
	{
	    return surface;
	}

	// Accessor/mutator to underlying C SDL object
	// in RW.
	inline SDL_Surface *cobj (void)
	{
	    return surface;
	}

      private:

	SDL_Surface * surface;

	inline Uint32 MapRGB (const Color & c) const
	{
	    return SDL_MapRGB (surface->format, c.GetR (), c.GetG (),
			       c.GetB ());
	}

	inline Uint8 *GetPixel (const Point & px) const
	{
	    return static_cast < Uint8 * >(surface->pixels)
		+ surface->pitch * px.y +
		px.x * surface->format->BytesPerPixel;
	}

	void Clone (const Surface & s);

	void vfunc_DisplayFormat(SDL_Surface* (*func) (SDL_Surface *));

    };

};

#endif /*      __SURFACE_H__   */
