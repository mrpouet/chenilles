#ifndef __SPRITE_H__
#define __SPRITE_H__

#ifdef __ANIMATION_H__

namespace Chenilles
{

    class Sprite
    {

      private:
	Sprite (const char *path, Uint32 delay)
	{
	    m_surface = Surface::CreateFromFile (path);
	    m_surface.DisplayFormatAlpha ();
	    m_delay = delay;
	}

	Surface m_surface;

	Uint32 m_delay;

	friend class Animation;
    };

    class SpriteCache
    {
      private:
	SpriteCache (Uint16 width, Uint16 height)
	{
	    m_cache = Surface::CreateRGB (Rectangle (0, 0, width, height));
	    m_cache.DisplayFormatAlpha();
	}

	Surface m_cache;

	Rectangle m_geometry;

	friend class Animation;
    };

};

#else
#error "__FILE__ is a private header file, don't use it !"
#endif

#endif
