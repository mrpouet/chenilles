#include <iostream>
#include <interface/camera.h>
#include <interface/hmi.h>
#include <tools/game_exception.h>

#include "editor.h"
#include <cstdio>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

namespace
{
    inline void on_init (void)
    {
	HMI::GetRef ().set_external_tip ();
    }

    inline void cursor_init (void)
    {
	HMI::GetRef ().SetCursor (HMI::CURSOR_TIP,
				  "../../data/menu/cursor_main.png");
	HMI::GetRef ().SwitchToCursor (HMI::CURSOR_TIP);
    }

};

Editor::Editor ():
m_gui (DEFAULT_WIDTH, DEFAULT_HEIGHT),
m_current_project (m_project_handler.end ())
{

    Glib::signal_timeout ().connect (sigc::mem_fun (*this,
						    &Editor::editor_refresh),
				     20);
    m_gui.signal_init ().connect (sigc::ptr_fun (on_init));

}

Editor::~Editor ()
{
    for (ProjectList::const_iterator it = m_project_handler.begin ();
	 it != m_project_handler.end (); it++)
	delete *it;
}

bool Editor::editor_refresh (void)
{
    // ProjectList contains zero project opened, nothing to do.
    if (m_project_handler.empty ())
	return true;
    Camera::GetRef ().Refresh ();
    (*m_current_project)->get_drawable ().draw ();
    HMI::GetRef ().RefreshOutput ();
    return true;
}

void
Editor::open_project (const Glib::ustring & filename)
{
    Project *p = NULL;

    // Initial project, we need to init HMI Cursor,
    // which will draw funny tip on the drawable ;)
    if (m_project_handler.empty ())
	cursor_init ();
    p = new ProjectMap ();

    try
    {
	p->open (filename);
    }
    catch (const GameException & e)
    {
	cerr << "Caught exception:" << endl
	    << "what  : " << e.what () << endl
	    << "error : " << e.error () << endl;
    }

    m_project_handler.push_back (p);
    switch_to_new_project ();

}

void
Editor::add_layer_to_project (const Glib::ustring & filename)
{
    if ((*m_current_project)->get_drawable ().empty ())
	cursor_init ();
    (*m_current_project)->get_drawable ().add_layer (filename);
}
