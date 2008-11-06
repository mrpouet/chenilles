#include <iostream>
#include <camera.h>
#include <game_exception.h>
#include <tools/base.h>

#include "editor.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

Editor::Editor ():
m_gui (DEFAULT_WIDTH, DEFAULT_HEIGHT),
m_current_project (m_project_handler.end ())
{
    new_project ();
    m_gui.signal_init().connect(sigc::mem_fun(*this, &Editor::on_init));
}

Editor::~Editor ()
{
    for (ProjectList::const_iterator it = m_project_handler.begin ();
	 it != m_project_handler.end (); it++)
	delete *it;
}

bool Editor::editor_refresh (void)
{
    Camera::GetRef ().Refresh ();
    (*m_current_project)->get_drawable ().draw ();

    HMI::GetRef ().RefreshOutput ();

    return true;
}

