#include <libxml++/nodes/textnode.h>

#include "xml_parser.h"

namespace
{

    inline void xml_throw (const Node * node, const char *msg)
    {
	throw XMLException (string (node->get_name ().c_str ()) + msg);
    }

};

XMLParser::XMLParser (void)
{
    parser = NULL;
}

XMLParser::~XMLParser (void)
{
    FreeDoc ();
}

void
XMLParser::FreeDoc (void)
{
    if (parser != NULL)
	delete parser;
}

void
XMLParser::LoadDoc (const string & xmldoc)
throw (std::exception)
{
    if (parser != NULL)
	return;
    parser = new DomParser ();
    parser->set_validate ();
    parser->parse_file (xmldoc);
}

const Node *
XMLParser::getNode (const Glib::ustring & xpath) const
{
    if (!parser)
	return NULL;
    NodeSet set = parser->get_document ()->get_root_node ()->find (xpath);
    return (set.empty ())? NULL : set[0];
}

Glib::ustring
XMLParser::getAttribute (const Node * node,
			 const Glib::ustring & att_name) const
throw (XMLException)
{
    const Element *eNode = dynamic_cast < const Element * >(node);

    if (!eNode)
	xml_throw (node, " is not a Element Node");
    return eNode->get_attribute_value (att_name);
}

Glib::ustring
XMLParser::getText (const Node * node) const throw (XMLException)
{
    const Node::NodeList list = node->get_children ("text");
    const TextNode *tNode = NULL;

    if (list.empty ())
	xml_throw (node, " has not childs");
    tNode = dynamic_cast < const TextNode *>(list.front ());

    if (!tNode)
	xml_throw (node, " has a child type != TextNode");
    return tNode->get_content ();
}
