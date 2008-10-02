#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <string>
#include <libxml++/parsers/domparser.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/textnode.h>
#include <tools/singleton.h>
#include <tools/base.h>

#include "xml_exception.h"

using namespace xmlpp;

class XMLParser:public Singleton<XMLParser>
{
  public:

    // Load a context for the singleton XMLParser object, with
    // the following file.
    void LoadDoc (const string & xmldoc) throw (std::exception);

    // Free the current context
    void FreeDoc (void);

    // Returns the Node corresponding to the 'xpath' syntax
    const Node *getNode (const Glib::ustring & xpath) const;

    // Returns the attribute value named "att_name" in "node" Node
    Glib::ustring getAttribute (const Node * node,
				const Glib::ustring & att_name) const
	throw (XMLException);

    // Get the text content of a Element Node
    Glib::ustring getText (const Node * node) const throw (XMLException);

    
    inline bool isTextNode (const Node * node) const
    {
	return dynamic_cast < const TextNode *>(node);
    }

    // Get the sibling of a node
    // @param node The node which we want the sibling
    // @return the node sibling of "node".
    const Node *NextSibling (const Node * node) const;

  private:

    // The parser context
    DomParser * parser;
    XMLParser (void);
    ~XMLParser (void);
    friend class Singleton<XMLParser>;

};

#endif
