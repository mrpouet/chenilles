#include <config/xml_writer.h>
#include "project_map.h"

Drawable::iterator EditableMap::add_layer (const Glib::ustring & filename)
{
    Surface
    layer (filename);
    Drawable::iterator it;

    m_layers.push_back (layer);
    it = m_layers.end ();
    it--;

    // Current EditableMap contains nothing, so
    // we need truncate the main layer iterator
    // to avoiding conflicts with Map::draw().
    // (see the source code).
    if (m_main_it == m_layers.end ())
	m_main_it = it;
    m_init_draw = true;

    return it;

}

void
EditableMap::set_layer (const Drawable::iterator & it,
			const Glib::ustring & data)
{
    if (data == "main")
	m_main_it = it;
    else if (data == "explosion")
      {
	  m_expl_it = it;
	  m_init_draw = true;
	  draw ();
      }


}

void
EditableMap::write_to_file (const Glib::ustring & filename,
			    const std::list < Glib::ustring > &l)
{
    XMLWriter & writer = XMLWriter::GetRef ();
    Glib::ustring att_value;
    std::list < Glib::ustring >::const_iterator itlist = l.begin ();
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

    for (LayerList::iterator it = m_layers.begin ();
	 (it != m_layers.end ()) && (itlist != l.end ()); it++, itlist++)
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

	  writer.AddText (node, Glib::ustring (*itlist) + ".png");

	  node = node->get_parent ();

      }

    writer.SaveAs (filename);

    writer.FreeDoc ();

    XMLWriter::CleanUp ();

}

void
ProjectMap::open (const Glib::ustring & filename)
{
    m_map = EditableMap (filename);
    m_filename = filename;
}

void
ProjectMap::save_as (const Glib::ustring & filename,
		     const std::list < Glib::ustring > &l)
{
    m_map.write_to_file (filename, l);
    m_filename = filename;
}
