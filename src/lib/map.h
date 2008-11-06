#ifndef __MAP_H__
#define __MAP_H__

#include <exception>
#include <list>
#include <vector>
#include <surface.h>
#include <glibmm/ustring.h>

namespace Chenilles
{

    class Map
    {

      public:

	typedef std::vector < Glib::ustring > InfoArray;
	typedef void (*ParseCallback) (const Glib::ustring &);

	// Instance a Map from a mapsheet
	// @throw exception
	// @param xmldoc The absolute path to the mapsheet
	  Map (const std::string & xmldoc);

	inline bool isInTheVacuum (const Point & p) const
	{
	    return m_main_it->GetPixColor (p).GetA () == 0;
	}

	inline bool isInTheGround (const Point & p) const
	{
	    return m_main_it->GetPixColor (p).GetA () == 255;
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

	typedef std::list < Surface > LayerList;

	// A map is only instanciable as default by her subclass.
	  Map ();

	// Same as public Map constructor except that this method
	// can be called from a subclass when we want
	// independante of constructor list initializer.
	// @throw exception.
	// @param xmldoc The Absolute path to the mapsheet
	// @param vfunc function pointer which pointed from to
	// a callback to call when a layer is added in LayerList container.
	// parameter corresponding to layerfile path.
	// (give extra informations about mapsheet to subclass avoiding
	// complexity increasing, note that the only extra info are layername).
	void CreateFromXML (const std::string & xmldoc,
			    ParseCallback vfunc = NULL);

	LayerList m_layers;

	LayerList::iterator m_main_it;

	LayerList::iterator m_expl_it;

	bool m_init_draw;

	InfoArray m_infos;

      private:
	// No copyable
	Map & operator= (const Map & map);

    };

};

#endif
