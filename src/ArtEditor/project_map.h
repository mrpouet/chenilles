#ifndef __PROJECT_MAP_H__
#define __PROJECT_MAP_H__

#include <tr1/unordered_map>
#include <map.h>
#include <tools/base.h>
#include "project.h"
#include <cstdio>

class ProjectMap;

class EditableMap:public Drawable, public Map
{
  public:

    EditableMap () {};

    ~EditableMap () {};

    explicit EditableMap (const Glib::ustring & xmldoc);

    void open (const Glib::ustring & xmldoc);

    void add_layer (const Glib::ustring & path);

    void set_layer_spec (const Glib::ustring & layername,
			 const Glib::ustring & data);

    Glib::ustring get_layer_spec (const Glib::ustring & layername)
    {
	Map::LayerList::iterator it = m_Ltable[layername.raw ()];
	return (m_main_it == it) ? "main" :
	    ((m_expl_it == it) ? "explosion" : "background");

    }

    inline void draw (void)
    {
	if (!m_layers.empty ())
	    Map::draw ();
    }

    bool empty (void) const
    {
	return m_layers.empty ();
    }

    Drawable::InfoArray & get_infos (void)
    {
	return Map::get_infos ();
    }

    const Drawable::FilesList & get_fileslist (void) const
    {
	return m_files_list;
    }

  private:

    typedef std::tr1::unordered_map < std::string, 
      Map::LayerList::iterator > HashTable;
    typedef std::list < Glib::ustring > FilesList;

    void write_to_file (const Glib::ustring & filename);

    friend class ProjectMap;
    friend void container_builder (const Glib::ustring &);

    FilesList m_files_list;

    HashTable m_Ltable;
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
