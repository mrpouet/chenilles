#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <glibmm/ustring.h>

// Abstract Base Class.
//
// Interface of a ArtEditor's Drawable artwork
// (Animation or Map for example).
struct Drawable
{
  public:

    virtual ~Drawable() {};

    virtual inline bool empty (void) const = 0;

    virtual void draw (void) = 0;

    virtual void add_layer (const Glib::ustring & filename) = 0;

    virtual void set_layer_visible (const Glib::ustring & name) = 0;

    virtual void set_layer_exclude (const Glib::ustring & name) = 0;

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

    virtual ~ Project () {};

    virtual void open (const Glib::ustring & filename) = 0;

    inline void save (void)
    {
	if (!m_filename.empty ())
	    save_as (m_filename);
    }

    virtual void save_as (const Glib::ustring & filename) = 0;

    virtual inline Drawable & get_drawable (void) = 0;

  protected:

    // The file location of the current project
    Glib::ustring m_filename;

};

#endif
