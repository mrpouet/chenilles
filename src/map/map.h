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

    //TODO: Remove @param in the future to replace it directly
    // by the screen Surface
    void Draw (Surface & s) const;

  private:

    Surface m_sky;
    Surface m_ground;
    Surface m_foreground;
    Surface m_explosion;

};

#endif
