#include <game/timer.h>
#include <tools/debug.h>

#include "sprite.h"

Sprite::Sprite()
{
  draw_first = false;
  finish = true;
  current_frame = 0;
  last_update_time = 0;
}

Sprite::Sprite (const Surface& dst, bool draw):
  dst_surface (dst)
{
    draw_first = draw;
    finish = true;
    current_frame = 0;
    last_update_time = 0;
}

void
Sprite::Start (void)
{
  finish = false;
  current_frame = 0;
  last_update_time = Timer::GetInstance()->Read();
}

void
Sprite::Update (void)
{
  int id = current_frame % frames.size();
  SpriteFrame& tmp = frames[id];
  Uint32 current_time;


  if (finish && !draw_first)
    return;

  dst_surface.Blit (tmp.surface, current_location);
      
  MSG_DEBUG ("graphics.sprite",
	     "Blit frames[%lu] to dest surface at (%d, %d)\n", id, 
	     current_location.x, current_location.y);

  if (current_frame == frames.size ())
    {
      MSG_DEBUG ("graphics.sprite",
		 "End of frames container, SpriteMotion is finish\n");
      finish = true;
      return;
    }

  current_time = Timer::GetInstance()->Read();

  if(finish || current_time < (last_update_time + tmp.time_delay))
    return;
  last_update_time = current_time;
  current_frame++;

}

void
Sprite::NewFrame (const Surface& s, Uint32 delay)
{
  frames.push_back (SpriteFrame (s, delay));
}

void
Sprite::NewFrame (const char *gfx_path, Uint32 delay)
{
  frames.push_back (SpriteFrame (Surface(gfx_path), delay));
}
