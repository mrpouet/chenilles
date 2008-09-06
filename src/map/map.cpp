#include <config/xml_parser.h>
#include "map_exception.h"
#include "map.h"

namespace
{
    inline const Node *getLayer (const string & xpath)
    {
	const Node *node = XMLParser::GetInstance ()->getNode (xpath);
	if (!node)
	    throw MapException ("No <layer> node corresponding to \'" + xpath
				+ "\' XPath syntax");
	return node;

    }

};

Map::Map (const string & xmldoc) throw (std::exception)
{
    XMLParser *parser = XMLParser::GetInstance ();
    const Node *node = NULL;
    string path (xmldoc);

    parser->LoadDoc (xmldoc);

    path.erase (path.rfind ("/") + 1, string::npos);

    node = getLayer ("//layer[@type=\"sky\"]");

    m_sky = Surface (path + parser->getText (node).c_str ());

    node = getLayer ("//layer[@type=\"ground\"]");

    m_ground = Surface (path + parser->getText (node).c_str ());

    node = getLayer ("//layer[@type=\"foreground\"]");

    m_foreground = Surface (path + parser->getText (node).c_str ());

    node = getLayer ("//layer[@type=\"explosion\"]");

    m_explosion = Surface (path + parser->getText (node).c_str ());

    parser->FreeDoc ();
}

void
Map::Draw (Surface & s) const
{
    s.Blit (m_sky);
    s.Blit (m_ground);
    s.Blit (m_foreground);
}
