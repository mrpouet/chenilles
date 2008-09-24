#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>

#include <iostream>
#include "editor_ui.h"

using sigc::mem_fun;
using Glib::ustring;
using std::cout;
using std::endl;

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

    ToolButton *New = manage (new ToolButton (Stock::NEW));
    ToolButton *Open = manage (new ToolButton (Stock::OPEN));
    ToolButton *Save = manage (new ToolButton (Stock::SAVE));
    ToolButton *SaveAs = manage (new ToolButton (Stock::SAVE_AS));
    Toolbar *toolbar = manage (new Toolbar ());

    set_size_request (width, height);

    set_border_width (0);

    set_title ("ArtEditor - Artworks Editor For Chenilles");

    bar->append (*file);
    bar->append (*edit);
    bar->append (*option);
    bar->append (*help);

    Open->signal_clicked ().connect (mem_fun (*this,
					      &EditorUI::on_open_clicked));
    SaveAs->signal_clicked ().connect (mem_fun (*this,
						&EditorUI::
						on_saveas_clicked));

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
EditorUI::on_saveas_clicked (void)
{
  open_saveas_dialog ("Save file as", FILE_CHOOSER_ACTION_SAVE);

  cout << "Not implemented yet !" << endl;
}

void
EditorUI::on_open_clicked (void)
{
    open_saveas_dialog ("Open file", FILE_CHOOSER_ACTION_OPEN);

    cout << "Not implemented yet !" << endl;
}

ustring
EditorUI::open_saveas_dialog (const ustring & title,
			      const FileChooserAction & action)
{
    FileChooserDialog dialog (*this, title, action);
    ustring ret;

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
