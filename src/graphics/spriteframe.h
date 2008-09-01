#ifndef __SPRITEFRAME_H__
#define __SPRITEFRAME_H__

#include "surface.h"

class Sprite;

class SpriteFrame
{

  private:
    friend class Sprite;
    Uint32 time_delay;
    Surface surface;

  public:
    SpriteFrame (const Surface& s, Uint32 delay = 100);
};

#endif /*      __SPRITEFRAME_H__       */
