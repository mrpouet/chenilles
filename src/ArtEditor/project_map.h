#ifndef __PROJECT_MAP_H__
#define __PROJECT_MAP_H__

#include <tr1/unordered_map>
#include <map.h>
#include <tools/base.h>
#include "project.h"

class ProjectMap;

class EditableMap:public Drawable, public Map
{
  public:

    EditableMap () {};

    ~EditableMap () {};

    explicit EditableMap (const Glib::ustring & xmldoc);

    void open (const Glib::ustring & xmldoc);

    void add_layer (const Glib::ustring & path);

    void splice_layer (const Glib::ustring & dest, const Glib::ustring & src);

    void set_layer_spec (const Glib::ustring & layername,
			 const Glib::ustring & data);

    Glib::ustring get_layer_spec (const Glib::ustring & layername)
    {
	Map::LayerList::iterator it = m_Ltable[layername.raw ()].first;
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

    static inline 
    Glib::ustring layerfilename_from_path (const Glib::ustring & path)
    {
        Glib::ustring::size_type id = path.rfind('/');
	return path.substr ((id != Glib::ustring::npos) ? (id + 1): 0);
    }
    static inline 
    Glib::ustring layername_from_path (const Glib::ustring & path)
    {
        Glib::ustring sub = layerfilename_from_path (path);
	return sub.substr (0, sub.rfind ('.'));
    }

  private:

    typedef std::list < Glib::ustring > FilesList;
    typedef std::pair <Map::LayerList::iterator, FilesList::iterator> PairIter;
    typedef std::tr1::unordered_map < std::string, PairIter> HashTable;

    void write_to_file (const Glib::ustring & filename);

    void add_containers_entry (const Glib::ustring & path);

    inline void layer_add_vfunc (const Glib::ustring & path)
    {
      add_containers_entry(path);
    }

    friend class ProjectMap;

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
