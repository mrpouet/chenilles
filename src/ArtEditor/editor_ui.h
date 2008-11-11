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
#include "layerstore.h"

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
    
    void on_wrapper_init(void);

    // Methods connect to "drop_end" signal (see layerstore module)
    // @param dest The Row after which "src" was inserted
    // @param src The Row dropped.
    // Note: The main "problem" (not a really problem because DnD is very well managed within gtkmm)
    // is m_iconview is both the Drag and the Drop site, so all Widget::drag_* are called
    // "together" (and on the same widget) => emplies that we can't formelly indentify Drop or Drag site
    // at a given time.
    // Solution consist to create a LayerStore class which inherit from Gtk::ListStore
    // and we just need to overload drag_data_received_vfunc method, which is called
    // when a insertion before 'src' row is tried (tried by gtk itself on user ask, after a drop)
    // see layerstore module for more details.
    void on_iconview_drop_end(const TreeModel::Row& dest, const TreeModel::Row &src);

    void on_new_clicked (void);

    void on_open_clicked (void);

    void on_saveas_clicked (void);

    void on_combo_changed(void);

    void on_iconview_selection_changed(void);

    void on_save_clicked (void);

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
    Glib::RefPtr<LayerStore> m_refIconTreeModel;
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

    int m_width;
    
    int m_height;

    VBox m_vbox;

    HBox m_hbox;


};

#endif
