#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <tools/base.h>

#include "surface.h"

class Sprite
{
  private:
  Sprite (const Surface & surface, Uint32 delay = 120):
    m_surface (surface), m_delay (delay)
    {}

    Surface m_surface;
    Uint32 m_delay;
    friend class Animation;
};

class Animation
{
  public:

    Animation (const Surface & dest);

    // Start motion
    // If Animation doesn't contain any Sprite (absurde)
    // this method do nothing.
    void Start (void);

    // Refresh to next Sprite if his delay of the current
    // is finished.
    // If motion isn't started this method blit the first frame of
    // the motion to "dest" Surface.
    void Update (void);

    // Stop motion
    inline void Stop (void)
    {
	m_active = false;
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

    // Add a new Sprite in the current Animation, 
    // with the fallowing time delay.
    inline void newSprite (const char *path, Uint32 delay = 120)
    {
	m_frames.push_back (Sprite (Surface (path), delay));
    }

  private:

    bool m_active;
    Uint32 m_last_update_time;
    Surface m_dest;
    Point m_location;
    vector < Sprite > m_frames;
    vector < Sprite >::iterator m_it;

};

#endif /*      __ANIMATION_H__    */
