#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <surface.h>
#include <private/sprite.h>

namespace Chenilles
{

    class Animation
    {
      public:

	//FIXME: rewrite this module unless dest Surface
	// we need to use Camera in intern.
	Animation (const Surface & dest);

	// Start motion
	// If Animation doesn't contain any Sprite (absurde)
	// this method do nothing.
	void Start (void);

	// Refresh to next Sprite if his "time delay" was finished.
	// If motion isn't started this method blit the first frame 
	// to "dest" Surface.
	void Update (void);

	// Stop motion
	inline void Stop (bool visible = true)
	{
	    m_active = false;
	    m_visible = visible;
	    m_it = m_frames.begin ();
	}

	inline void setXLocation (Sint16 x)
	{
	    m_location.x = x;
	}

	inline void setYLocation (Sint16 y)
	{
	    m_location.y = y;
	}

	inline void setLocation (const Point & p)
	{
	    m_location = p;
	}

	inline int GetWidth (void) const
	{
	    return m_it->m_surface.GetWidth ();
	}

	inline int GetHeight (void) const
	{
	    return m_it->m_surface.GetHeight ();
	}

	// Add a new Sprite in the current Animation, 
	// with the fallowing time delay.
	void newSprite (const char *path, Uint32 delay = 120);

      private:

	typedef std::vector < Sprite > SpriteArray;

	bool m_visible;

	bool m_active;

	Uint32 m_last_update_time;

	//FIXME: Remove this member later 
	// (replace it by screen or map Surface directly)
	Surface m_dest;

	Point m_location;

	SpriteArray m_frames;

	SpriteArray::iterator m_it;

    };

};

#endif /*      __ANIMATION_H__    */
