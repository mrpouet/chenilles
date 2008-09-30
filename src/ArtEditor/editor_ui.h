#ifndef __GUI_H__
#define __GUI_H__

#include <gtkmm/window.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/liststore.h>

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

    struct UstringColumns:public TreeModel::ColumnRecord
    {
	UstringColumns ()
	{
	    add (str);
	}

	TreeModelColumn<Glib::ustring> str;
    };

    struct LayerColumns:public UstringColumns
    {
	LayerColumns ():UstringColumns ()
	{
	    add (type);
	}

	TreeModelColumn<Glib::ustring> type;

    };

    void on_new_clicked (void);

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    inline void on_combo_change (const Glib::ustring & path,
				 const Glib::ustring & text)
    {
	TreePath treepath (path);
	TreeModel::Row row = *m_list->get_iter (treepath);
	row[m_layercolumns.type] = text;
    }

    inline void add_list_entry (const Glib::ustring & name,
				const Glib::ustring & type)
    {
	TreeModel::Row row = *(m_list->append ());
	row[m_layercolumns.str] = name;
	row[m_layercolumns.type] = type;
    }

    inline void add_menu_item (Menu * menu, const StockID & id)
    {
	ImageMenuItem *item = manage (new ImageMenuItem (id));
	menu->append (*item);
    }

    inline void add_menu_separator (Menu * menu)
    {
	SeparatorMenuItem *item = manage (new SeparatorMenuItem ());
	menu->append (*item);
    }

    Glib::ustring open_saveas_dialog (const Glib::ustring & title,
				      const FileChooserAction & action);

    // The main ListStore in which information layers
    // will be display.
    Glib::RefPtr<ListStore> m_list;

    // The CellRendereCombo TreeModel.
    Glib::RefPtr<ListStore> m_refTreeModel;

    // The main ListStore TreeModelColumn
    LayerColumns m_layercolumns;

    // The CellRendererCombo TreeModelColumn
    UstringColumns m_ustringcolumns;

    // The GtkSDL Wrapper
    // which contains SDL stream screen
    GtkSDL m_SDLArea;

};

#endif
