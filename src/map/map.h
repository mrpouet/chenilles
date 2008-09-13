#ifndef __MAP_H__
#define __MAP_H__

#include <exception>
#include <graphics/surface.h>

class Map
{

  public:

    explicit Map (const string & xmldoc) throw (std::exception);

    inline bool isInTheVacuum (const Point & p) const
    {
	return m_ground.GetRGBA (p).GetA () == 0;
    }

    inline bool isInTheGround (const Point & p) const
    {
	return m_ground.GetRGBA (p).GetA () == 255;
    }

    inline int WidthOfWorld (void) const
    {
	return m_sky.GetWidth ();
    }

    inline int HeightOfWorld (void) const
    {
	return m_sky.GetHeight ();
    }

    inline bool IsOutOfWorldX (int x) const
    {
	return (x < 0) || (x >= WidthOfWorld ());
    }

    inline bool IsOutOfWorldY (int y) const
    {
	return (y < 0) || (y >= HeightOfWorld ());
    }

    inline bool IsOutOfWorld (const Point & pos) const
    {
	return IsOutOfWorldX (pos.x) || IsOutOfWorldY (pos.y);
    }

    void draw (void) const;

  private:

    Surface m_sky;
    Surface m_ground;
    Surface m_foreground;
    Surface m_explosion;

};

#endif
