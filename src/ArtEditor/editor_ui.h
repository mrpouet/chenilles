#ifndef __GUI_H__
#define __GUI_H__

#include <gtkmm/window.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>

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

    void on_new_clicked(void);

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    inline void add_menu_item(Menu* menu, const StockID& id)
    {
      ImageMenuItem *item = manage(new ImageMenuItem(id));
      menu->append(*item);
    }

    inline void add_menu_separator(Menu* menu)
    {
      SeparatorMenuItem *item = manage(new SeparatorMenuItem());
      menu->append(*item);
    }

    Glib::ustring open_saveas_dialog (const Glib::ustring & title,
				      const FileChooserAction & action);
    ListViewText m_list;

    GtkSDL m_SDLArea;

};

#endif
