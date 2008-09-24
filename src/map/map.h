#ifndef __MAP_H__
#define __MAP_H__

#include <exception>
#include <vector>
#include <graphics/surface.h>

class Map
{

  public:

    explicit Map (const string & xmldoc) throw (std::exception);
   
    inline bool isInTheVacuum (const Point & p) const
    {
	return m_layers[m_main_id].GetRGBA (p).GetA () == 0;
    }

    inline bool isInTheGround (const Point & p) const
    {
	return m_layers[m_main_id].GetRGBA (p).GetA () == 255;
    }
   
    inline int WidthOfWorld (void) const
    {
	return m_layers[0].GetWidth ();
    }

    inline int HeightOfWorld (void) const
    {
	return m_layers[0].GetHeight ();
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

    void draw (void);

  private:

    vector<Surface> m_layers;
    unsigned int m_main_id;
    unsigned int m_expl_id;
    bool m_init_draw;

 protected:
    Map(){};
};

#endif
