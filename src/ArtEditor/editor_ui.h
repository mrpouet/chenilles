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
#include <gtkmm/combobox.h>
#include <gtkmm/iconview.h>
#include <tools/base.h>

#include "gtksdl.h"
#include "info_dialog.h"
#include "project.h"
#include <cstdio>
#define UIDATA DATA "/ui/"

class EditorUI:public Window
{
  public:

    EditorUI (int width, int height);

    inline sigc::signal<void>& signal_init(void)
    {
      return m_SDLArea.signal_init();
    }


  private:

    struct IconModelColumns:public TreeModel::ColumnRecord
    {
      IconModelColumns ()
	{
	    add (m_label);
	    add (m_pixbuf);
	    add (m_index);
	}

	TreeModelColumn<Glib::ustring> m_label;
	TreeModelColumn< Glib::RefPtr < Gdk::Pixbuf > > m_pixbuf;
	// Undisplayed value, just needed to save informations
	// per line.
	// The row number to select in combobox
	// (which represents the layer type)
	TreeModelColumn<int> m_index;
    };

    struct ComboModelColumns:public TreeModel::ColumnRecord
    {
	ComboModelColumns ()
	{
	    add (m_type);
	}

	TreeModelColumn<Glib::ustring> m_type;
    };
    
    void on_cursor_init(void);

    void on_new_clicked (void);

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    void on_combo_changed(void);

    void on_iconview_selection_changed(void);

    void on_info_clicked (void);


    void on_info_response (int response);

    inline void on_about_clicked (void)
    {
	m_about_dialog.show ();
    }

    inline void dialog_hide(Widget& widget, int response)
    {
      if (response == RESPONSE_CANCEL)
	widget.hide();
    }

    inline void on_about_response (int response)
    {
      dialog_hide(m_about_dialog, response);
    }

    void add_icon_entry (const Glib::ustring & label,
			 const Glib::ustring & spec);

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

    TreeModel::Row get_last_selected_item_row(void);

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

    // IconView layers
    IconView m_iconview;

    ComboBox m_combobox;

    // About ArtEditor
    AboutDialog m_about_dialog;

    // Artwork information dialog
    InfoDialog m_info_dialog;

    // The GtkSDL Wrapper
    // (which contains SDL stream screen)
    GtkSDL m_SDLArea;

};

#endif
