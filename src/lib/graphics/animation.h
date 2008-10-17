#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <tools/base.h>

#include "surface.h"

class Sprite
{
  private:
  Sprite (const char *path, Uint32 delay):
    m_surface (path), m_delay (delay)
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

    // Refresh to next Sprite if his "time delay" was finished.
    // If motion isn't started this method blitd the first frame 
    // to "dest" Surface.
    void Update (void);

    // Stop motion
    inline void Stop (bool visible = true)
    {
	m_active  = false;
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

    inline int GetWidth(void) const
    {
      return m_it->m_surface.GetWidth();
    }

    inline int GetHeight(void) const
    {
      return m_it->m_surface.GetHeight();
    }

    // Add a new Sprite in the current Animation, 
    // with the fallowing time delay.
    inline void newSprite (const char *path, Uint32 delay = 120)
    {
	m_frames.push_back (Sprite(path, delay));
    }

  private:
    bool m_visible;
    bool m_active;
    Uint32 m_last_update_time;
    //FIXME: Remove this member later 
    // (replace it by screen or map Surface directly)
    Surface m_dest;
    Point m_location;
    vector<Sprite> m_frames;
    vector<Sprite>::iterator m_it;

};

#endif /*      __ANIMATION_H__    */
