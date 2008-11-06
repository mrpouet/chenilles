#include <xml_writer.h>
#include "editor.h"
#include "project_map.h"

using Glib::ustring;

namespace
{
    inline Glib::ustring layername_from_path (const ustring & path)
    {
	int id = path.rfind ('/') + 1;
	return path.substr (id, path.length () - id
			    - (path.length () - path.rfind ('.')));
    }
};

void
container_builder (const Glib::ustring & path)
{
    Drawable & d = Editor::GetRef ().get_current_project ().get_drawable ();
    EditableMap & map = static_cast < EditableMap & >(d);

    ustring file = layername_from_path (path);
    map.m_Ltable[file.raw ()] = --(map.m_layers.end ());
    map.m_files_list.push_back (file);
}

EditableMap::EditableMap (const Glib::ustring & xmldoc)
{
    CreateFromXML (xmldoc, container_builder);
}

void
EditableMap::open (const Glib::ustring & xmldoc)
{
    CreateFromXML (xmldoc, container_builder);
}

void
EditableMap::add_layer (const Glib::ustring & path)
{
    Surface layer = Surface::CreateFromFile (path);
    Map::LayerList::iterator it;
    ustring file = layername_from_path (path);

    m_layers.push_back (layer);
    it = --m_layers.end ();

    m_Ltable[file.raw ()] = it;
    m_files_list.push_back (file);

    // Current EditableMap contains nothing, so
    // we need truncate the main layer iterator
    // to avoiding conflicts with Map::draw().
    // (see the source code).
    if (m_main_it == m_layers.end ())
	m_main_it = it;
    m_init_draw = true;

}

void
EditableMap::set_layer_spec (const Glib::ustring & layername,
			     const Glib::ustring & data)
{
    if (data == "main")
      {
	  // Switch layer from not visible to visible
	  // so we need to redraw entire map to display it.
	  if (get_layer_spec (layername) == "explosion")
	      m_init_draw = true;
	  m_main_it = m_Ltable[layername.raw ()];
      }
    else if (data == "explosion")
      {

	  m_expl_it = m_Ltable[layername.raw ()];
	  m_init_draw = true;
      }

    if (m_init_draw)
	draw ();

}

void
EditableMap::write_to_file (const Glib::ustring & filename)
{
    XMLWriter & writer = XMLWriter::GetRef ();
    Glib::ustring att_value;
    std::list < ustring >::const_iterator itlist = m_files_list.begin ();
    Node *node = NULL;
    const char *informations[] = { "author", "name", "description" };

    writer.CreateDoc ("map");

    node = writer.GetRootNode ();

    node = writer.AddChild (node, "informations");

    for (unsigned int i = 0; i < m_infos.size (); i++)
      {
	  node = writer.AddChild (node, informations[i]);
	  writer.AddText (node, m_infos[i]);

	  node = node->get_parent ();
      }

    node = writer.GetRootNode ();

    for (Map::LayerList::iterator it = m_layers.begin ();
	 (it != m_layers.end ()) && (itlist != m_files_list.end ());
	 it++, itlist++)
      {
	  node = writer.AddChild (node, "layer");

	  if (it == m_main_it)
	      att_value.append ("ground");
	  else if (it == m_expl_it)
	      att_value.append ("explosion");
	  else
	      att_value.append ("background");

	  writer.SetAttribute (node, "type", att_value);
	  att_value.erase ();

	  writer.AddText (node, (*itlist));

	  node = node->get_parent ();

      }

    writer.SaveAs (filename);

    writer.FreeDoc ();

    XMLWriter::CleanUp ();

}

/*
 * ProjectMap class methods definitions
 */

void
ProjectMap::open (const Glib::ustring & filename)
{
    m_map.open (filename);
    m_filename = filename;
}

void
ProjectMap::save_as (const Glib::ustring & filename)
{
    m_map.write_to_file (filename);
    m_filename = filename;
}
