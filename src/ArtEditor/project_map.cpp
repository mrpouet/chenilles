#include <config/xml_writer.h>
#include "project_map.h"
#include <cstdio>

void
EditableMap::add_layer (const Glib::ustring & filename)
{
    Surface layer (filename);
    LayerList::iterator it;

    m_layers.push_back (layer);
    it = m_layers.end ();
    it--;
    m_handler_it[filename] = it;

    m_main_it = it;
    m_init_draw = true;
}

void
EditableMap::set_layer_visible (const Glib::ustring & name)
{
    printf ("%s\n", name.c_str ());
    return;
}

void
EditableMap::set_layer_exclude (const Glib::ustring & name)
{
    printf ("%s\n", name.c_str ());
    return;
}

void
EditableMap::write_to_file (const Glib::ustring & filename)
{
    /*  XMLWriter &writer = XMLWriter::GetRef();
       Glib::ustring att_value;
       Map::LayerList::iterator el;
       Node *node = NULL;

       writer.CreateDoc("map");

       node = writer.GetRootNode();

       // We need to declare(in the sheet's map) ONLY visible layers
       for (LayerList::iterator it = m_layers.begin();
       it != m_layers.end();
       it++)
       {
       node = writer.AddChild(node, "layer");

       if (it == m_main_it)
       att_value.append("ground");
       else if (it == m_expl_it)
       att_value.append("explosion");
       else
       att_value.append("background");

       writer.SetAttribute(node, "type", att_value);
       att_value.erase();

       el = m_handler_it.find(*it);

       writer.SetText(node, el);

       node = node->get_parent();

       }

       writer.SaveAs(filename);

       writer.FreeDoc();

       XMLWriter::CleanUp();
     */
}

void
ProjectMap::open (const Glib::ustring & filename)
{
    m_map = EditableMap (filename);
    m_filename = filename;
}

void
ProjectMap::save_as (const Glib::ustring & filename)
{
    m_map.write_to_file (filename);
    m_filename = filename;
}
