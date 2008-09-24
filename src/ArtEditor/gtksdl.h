#ifndef __GTKSDL_H__
#define __GTKSDL_H__

#include <gtkmm/drawingarea.h>

using namespace Gtk;
using sigc::signal;

class GtkSDL:public DrawingArea
{
  public:
    GtkSDL (int width, int height);
    ~GtkSDL ();

    inline const signal<void>& signal_init (void)
    {
	return m_signal_init;
    }

  private:

    inline void do_init (void)
    {
	m_signal_init.emit ();
    }

    bool on_expose_event (GdkEventExpose * event);

    bool on_motion_notify_event (GdkEventMotion * event);

    bool m_init;

    signal<void> m_signal_init;

};

#endif
