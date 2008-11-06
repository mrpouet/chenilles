#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <hmi.h>
#include <singleton.h>
#include <gtkmm/main.h>

#include "editor_ui.h"
#include "project_map.h"
#include <tools/base.h>

#define EDITORDATA DATA "/ArtEditor/"

using std::list;
using Gtk::Main;

class Editor:public Singleton<Editor >
{

  public:

    // Create a new project
    // as default current project editor is switched to it
    inline void new_project (void)
    {
	m_project_handler.push_back (new ProjectMap ());
	switch_to_new_project ();

    }

    // Open a project
    // @param filename The XML sheet project path.
    inline void open_project (const Glib::ustring & filename)
    {
      (*m_current_project)->open(filename);
    }


    // Save the current project
    // @param filename The filename which current project'll be save
    inline void save_project_as (const Glib::ustring & filename) const
    {
	(*m_current_project)->save_as (filename);
    }

    inline Project & get_current_project (void)
    {
	return **m_current_project;
    }


    // Run Editor
    // Note: Same as Gtk::Main::run().
    inline void run (void)
    {
	Main::run (m_gui);
    }

  private:

    Editor ();
    ~Editor ();
    friend class Singleton<Editor >;

    inline void switch_to_new_project (void)
    {
	m_current_project = m_project_handler.end ();
	m_current_project--;
    }

    bool editor_refresh (void);

    inline void on_init (void)
    {
	HMI::GetRef ().set_external_tip ();
	Glib::signal_timeout ().connect (sigc::mem_fun (*this,
					 &Editor::editor_refresh), 15);
    }

    EditorUI m_gui;

    typedef list<Project *> ProjectList;
    ProjectList m_project_handler;
    ProjectList::iterator m_current_project;

};

#endif
