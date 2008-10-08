#ifndef __EDITOR_UI_H__
#define __EDITOR_UI_H__

#include <gtkmm/window.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/liststore.h>
#include <gtkmm/entry.h>
#include <gtkmm/aboutdialog.h>

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

    struct IconModelColumns: public TreeModel::ColumnRecord
      {
	IconModelColumns()
	  {
	    add (m_label);
	    add (m_pixbuf);
	  }

	TreeModelColumn<Glib::ustring> m_label;
	TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_pixbuf;
      };

    struct ComboModelColumns: public TreeModel::ColumnRecord
    {
      ComboModelColumns()
	{
	  add(m_type);
	}

      TreeModelColumn<Glib::ustring> m_type;
    };

    void on_new_clicked (void);

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    inline void on_about_clicked(void)
    {
      m_about_dialog.show();
    }
      

    inline void add_icon_entry(const Glib::ustring& label)
    {
      TreeModel::Row row = *(m_refIconTreeModel->append());

      row[m_iconcolumns.m_label] = label;
      row[m_iconcolumns.m_pixbuf] = Gdk::Pixbuf::create_from_file("layer.png");
    }

    inline MenuItem *add_menu_item (Menu * menu, const StockID & id)
    {
	ImageMenuItem *item = manage (new ImageMenuItem (id));
	menu->append (*item);

	return item;
    }

    inline void add_menu_separator (Menu * menu)
    {
	SeparatorMenuItem *item = manage (new SeparatorMenuItem ());
	menu->append (*item);
    }

    Glib::ustring open_saveas_dialog (const Glib::ustring & title,
				      const FileChooserAction & action);

    // IconView TreeModel
    Glib::RefPtr<ListStore> m_refIconTreeModel;
    // ComboBox TreeModel
    Glib::RefPtr<ListStore> m_refComboTreeModel;

    // IconView ModelColumn
    IconModelColumns m_iconcolumns;
    // ComboBox ModelColumn
    ComboModelColumns m_combocolumns;

    // The map name text entry
    Entry m_name_entry;

    // About ArtEditor
    AboutDialog m_about_dialog;

    // The GtkSDL Wrapper
    // (which contains SDL stream screen)
    GtkSDL m_SDLArea;

};

#endif
