#ifndef __PROJECT_MAP_H__
#define __PROJECT_MAP_H__

#include <map>
#include <map/map.h>
#include "project.h"

class EditableMap:public Drawable, public Map
{
  public:

    EditableMap ():Drawable (), Map (){};

    ~EditableMap (){};

    explicit EditableMap (const Glib::ustring & xmldoc):Drawable (),
	Map (xmldoc)
    {};

    Drawable::iterator add_layer (const Glib::ustring & filename);

    void set_layer (const Drawable::iterator & it,
		    const Glib::ustring & data);

    void write_to_file (const Glib::ustring & filename,
			const std::list<Glib::ustring> &l);

    inline void draw (void)
    {
	if (!m_layers.empty ())
	    Map::draw ();
    }

    inline bool empty (void) const
    {
	return m_layers.empty ();
    }

    inline Drawable::InfoArray & get_infos (void)
    {
	return Map::get_infos ();
    }


};


class ProjectMap:public Project
{
  public:

    void open (const Glib::ustring & filename);

    void save_as (const Glib::ustring & filename,
		  const std::list<Glib::ustring> &l);

    inline Drawable & get_drawable (void)
    {
	return m_map;
    }

  private:

      EditableMap m_map;

};

#endif
