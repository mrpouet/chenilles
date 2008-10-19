#include <iostream>
#include <camera.h>
#include <game_exception.h>
#include <tools/base.h>

#include "editor.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

namespace
{

    inline void cursor_init (void)
    {
	//FIXME: Replace path cursor by:
	// ustring(DATA_GUI) + "cursor_main.png".
	// where DATA_GUI is a macro-constante defines in HMI 
	HMI::GetRef ().SetCursor (HMI::CURSOR_TIP,
				  Glib::ustring(UIDATA) + "cursor_main.png");
	HMI::GetRef ().SwitchToCursor (HMI::CURSOR_TIP);
    }

};


Editor::Editor ():
m_gui (DEFAULT_WIDTH, DEFAULT_HEIGHT),
m_current_project (m_project_handler.end ())
{

    m_gui.signal_init ().connect (sigc::mem_fun (*this, &Editor::on_init));

    new_project ();

}

Editor::~Editor ()
{
    for (ProjectList::const_iterator it = m_project_handler.begin ();
	 it != m_project_handler.end (); it++)
	delete *it;
}

bool
Editor::editor_refresh (void)
{
    Camera::GetRef ().Refresh ();
    (*m_current_project)->get_drawable ().draw ();

    HMI::GetRef ().RefreshOutput ();

    return true;
}

void
Editor::open_project (const Glib::ustring & filename)
{
    if ((m_project_handler.size () == 1) &&
	(*m_current_project)->get_drawable ().empty ())
	cursor_init ();

    //TODO: Add tabs later.
    (*m_current_project)->open (filename);

}

Drawable::iterator
    Editor::add_layer_to_drawable_project (const Glib::ustring & filename)
{
    if ((*m_current_project)->get_drawable ().empty ())
	cursor_init ();
    return (*m_current_project)->get_drawable ().add_layer (filename);
}
