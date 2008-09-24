#include <cstdio>
#include <gdkmm.h>
#include <gdk/gdkx.h>
#include <interface/camera.h>
#include "gtksdl.h"

using namespace Glib;

GtkSDL::GtkSDL (int width, int height):
DrawingArea ()
{
    // Disable double-buffering to avoid the automatic
    // clearing background color (due to Gdk::Window::begin_paint_rect)
    set_double_buffered (false);

    set_size_request (width, height);

    add_events (Gdk::POINTER_MOTION_MASK);

    m_init = true;

}

GtkSDL::~GtkSDL ()
{
    HMI::CleanUp ();
}

// Simple way to share the same cursor
// between Gtkmm and SDL (2 threads).
// The problem is Gtkmm lock the X11 cursor (thread-safe).
//
// TODO: Is it possible to ask to gdk to unlock x11 cursor ?
// (relock it at end on this method)

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
    gchar SDL_WINDOWID[32];

    if (!m_init)
      {
	  const Rectangle & box = Camera::GetRef ().GetCameraBox ();
	  Camera::GetRef ().ToRedraw (Rectangle (0, 0, box.w, box.h));
	  HMI::GetRef ().RefreshOutput ();
	  return true;
      }

    // As default, SDL check if environment variable
    // SDL_WINDOWID is defined, on true he diverts
    // the screen stream to the following XID (if it exists)
    // otherwises, just creates and display a new Xwindow.
    //FIXME: not portable to Win32.
    snprintf (SDL_WINDOWID, 32 * sizeof (gchar), "SDL_WINDOWID=%ld",
	      GDK_WINDOW_XWINDOW (unwrap (get_window ())));

    SDL_putenv (SDL_WINDOWID);

    HMI::Init ();

    HMI::GetRef ().SetVideoMode (get_width (), get_height ());

    do_init ();

    m_init = false;

    return true;
}
