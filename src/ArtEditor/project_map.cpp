#include <xml_writer.h>
#include "editor.h"
#include "project_map.h"

using Glib::ustring;


EditableMap::EditableMap (const Glib::ustring & xmldoc)
{
    open (xmldoc);
}

void
EditableMap::open (const Glib::ustring & xmldoc)
{
    CreateFromXML (xmldoc);
}

void
EditableMap::add_containers_entry (const Glib::ustring & path)
{
  m_files_list.push_back (layerfilename_from_path (path));
  m_Ltable[layername_from_path (path).raw ()] = 
    make_pair (--m_layers.end (), --m_files_list.end ()); 
}

void
EditableMap::add_layer (const Glib::ustring & path)
{
    Surface layer = Surface::CreateFromFile (path);
    Map::LayerList::iterator it;

    m_layers.push_back (layer);
    it = --m_layers.end ();

    add_containers_entry(path);

    // Current EditableMap contains nothing, so
    // we need truncate the main layer iterator
    // to avoiding conflicts with Map::draw().
    if (m_main_it == m_layers.end ())
	m_main_it = it;
    m_init_draw = true;

}

void
EditableMap::splice_layer (const Glib::ustring & dest,
			   const Glib::ustring & src)
{
    Map::LayerList::iterator layer_it_from = m_Ltable[src.raw ()].first;
    Map::LayerList::iterator layer_it_to = m_Ltable[dest.raw ()].first;

    FilesList::iterator file_it_from = m_Ltable[src.raw ()].second;
    FilesList::iterator file_it_to = m_Ltable[dest.raw ()].second;

    layer_it_to++;
    file_it_to++;

    // Please not that we splice a element into the same list
    // so, iterators will be valid after this call.
    m_layers.splice (layer_it_to, m_layers, layer_it_from);

    m_files_list.splice (file_it_to, m_files_list, file_it_from);

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
	  m_main_it = m_Ltable[layername.raw ()].first;
      }
    else if (data == "explosion")
      {

	  m_expl_it = m_Ltable[layername.raw ()].first;
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
