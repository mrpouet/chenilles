#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <string>
#include <libxml++/parsers/domparser.h>
#include <libxml++/nodes/node.h>
#include <tools/singleton.h>
#include <tools/base.h>

#include "xml_exception.h"

using namespace xmlpp;

class XMLParser:public Singleton < XMLParser >
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
				const Glib::ustring& att_name) const
	throw (XMLException);

    // Get the text content of a Element Node
      Glib::ustring getText (const Node * node) const throw (XMLException);

  private:

    // The parser context
    DomParser * parser;
    XMLParser (void);
    ~XMLParser (void);
    friend class Singleton<XMLParser>;

};

#endif
