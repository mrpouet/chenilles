#ifndef __XML_WRITER_H__
#define __XML_WRITER_H__

#include <libxml++/document.h>

#include <singleton.h>

namespace Chenilles
{

    using namespace xmlpp;

    class XMLWriter:public Singleton < XMLWriter >
    {
      public:

	// Create a XML tree with the following root node.
	// @param root The Root node name.
	void CreateDoc (const Glib::ustring & root);

	// Free the current context
	void FreeDoc (void);

	// Save the internal generated XML tree to a formatted file.
	// @param filename The filename into tree'll be save.
	// @param encoding The encoding file (UTF-8 as default).
	inline void SaveAs (const Glib::ustring & filename,
			    const Glib::ustring & encoding =
			    Glib::ustring ("UTF-8"))
	{
	    if (m_doc)
		m_doc->write_to_file_formatted (filename, encoding);
	}

	// Get the root Node of the current XML tree.
	// @return a pointer from to root node.
	inline Node *GetRootNode (void) const
	{
	    return (m_doc) ? m_doc->get_root_node () : NULL;
	}

	// Add a child Element, to "parent" Node.
	// @throw exception
	// @param parent The Node to add the child.
	// @param name The child name.
	// @returns the newly-created child.
	Element *AddChild (Node * parent, const Glib::ustring & name);

	// Add a child Text, to "parent" Node.
	// @throw exception
	// @param parent The Node to add the child.
	// @param content The content of the child text.
	void AddText (Node * parent, const Glib::ustring & content);


	// Add New Attribute to following node.
	// @throw exception
	// @param node The Node in which attribute will be set
	// @param name The attribute name.
	// @param value The attribute value.
	void SetAttribute (Node * node, const Glib::ustring & name,
			   const Glib::ustring & value);

      private:

	  XMLWriter ();
	 ~XMLWriter ();

	friend class Singleton < XMLWriter >;

	Document *m_doc;

    };

};

#endif
