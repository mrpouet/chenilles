#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <libxml++/parsers/domparser.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/textnode.h>
#include <singleton.h>

namespace Chenilles
{

    using namespace xmlpp;

    class XMLParser:public Singleton < XMLParser >
    {
      public:

	// Load a XML Document, parse it, and load
	// the corresponding XML Tree and Parser context into memory.
        // If you try to load an other document when a existing is loaded
        // (and not freed) ask is reject (do nothing).
        // @throw exception
	// @param xmldoc The absolute path to a XML Document
        void LoadDoc (const Glib::ustring & xmldoc);

	// Free the current context
	void FreeDoc (void);

	// Get the Node corresponding to the following XPath syntax.
	// @param xpath A XPath syntax
	// @return The Node corresponding to XPath syntax, if
	// this path exist in tree, NULL otherwises.
	const Node *getNode (const Glib::ustring & xpath) const;

	const Node *getFirstChild(const Node *parent) const;

	// Get the value of following attribute in a Node
	// @throw exception
	// @param node The Node which contains the attribute
	// @param att_name The attribute name
	// @return The value of the mentionned attribute
	  Glib::ustring getAttribute (const Node * node,
				      const Glib::ustring & att_name) const;

	  int getIntAttribute (const Node *node, 
			       const Glib::ustring & att_name) const;

	// Get the text content of a Element Node
	// @throw exception
	// @param node The Node in which take the value
	// @return The content of the TextNode contained in node.
	  Glib::ustring getText (const Node *node) const;


	// The following node is a TextNode ?
	// @param node The Node about we want information type.
	// @return true if node is a TextNode, false otherwises.
	inline bool isTextNode (const Node * node) const
	{
	    return dynamic_cast < const TextNode *>(node);
	}

	// Get the sibling of a node
	// @param node The node which we want the sibling
	// @return the node sibling of "node".
	const Node *NextSibling (const Node * node) const;

      private:

	  DomParser * parser;

	  XMLParser (void);

	 ~XMLParser (void);

	friend class Singleton < XMLParser >;

    };

};

#endif
