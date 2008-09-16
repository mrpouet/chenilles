#include <string>
#include <fstream>
#include <iostream>
#include <tools/base.h>

#include <libxml/tree.h>
#include "xml_writer.h"

XMLWriter::XMLWriter ()
{
    m_doc = NULL;
}

void
XMLWriter::CreateDoc (const Glib::ustring & root)
{
    if (m_doc)
	return;
    m_doc = new Document ();
    m_doc->create_root_node (root);

    // standalone="no" (no external refs)
    m_doc->cobj ()->standalone = 0;

    // Defining DTD internal subset (on local system).
    m_doc->set_internal_subset (root, Glib::ustring (),
				Glib::ustring ("../") + root + ".dtd");
}

void
XMLWriter::FreeDoc (void)
{
    if (!m_doc)
	return;
    delete m_doc;
    m_doc = NULL;
}

Element *
XMLWriter::AddChild (Node * parent, const Glib::ustring & name)
throw (XMLException)
{
    Element *child = parent->add_child (name);

    if (!child)
	throw XMLException (string ("Unable to add child Element ")
			    + name.c_str () + " to "
			    + parent->get_name ().c_str ());
    return child;
}

void
XMLWriter::AddText (Node * parent, const Glib::ustring & content)
throw (XMLException)
{
    Element *eparent = dynamic_cast<Element *>(parent);

    if (!eparent)
	throw XMLException (string (parent->get_name ().c_str ())
			    + " is not a Element Node");
    eparent->add_child_text (content);
}


void
XMLWriter::SetAttribute (Node * node, const Glib::ustring & name,
			 const Glib::ustring & value)
throw (XMLException)
{
    Element *enode = dynamic_cast<Element *>(node);

    if (!enode)
	throw XMLException (string (node->get_name ().c_str ()) +
			    " can't contain any attributes(!= ElementNode)");
    enode->set_attribute (name, value);
}
