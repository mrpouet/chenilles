#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <tools/singleton.h>
#include <gtkmm/main.h>

#include "editor_ui.h"
#include "project_map.h"

using std::list;
using Gtk::Main;

class Editor:public Singleton<Editor>
{

  public:

    // Create a new project
    // as default current project editor is switched to it
    inline void
    new_project (void)
    {
	m_project_handler.push_back (new ProjectMap ());
	switch_to_new_project ();

    }

    // Open a project
    // @param filename The XML sheet project path.
    void open_project (const Glib::ustring & filename);


    // Save the current project
    // @param filename The filename which current project'll be save
    inline void
    save_project_as (const Glib::ustring & filename) const
    {
	(*m_current_project)->save_as (filename);
    }

    // Add a layer to the current project
    // @param filename The path from which layer'll be instance and load
    void add_layer_to_project (const Glib::ustring & filename);


    // Run Editor
    // Note: Same as Gtk::Main::run().
    inline void run (void)
    {
	Main::run (m_gui);
    }

  private:

    Editor ();
    ~Editor ();
    friend class Singleton<Editor>;

    inline void switch_to_new_project (void)
    {
	m_current_project = m_project_handler.end ();
	m_current_project--;
    }

    bool editor_refresh (void);

    EditorUI m_gui;

    typedef list <Project *> ProjectList;
    ProjectList m_project_handler;
    ProjectList::iterator m_current_project;

};

#endif
