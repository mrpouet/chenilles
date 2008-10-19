#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <glibmm/ustring.h>
#include <surface.h>
#include <list>

// Abstract Base Class.
//
// Interface of a ArtEditor's Drawable artwork
// (Animation or Map for example).
struct Drawable
{
  public:

    typedef std::list<Surface>::iterator iterator;
    typedef std::vector<Glib::ustring> InfoArray;

    virtual ~ Drawable (){};

    virtual inline bool empty (void) const = 0;

    virtual void draw (void) = 0;

    virtual iterator add_layer (const Glib::ustring & filename) = 0;

    virtual void set_layer (const iterator & it, 
			    const Glib::ustring & data) = 0;

    virtual inline InfoArray & get_infos (void) = 0;

};

// Abstract Base Class.
//
// Interface of a ArtEditor's project,
// typically we just need to inherit from this base,
// to define particular and a specific Project.
// DON'T USE IT DIRECTLY (it's abstract, so not instanciable).

class Project
{
  public:

    virtual ~ Project (){};

    virtual void open (const Glib::ustring & filename) = 0;

    //FIXME: writer this method
    inline void save (void)
    {
	return;
    }

    virtual void save_as (const Glib::ustring & filename,
			  const std::list<Glib::ustring> &l) = 0;

    virtual inline Drawable & get_drawable (void) = 0;

  protected:

    // The file location of the current project
    Glib::ustring m_filename;

};

#endif
