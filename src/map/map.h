#ifndef __MAP_H__
#define __MAP_H__

#include <exception>
#include <list>
#include <vector>
#include <graphics/surface.h>
#include <glibmm/ustring.h>

class Map
{

  public:

    typedef vector<Glib::ustring> InfoArray;

    explicit Map (const string & xmldoc) throw (std::exception);

    inline bool isInTheVacuum (const Point & p) const
    {
	return m_main_it->GetRGBA (p).GetA () == 0;
    }

    inline bool isInTheGround (const Point & p) const
    {
	return m_main_it->GetRGBA (p).GetA () == 255;
    }

    inline int WidthOfWorld (void) const
    {
	return m_main_it->GetWidth ();
    }

    inline int HeightOfWorld (void) const
    {
	return m_main_it->GetHeight ();
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

    // Draw the map to the Camera.
    // This method redraw only the "scrolled" parts,
    // reducing CPU time.
    void draw (void);

    // InfoArray accessor in read-only
    // @return: the map info array
    // elements are respectively in this order:
    //  - author : the map author name.
    //  - name   : the map name. 
    //  - description : a small text to describe the map.
    inline const InfoArray & get_infos (void) const
    {
	return m_infos;
    }
    
    // InfoArray accessor/mutator in rw.
    // @return: the map info array, respectively exactly the same,
    // ways as the last method.
    inline InfoArray & get_infos (void)
    {
	return m_infos;
    }

  protected:
      Map ();

    // Non copyable, except by their sub-class
    Map & operator= (const Map & map);

    typedef list<Surface> LayerList;
    LayerList m_layers;
    LayerList::iterator m_main_it;
    LayerList::iterator m_expl_it;
    bool m_init_draw;
    InfoArray m_infos;
};

#endif
