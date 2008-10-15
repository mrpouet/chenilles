#ifndef __INFO_DIALOG_H__
#define __INFO_DIALOG_H__

#include <gtkmm/entry.h>
#include <gtkmm/dialog.h>

using namespace Gtk;

class InfoDialog:public Dialog
{
  public:
    InfoDialog (Gtk::Window & parent);

    inline Glib::ustring get_author (void) const
    {
	return m_author.get_text ();
    }

    inline void set_author (const Glib::ustring & author)
    {
	m_author.set_text (author);
    }

    inline Glib::ustring get_name (void) const
    {
	return m_name.get_text ();
    }

    inline void set_name (const Glib::ustring & name)
    {
	m_name.set_text (name);
    }

    inline Glib::ustring get_desc (void) const
    {
	return m_desc.get_text ();
    }

    inline void set_desc (const Glib::ustring & desc)
    {
	m_desc.set_text (desc);
    }

  private:
    Entry m_author;
    Entry m_name;
    Entry m_desc;

};

#endif
