#ifndef __GTKSDL_H__
#define __GTKSDL_H__

#include <gtkmm/drawingarea.h>
#include <SDL/SDL_types.h>

using namespace Gtk;
using sigc::signal;

class GtkSDL: public DrawingArea
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

    void SetVideoMode (int width, int height);

    bool on_configure_event (GdkEventConfigure * event);

    bool on_resize (void);

    bool on_expose_event (GdkEventExpose * event);

    bool on_motion_notify_event (GdkEventMotion * event);

    bool m_init;

    bool m_need_resize;

    Uint32 m_configure_time;

    signal<void> m_signal_init;

};

#endif
