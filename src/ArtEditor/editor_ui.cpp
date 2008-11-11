#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>

#include "editor.h"
#include "project_map.h"

#define INIT_WIDTH  480
#define INIT_HEIGHT 80

using sigc::mem_fun;
using Glib::ustring;

namespace
{
    bool attachment = false;

    inline bool on_wrapper_leave_notify_event (GdkEventCrossing * event)
    {
	HMI::GetRef ().SwitchToCursor (HMI::NO_CURSOR);
	return true;
    }

    inline bool on_wrapper_enter_notify_event (GdkEventCrossing * event)
    {
	HMI::GetRef ().SwitchToCursor (HMI::CURSOR_TIP);
	return true;
    }

};


EditorUI::EditorUI (int width, int height):
Window (), m_info_dialog (*this), m_SDLArea (640, 480)
{
    VBox *layerbox = manage (new VBox ());

    MenuBar *bar = manage (new MenuBar ());
    MenuItem *file = manage (new MenuItem ("File", true));
    MenuItem *edit = manage (new MenuItem ("Edit", true));
    MenuItem *help = manage (new MenuItem ("Help", true));

    Menu *filemenu = manage (new Menu ());
    Menu *editmenu = manage (new Menu ());
    Menu *helpmenu = manage (new Menu ());

    ToolButton *New = manage (new ToolButton (Stock::NEW));
    ToolButton *Open = manage (new ToolButton (Stock::OPEN));
    ToolButton *Save = manage (new ToolButton (Stock::SAVE));
    ToolButton *SaveAs = manage (new ToolButton (Stock::SAVE_AS));
    Toolbar *toolbar = manage (new Toolbar ());
    list < Glib::ustring > authors;
    MenuItem *item = NULL;

    m_width = width;

    m_height = height;

    set_size_request (INIT_WIDTH, INIT_HEIGHT);

    set_border_width (0);

    set_title ("ArtEditor");

    // File submenu

    item = add_menu_item (filemenu, Stock::NEW);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_new_clicked));

    item = add_menu_item (filemenu, Stock::OPEN);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_open_clicked));

    add_menu_separator (filemenu);

    item = add_menu_item (filemenu, Stock::SAVE);
    item->signal_activate ().connect (mem_fun
				      (*this, &EditorUI::on_save_clicked));

    item = add_menu_item (filemenu, Stock::SAVE_AS);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_saveas_clicked));


    add_menu_separator (filemenu);

    add_menu_item (filemenu, Stock::CLOSE);
    add_menu_item (filemenu, Stock::QUIT);

    file->set_submenu (*filemenu);

    // Edit submenu
    item = add_menu_item (editmenu, Stock::DIALOG_INFO);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_info_clicked));

    edit->set_submenu (*editmenu);

    m_info_dialog.signal_response ().connect (mem_fun (*this,
						       &EditorUI::
						       on_info_response));

    // Help submenu

    // About dialog

    m_about_dialog.set_name ("ArtEditor");
    m_about_dialog.set_version ("1.0alpha1");
    m_about_dialog.set_copyright
	("Copyright © 2008 PERIER Romain (mrpouet)");
    m_about_dialog.set_comments (get_title ());
    m_about_dialog.set_license ("General Public License v3 (GPL)");

    m_about_dialog.set_website ("http://chenilles.org/ArtEditor");
    m_about_dialog.set_website_label ("ArtEditor website");

    authors.push_back ("NOM Prénom (toineo)");
    authors.push_back ("PERIER Romain (mrpouet)");

    m_about_dialog.set_authors (authors);

    authors.clear ();
    authors.push_back ("PERIER Romain (mrpouet)");
    m_about_dialog.set_documenters (authors);

    authors.clear ();
    authors.push_back ("PILLEBOUE Adrien (necropotame)");
    authors.push_back ("NOM Prénom (herberts)");
    authors.push_back ("NOM Prénom (vladisback)");
    m_about_dialog.set_artists (authors);

    m_about_dialog.set_translator_credits ("PERIER Romain (mrpouet)");
    m_about_dialog.
	set_logo (Gdk::Pixbuf::
		  create_from_file (ustring (EDITORDATA) + "logo.png"));

    m_about_dialog.signal_response ().connect (mem_fun (*this,
							&EditorUI::
							on_about_response));

    add_menu_item (helpmenu, Stock::HELP);

    item = add_menu_item (helpmenu, Stock::ABOUT);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_about_clicked));

    help->set_submenu (*helpmenu);

    bar->append (*file);
    bar->append (*edit);
    bar->append (*help);

    New->signal_clicked ().connect (mem_fun (*this,
					     &EditorUI::on_new_clicked));

    Open->signal_clicked ().connect (mem_fun (*this,
					      &EditorUI::on_open_clicked));

    Save->signal_clicked ().connect (mem_fun
				     (*this, &EditorUI::on_save_clicked));

    SaveAs->signal_clicked ().connect (mem_fun (*this,
						&EditorUI::
						on_saveas_clicked));

    toolbar->append (*New);
    toolbar->append (*Open);
    toolbar->append (*Save);
    toolbar->append (*SaveAs);

    // IconView
    m_refIconTreeModel = LayerStore::create (m_iconcolumns);

    m_refIconTreeModel->signal_drop_end ().connect (mem_fun (*this,
							     &EditorUI::
							     on_iconview_drop_end));

    m_iconview.set_model (m_refIconTreeModel);
    m_iconview.set_markup_column (m_iconcolumns.m_label);
    m_iconview.set_pixbuf_column (m_iconcolumns.m_pixbuf);
    m_iconview.signal_selection_changed ().connect (mem_fun (*this,
							     &EditorUI::
							     on_iconview_selection_changed));
    m_iconview.set_reorderable ();

    // ComboBox
    m_refComboTreeModel = ListStore::create (m_combocolumns);
    m_combobox.set_model (m_refComboTreeModel);

    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "background";
    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "main";
    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "explosion";

    m_combobox.pack_start (m_combocolumns.m_type);
    m_combobox.signal_changed ().connect (mem_fun (*this,
						   &EditorUI::
						   on_combo_changed));

    // Boxes
    layerbox->pack_start (m_combobox, false, false);
    layerbox->pack_start (m_iconview);

    m_hbox.pack_start (m_SDLArea);
    m_hbox.pack_start (*layerbox);

    m_vbox.pack_start (*bar, false, false);

    m_vbox.pack_start (*toolbar, false, false);


    add (m_vbox);

    show_all ();

}

void
EditorUI::on_wrapper_init (void)
{
    HMI::GetRef ().SetCursor (HMI::CURSOR_TIP,
			      Glib::ustring (UIDATA) + "cursor_main.png");
    HMI::GetRef ().SwitchToCursor (HMI::CURSOR_TIP);
    m_SDLArea.signal_enter_notify_event ().
	connect (sigc::ptr_fun (&on_wrapper_enter_notify_event));
    m_SDLArea.signal_leave_notify_event ().
	connect (sigc::ptr_fun (&on_wrapper_leave_notify_event));
    on_open_clicked ();
}

void
EditorUI::on_iconview_drop_end (const TreeModel::Row & dest,
				const TreeModel::Row & src)
{
    Glib::ustring dest_layer_name = dest[m_iconcolumns.m_label];

    Editor::GetRef ().get_current_project ().get_drawable ().
	splice_layer (dest_layer_name, src.get_value (m_iconcolumns.m_label));

}

void
EditorUI::add_icon_entry (const Glib::ustring & label,
			  const Glib::ustring & spec)
{
    TreeModel::Row row = *(m_refIconTreeModel->append ());

    row[m_iconcolumns.m_label] = label;
    row[m_iconcolumns.m_pixbuf] =
	Gdk::Pixbuf::create_from_file (Glib::ustring (EDITORDATA) +
				       "layer.png");
    row[m_iconcolumns.m_index] = (spec == "main") ? 1 :
	((spec == "explosion") ? 2 : 0);
}

void
EditorUI::on_info_response (int response)
{
    switch (response)
      {
      case RESPONSE_OK:
	  {
	      Project & project = Editor::GetRef ().get_current_project ();
	      Drawable::InfoArray & array =
		  project.get_drawable ().get_infos ();

	      if (array.empty ())
		  array.resize (3);
	      array[0] = m_info_dialog.get_author ();
	      array[1] = m_info_dialog.get_name ();

	      if (array.size () >= 3)
		  array[2] = m_info_dialog.get_desc ();
	  }
      default:
	  m_info_dialog.hide ();
	  break;
      }
}

void
EditorUI::on_save_clicked (void)
{
    if (!Editor::GetRef ().get_current_project ().save ())
	on_saveas_clicked ();
}

void
EditorUI::on_info_clicked (void)
{
    const Drawable::InfoArray & array =
	Editor::GetRef ().get_current_project ().get_drawable ().get_infos ();

    if (!array.empty ())
      {
	  m_info_dialog.set_author (array[0]);
	  m_info_dialog.set_name (array[1]);

	  if (array.size () >= 3)
	      m_info_dialog.set_desc (array[2]);
      }
    m_info_dialog.show ();
}

TreeModel::Row EditorUI::get_last_selected_item_row (void)
{
    list < TreeModel::Path > paths_list = m_iconview.get_selected_items ();

    if (paths_list.empty ())
	throw - 1;
    TreeModel::Path path = *(paths_list.begin ());
    return TreeModel::Row (*m_refIconTreeModel->get_iter (path));
}

void
EditorUI::on_iconview_selection_changed (void)
{

    TreeModel::Row row;
    try
    {
	row = get_last_selected_item_row ();
    }
    catch (int &e)
    {
	return;
    }

    m_combobox.set_active (row[m_iconcolumns.m_index]);

}

void
EditorUI::on_combo_changed (void)
{
    TreeModel::iterator iter = (m_combobox.get_active ());
    TreeModel::Row comborow = *iter;
    TreeModel::Row iconrow;

    try
    {
	iconrow = get_last_selected_item_row ();
    }
    catch (int &e)
    {
	return;
    }

    Editor::GetRef ().get_current_project ().
	get_drawable ().set_layer_spec (iconrow[m_iconcolumns.m_label],
					comborow[m_combocolumns.m_type]);

    iconrow[m_iconcolumns.m_index] = m_combobox.get_active_row_number ();
}

void
EditorUI::on_new_clicked (void)
{
    Editor::GetRef ().new_project ();
}

void
EditorUI::on_saveas_clicked (void)
{
    ustring ret = open_saveas_dialog ("Save file as",
				      FILE_CHOOSER_ACTION_SAVE);
    if (ret.empty ())
	return;

    Editor::GetRef ().save_project_as (ret);
}

void
EditorUI::on_open_clicked (void)
{
    static ustring ret;
    static size_t id;
    Editor & editor = Editor::GetRef ();
    Drawable & d = editor.get_current_project ().get_drawable ();

    if (!attachment)
      {

	  ret = open_saveas_dialog ("Open file", FILE_CHOOSER_ACTION_OPEN);

	  if (ret.empty ())
	      return;

	  id = ret.rfind (".") + 1;

	  // Attaching Hbox and GtkSDL stream to the window
	  if (d.empty ())
	    {
		set_size_request (m_width, m_height);
		m_vbox.pack_start (m_hbox);
		m_vbox.show_all ();
		m_SDLArea.signal_init ().connect (mem_fun(*this,
				   &EditorUI::on_wrapper_init));
		attachment = true;
		return;
	    }

      }


    if (ret.substr (id) == "png")
      {
	  d.add_layer (ret);
	  ret = EditableMap::layername_from_path (ret);

	  // layer is of type background as default.
	  // user is welcome to change it.
	  add_icon_entry (ret, "background");
      }
    else
      {
	  editor.open_project (ret);

	  for (Drawable::FilesList::const_iterator it =
	       d.get_fileslist ().begin ();
	       it != d.get_fileslist ().end (); it++)
	    {
		ret = EditableMap::layername_from_path (*it);
		add_icon_entry (ret, d.get_layer_spec (ret));
	    }

      }

    attachment = false;

}

ustring
    EditorUI::open_saveas_dialog (const ustring & title,
				  const FileChooserAction & action)
{
    FileChooserDialog dialog (*this, title, action);
    ustring ret;

    // A filter for files, depending of the type
    // (thanks to toineo)
    FileFilter filter_png, filter_xml;

    if (action == FILE_CHOOSER_ACTION_OPEN)
      {
	  filter_png.set_name ("Layer (png)");
	  filter_png.add_mime_type ("image/png");
	  dialog.add_filter (filter_png);
      }

    filter_xml.set_name ("Map/Sprite sheet (xml)");
    filter_xml.add_mime_type ("text/xml");

    dialog.add_filter (filter_xml);

    dialog.add_button (Stock::CANCEL, RESPONSE_CANCEL);
    dialog.add_button ((action == FILE_CHOOSER_ACTION_OPEN) ?
		       Stock::OPEN : Stock::SAVE, RESPONSE_OK);

    switch (dialog.run ())
      {
      case RESPONSE_OK:
	  ret = dialog.get_filename ();
	  break;
      default:
	  ret = ustring ();
	  break;
      }

    return ret;

}
