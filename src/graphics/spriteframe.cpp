#include "spriteframe.h"

SpriteFrame::SpriteFrame (const Surface& s, Uint32 delay):
  surface (s)
{
  time_delay = delay;
}
