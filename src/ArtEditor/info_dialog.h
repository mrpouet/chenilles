#ifndef __INFO_DIALOG_H__
#define __INFO_DIALOG_H__

#include <gtkmm/dialog.h>

using namespace Gtk;

class InfoDialog:public Dialog
{
  public:
    InfoDialog (Gtk::Window & parent);

};

#endif
