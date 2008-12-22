#include <string>
#include <cstdlib>
#include <libxml++/nodes/textnode.h>
#include <xml_parser.h>
#include <tools/base.h>
#include <private/xml_exception.h>

#if XMLPP_MINOR < 18
#	include <libxml/tree.h>
#endif

using Glib::ustring;

namespace
{

  inline void xml_throw (const xmlpp::Node * node, const char *msg)
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
    if (!parser)
      return;
    delete parser;
    parser = NULL;
}

void
XMLParser::LoadDoc (const Glib::ustring & xmldoc)
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

const Node *
XMLParser::getFirstChild (const Node *parent) const
{
  if (!parser)
    return NULL;
  Node::NodeList list = parent->get_children();
  return (list.empty()) ? NULL : *list.begin();
}

Glib::ustring
XMLParser::getAttribute (const Node * node,
			 const Glib::ustring & att_name) const
{
    const Element *eNode = dynamic_cast < const Element * >(node);

    if (!eNode)
	xml_throw (node, " is not a Element Node");
#if XMLPP_MINOR >= 20
    return eNode->get_attribute_value (att_name);
#else
    return eNode->get_attribute (att_name)->get_value ();
#endif

}

int
XMLParser::getIntAttribute (const Node * node,
			    const Glib::ustring & att_name) const
{
  ustring att = getAttribute(node, att_name);
  char *endptr = NULL;
  int i = strtol(att.c_str(), &endptr, 10);

  if (*endptr != '\0')
    xml_throw(node, " has not a attribute of interger form (in 10 base)");
  return i;
}

//FIXME: replace get_children by get_child_text.
Glib::ustring XMLParser::getText (const Node * node) const
{
    const
	Node::NodeList
	list = node->get_children ("text");
    const TextNode *
	tNode = NULL;

    if (list.empty ())
	xml_throw (node, " has not childs");
    tNode = dynamic_cast < const TextNode *>(list.front ());

    if (!tNode)
	xml_throw (node, " has a child type != TextNode");
    return tNode->get_content ();
}

// C++ instances will be delete by libxml++ itself
// so, zero memory leaks ;)
const Node *
XMLParser::NextSibling (const Node * node) const
{
#if XMLPP_MINOR >= 18
    return node->get_next_sibling ();
#else
    const Node *sibling = NULL;

    if (!node->cobj ()->next)
	return NULL;

    switch (node->cobj ()->next->type)
      {
      case XML_TEXT_NODE:
	  sibling = new TextNode (node->cobj ()->next);
	  break;
      case XML_ELEMENT_NODE:
	  sibling = new Element (node->cobj ()->next);
	  break;
      default:
	  sibling = new Node (node->cobj ()->next);
	  break;
      }

    return sibling;
#endif
}
