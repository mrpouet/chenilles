#include <cstdio>
#include <gdkmm.h>
#include <gdk/gdkx.h>
#include <interface/camera.h>
#include <game/timer.h>
#include "gtksdl.h"

using Glib::unwrap;

GtkSDL::GtkSDL (int width, int height):
DrawingArea ()
{
    // Disable double-buffering to avoid the automatic
    // clearing background color (due to Gdk::Window::begin_paint_rect)
    set_double_buffered (false);

    set_size_request (width, height);

    add_events (Gdk::POINTER_MOTION_MASK);

    m_init = true;

    m_need_resize = false;

    m_configure_time = 0;

    Glib::signal_timeout ().connect (sigc::mem_fun (*this,
						    &GtkSDL::on_resize), 20);

}

GtkSDL::~GtkSDL ()
{
    HMI::CleanUp ();
    Timer::CleanUp ();
}

void
GtkSDL::SetVideoMode (int width, int height)
{
    gchar SDL_WINDOWID[32];

    // As default, SDL check if environment variable
    // SDL_WINDOWID is defined, on true he diverts
    // the screen stream to the following XID (if it exists)
    // otherwises, just creates and display a new Xwindow.
    //FIXME: not portable to Win32.
    snprintf (SDL_WINDOWID, 32 * sizeof (gchar), "SDL_WINDOWID=%ld",
	      GDK_WINDOW_XWINDOW (unwrap (get_window ())));

    SDL_putenv (SDL_WINDOWID);

    HMI::Init ();

    HMI::GetRef ().SetVideoMode (width, height, true);

    Timer::GetRef ().Reset ();
}

bool
GtkSDL::on_configure_event (GdkEventConfigure * event)
{
    if (m_init)
	return true;
    set_size_request (event->width, event->height);
    // Ask to HMI to blank the screen
    HMI::GetRef ().Clear ();
    // Locking any refresh during resize
    // (avoiding flickers)
    HMI::GetRef ().LockRefresh ();
    m_configure_time = Timer::GetRef ().Read ();
    m_need_resize = true;
    return true;
}

// Use a timer to delay as much as possible 
// the real time resize.
// More "configure_event" signal is emit and more time of
// the resize is delayed.
// Is equivalent to really resize SDL stream, ONLY
// at the end of handler resizement (using borders).

bool
GtkSDL::on_resize (void)
{
    if ((!m_need_resize) ||
	(Timer::GetRef ().Read () < (m_configure_time + 100)))
	return true;
    SetVideoMode (get_width (), get_height ());
    HMI::GetRef ().UnlockRefresh ();
    m_need_resize = false;
    return true;
}

// Simple way to share the same cursor
// between Gtkmm and SDL (2 threads).
// The problem is due to thread-safe.

bool
GtkSDL::on_motion_notify_event (GdkEventMotion * event)
{
    Point sdl_tip;

    get_pointer (sdl_tip.x, sdl_tip.y);

    HMI::GetRef ().SetPointer (sdl_tip);

    HMI::GetRef ().RefreshOutput ();

    return true;
}

bool
GtkSDL::on_expose_event (GdkEventExpose * event)
{

    if (!m_init)
      {
	  const Rectangle & box = Camera::GetRef ().GetCameraBox ();
	  Camera::GetRef ().ToRedraw (Rectangle (0, 0, box.w, box.h));
	  HMI::GetRef ().RefreshOutput ();
	  return true;
      }


    SetVideoMode (get_width (), get_height ());

    do_init ();

    m_init = false;

    return true;
}
