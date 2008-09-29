#ifndef __PROJECT_MAP_H__
#define __PROJECT_MAP_H__

#include <map>
#include <map/map.h>
#include "project.h"

class EditableMap:public Drawable, public Map
{
  public:

    EditableMap ():Drawable (), Map () {};

    ~EditableMap(){};

    explicit EditableMap (const Glib::ustring & xmldoc):Drawable (),
	Map (xmldoc) {};

    void add_layer (const Glib::ustring & filename);

    void set_layer_visible (const Glib::ustring & name);

    void set_layer_exclude (const Glib::ustring & name);

    void write_to_file (const Glib::ustring & filename);

    inline void draw (void)
    {
      if (!m_layers.empty())
	Map::draw ();
    }

    inline bool empty(void) const
    {
      return m_layers.empty() && m_exclude_list.empty();
    }

  private:
    LayerList m_exclude_list;
    map<Glib::ustring, LayerList::iterator> m_handler_it;

};


class ProjectMap:public Project
{
  public:

    void open (const Glib::ustring & filename);

    void save_as (const Glib::ustring & filename);

    inline Drawable & get_drawable (void)
    {
	return m_map;
    }

  private:

      EditableMap m_map;

};

#endif
