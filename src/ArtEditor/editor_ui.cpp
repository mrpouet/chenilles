#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>

#include "editor.h"
#include "project_map.h"

using sigc::mem_fun;
using Glib::ustring;

EditorUI::EditorUI (int width, int height):
Window (),
m_info_dialog (*this),
m_SDLArea (640, 480)
{
    VBox *vbox = manage (new VBox ());
    HBox *hbox = manage (new HBox ());
    VBox *layerbox = manage (new VBox ());

    MenuBar *bar = manage (new MenuBar ());
    MenuItem *file = manage (new MenuItem ("File", true));
    MenuItem *edit = manage (new MenuItem ("Edit", true));
    MenuItem *option = manage (new MenuItem ("Option", true));
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

    set_size_request (width, height);

    set_border_width (0);

    set_title ("ArtEditor - Artworks Editor For Chenilles");

    // File submenu

    add_menu_item (filemenu, Stock::NEW);
    add_menu_item (filemenu, Stock::OPEN);

    add_menu_separator (filemenu);

    add_menu_item (filemenu, Stock::SAVE);
    add_menu_item (filemenu, Stock::SAVE_AS);

    add_menu_separator (filemenu);

    add_menu_item (filemenu, Stock::CLOSE);
    add_menu_item (filemenu, Stock::QUIT);

    file->set_submenu (*filemenu);

    // Edit submenu
    item = add_menu_item (editmenu, Stock::DIALOG_INFO);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_info_clicked));
    add_menu_separator (editmenu);
    add_menu_item (editmenu, Stock::PREFERENCES);

    edit->set_submenu (*editmenu);

    m_info_dialog.signal_response ().connect (mem_fun (*this,
					      &EditorUI::on_info_response));

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
    m_about_dialog.set_logo (Gdk::Pixbuf::create_from_file (ustring (DATA)
							    + "logo.png"));

    m_about_dialog.signal_response ().connect (mem_fun (*this,
					       &EditorUI::on_about_response));

    add_menu_item (helpmenu, Stock::HELP);

    item = add_menu_item (helpmenu, Stock::ABOUT);
    item->signal_activate ().connect (mem_fun (*this,
					       &EditorUI::on_about_clicked));

    help->set_submenu (*helpmenu);

    bar->append (*file);
    bar->append (*edit);
    bar->append (*option);
    bar->append (*help);

    New->signal_clicked ().connect (mem_fun (*this,
					     &EditorUI::on_new_clicked));

    Open->signal_clicked ().connect (mem_fun (*this,
					      &EditorUI::on_open_clicked));
    SaveAs->signal_clicked ().connect (mem_fun (*this,
						&EditorUI::on_saveas_clicked));

    toolbar->append (*New);
    toolbar->append (*Open);
    toolbar->append (*Save);
    toolbar->append (*SaveAs);

    // IconView
    m_refIconTreeModel = ListStore::create (m_iconcolumns);

    m_iconview.set_model (m_refIconTreeModel);
    m_iconview.set_markup_column (m_iconcolumns.m_label);
    m_iconview.set_pixbuf_column (m_iconcolumns.m_pixbuf);
    m_iconview.signal_selection_changed ().connect (mem_fun (*this,
				     &EditorUI::on_iconview_selection_changed));

    // ComboBox
    m_refComboTreeModel = ListStore::create (m_combocolumns);
    m_combobox.set_model (m_refComboTreeModel);

    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "background";
    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "main";
    (*m_refComboTreeModel->append ())[m_combocolumns.m_type] = "explosion";

    m_combobox.pack_start (m_combocolumns.m_type);
    m_combobox.signal_changed ().connect (mem_fun (*this,
					  &EditorUI::on_combo_changed));

    // Boxes
    layerbox->pack_start (m_combobox, false, false);
    layerbox->pack_start (m_iconview);

    hbox->pack_start (m_SDLArea);
    hbox->pack_start (*layerbox);

    vbox->pack_start (*bar, false, false);

    vbox->pack_start (*toolbar, false, false);

    vbox->pack_start (*hbox);

    add (*vbox);

    show_all ();

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
EditorUI::on_info_clicked (void)
{
    const Drawable::InfoArray & array =
	Editor::GetRef ().get_current_project ().get_drawable ().get_infos ();

    if (!array.empty ())
      {
	  m_info_dialog.set_author (array[0]);
	  m_info_dialog.set_name (array[1]);

	  // description is optional in the Map DTD.
	  if (array.size () >= 3)
	      m_info_dialog.set_desc (array[2]);
      }
    m_info_dialog.show ();
}

TreeModel::Row 
EditorUI::get_last_selected_item_row (void)
{
    list<TreeModel::Path> paths_list = m_iconview.get_selected_items ();

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

    TreeModel::iterator it =
	m_refComboTreeModel->get_iter (row[m_iconcolumns.m_type]);
    m_combobox.set_active (it);

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

    Editor::GetRef ().get_current_project ().get_drawable ().
	set_layer (iconrow[m_iconcolumns.m_it],
		   comborow[m_combocolumns.m_type]);

    iconrow[m_iconcolumns.m_type] =
	m_refComboTreeModel->get_path (iter).to_string ();
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
    TreeModel::Children::iterator it;
    std::list < Glib::ustring > l;

    if (ret.empty ())
	return;

    for (it = m_refIconTreeModel->children ().begin ();
	 it != m_refIconTreeModel->children ().end (); it++)
	l.push_back ((*it)[m_iconcolumns.m_label]);

    Editor::GetRef ().save_project_as (ret, l);
}

void
EditorUI::on_open_clicked (void)
{
    ustring ret;
    size_t id;
    Editor & editor = Editor::GetRef ();
    Drawable::iterator it;

    ret = open_saveas_dialog ("Open file", FILE_CHOOSER_ACTION_OPEN);

    if (ret.empty ())
	return;

    id = ret.rfind (".");
    id++;

    if (ret.substr (id) == "png")
      {
	  ustring sub = ret.substr (ret.rfind ("/") + 1);
	  sub = sub.substr (0, sub.size () - 4);
	  it = editor.add_layer_to_drawable_project (ret);
	  add_icon_entry (sub, it);
      }
    else
	editor.open_project (ret);

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
