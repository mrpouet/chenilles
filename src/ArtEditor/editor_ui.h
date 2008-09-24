#ifndef __GUI_H__
#define __GUI_H__

#include <gtkmm/window.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>

#include "gtksdl.h"

class EditorUI:public Window
{
  public:

    EditorUI (int width, int height);

    inline signal<void> signal_init (void)
    {
	return m_SDLArea.signal_init ();
    }

  private:

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    Glib::ustring open_saveas_dialog (const Glib::ustring & title,
				      const FileChooserAction & action);
    ListViewText m_list;

    GtkSDL m_SDLArea;

};

#endif
