#include <gtkmm/label.h>
#include <gtkmm/stock.h>

#include "info_dialog.h"

InfoDialog::InfoDialog (Gtk::Window & parent):
Dialog ("Map informations", parent, false, true)
{
    VBox *vbox = get_vbox ();
    HBox *author_box = manage (new HBox ());
    HBox *name_box = manage (new HBox ());
    HBox *desc_box = manage (new HBox ());

    Label *name_label = manage (new Label ("name:"));
    Label *author_label = manage (new Label ("author:"));
    Label *desc_label = manage (new Label ("description:"));

    author_box->pack_start (*author_label);
    author_box->pack_end (m_author);
    name_box->pack_start (*name_label);
    name_box->pack_end (m_name);
    desc_box->pack_start (*desc_label);
    desc_box->pack_end (m_desc);

    vbox->pack_start (*author_box);
    vbox->pack_start (*name_box);
    vbox->pack_start (*desc_box);

    vbox->show_all ();

    add_button (Stock::CANCEL, RESPONSE_CANCEL);
    add_button (Stock::OK, RESPONSE_OK);

}
