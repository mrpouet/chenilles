#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>

#include "project_map.h"
#include "editor.h"

using sigc::mem_fun;
using Glib::ustring;

EditorUI::EditorUI (int width, int height):
Window (),
m_list (2),
m_SDLArea (640, 480)
{
    VBox *vbox = manage (new VBox ());
    HBox *hbox = manage (new HBox ());

    MenuBar *bar = manage (new MenuBar ());
    MenuItem *file = manage (new MenuItem ("File", true));
    MenuItem *edit = manage (new MenuItem ("Edit", true));
    MenuItem *option = manage (new MenuItem ("Option", true));
    MenuItem *help = manage (new MenuItem ("Help", true));

    Menu *filemenu = manage (new Menu ());

    ToolButton *New = manage (new ToolButton (Stock::NEW));
    ToolButton *Open = manage (new ToolButton (Stock::OPEN));
    ToolButton *Save = manage (new ToolButton (Stock::SAVE));
    ToolButton *SaveAs = manage (new ToolButton (Stock::SAVE_AS));
    Toolbar *toolbar = manage (new Toolbar ());

    set_size_request (width, height);

    set_border_width (0);

    set_title ("ArtEditor - Artworks Editor For Chenilles");

    add_menu_item (filemenu, Stock::NEW);
    add_menu_item (filemenu, Stock::OPEN);
    add_menu_separator (filemenu);

    add_menu_item (filemenu, Stock::SAVE);
    add_menu_item (filemenu, Stock::SAVE_AS);

    add_menu_separator (filemenu);

    add_menu_item (filemenu, Stock::CLOSE);
    add_menu_item (filemenu, Stock::QUIT);

    file->set_submenu (*filemenu);

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

    m_list.set_column_title (0, "Layers");
    m_list.set_column_title (1, "Types");

    hbox->pack_start (m_SDLArea, false, false);
    hbox->pack_start (m_list, false, false);

    vbox->pack_start (*bar, false, false);

    vbox->pack_start (*toolbar, false, false);

    vbox->pack_start (*hbox, false, false);

    add (*vbox);

    show_all ();

}

void
EditorUI::on_new_clicked (void)
{
    Editor::GetRef ().new_project ();
}

void
EditorUI::on_saveas_clicked (void)
{
    ustring ret = open_saveas_dialog ("Save file as", FILE_CHOOSER_ACTION_SAVE);

    if (ret.empty ())
	return;
    Editor::GetRef ().save_project_as (ret);
}

void
EditorUI::on_open_clicked (void)
{
    ustring ret;
    size_t id;
    Editor & editor = Editor::GetRef ();

    ret = open_saveas_dialog ("Open file", FILE_CHOOSER_ACTION_OPEN);

    if (ret.empty ())
	return;

    id = ret.rfind (".");
    id++;

    if (ret.substr (id) == "png")
      {
	  ustring sub = ret.substr (ret.rfind ("/") + 1);
	  sub = sub.substr (0, sub.size () - 4);
	  m_list.append_text (sub);
	  editor.add_layer_to_project (ret);
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

    filter_png.set_name ("Layer (png)");
    filter_png.add_mime_type ("image/png");

    filter_xml.set_name ("Map/Sprite sheet (xml)");
    filter_xml.add_mime_type ("text/xml");

    dialog.add_filter (filter_png);
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
