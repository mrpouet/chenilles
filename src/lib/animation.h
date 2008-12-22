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

        Animation (const std::string& xmldoc);

	// Start motion
	// If Animation doesn't contain any Sprite (absurde)
	// this method do nothing.
	void start (void);

	// Refresh to next Sprite if his "time delay" was finished.
	// If motion isn't started this method blit the first frame 
	// to "dest" Surface.
	void update (bool redraw_with_cache = false);

	// Stop motion
	inline void stop (bool visible = true)
	{
	    m_active = false;
	    m_visible = visible;
	    m_current_frame = m_frames.begin ();
	}

	inline void forceDraw(void)
	{
	  m_force_draw = true;
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

	inline const Point& getLocation (void) const
	{
	  return m_location;
	}

	inline int getWidth (void) const
	{
	    return m_current_frame->m_surface.GetWidth ();
	}

	inline int getHeight (void) const
	{
	    return m_current_frame->m_surface.GetHeight ();
	}

      private:

	typedef std::vector < Sprite > SpriteArray;

	bool m_visible;

	bool m_active;

	bool m_force_draw;

	Uint32 m_last_update_time;

	Point m_location;

	SpriteArray m_frames;

	SpriteArray::iterator m_current_frame;

	SpriteCache m_sprite_cache;

    };

};

#endif /*      __ANIMATION_H__    */
