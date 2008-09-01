#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <vector>
#include <tools/base.h>

#include "surface.h"

class Sprite
{
  private:

    // Draw first frame if sprite motion is disabled
    bool draw_first;
    bool finish;
    Uint32 last_update_time;
    size_t current_frame;
    Surface dst_surface;
    Point current_location;
    vector <SpriteFrame> frames;

  public:
    Sprite();

    Sprite (const Surface& dst, bool draw = true);

     // Get Width of the current frame in the motion
     inline int GetWidth(void) const {
       return frames[current_frame].surface.GetWidth();
     }

     // Get Height of the current frame in the motion.
     inline int GetHeight(void) const {
       return frames[current_frame].surface.GetHeight();
     }

    // Starts motion
    void Start (void);

    // Refresh to next SpriteFrame if the delay of the current
    // frame is finished.
    // If motion isn't started this method blit the first frame of
    // the motion ONLY if draw_first was defined (as default)
    void Update (void);


    inline bool IsFinish(void) const {
      return finish;
    }

    // Only necessary to force to stop during motion
    inline void Stop (void) { 
      finish = true;
    }

    inline void SetXLocation(Sint16 x) {
      current_location.x = x;
    }

    inline void SetYLocation(Sint16 y) {
      current_location.y = y;
    }

    inline void SetLocation(const Point& p) {
      current_location.x = p.x;
      current_location.y = p.y;
    }

    // Add a new frame in current Sprite with the fallowing time delay.
    void NewFrame (const Surface& s, Uint32 delay = 120);

    void NewFrame (const char *gfx_path, Uint32 delay = 120);
};

#endif /*      __SPRITE_H__    */
