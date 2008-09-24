#include <iostream>
#include <cstdlib>
#include <config/xml_parser.h>
#include <interface/camera.h>

#include "map_exception.h"
#include "map.h"

namespace
{
  inline void AbsolutePath(string& str)
  {
    str.erase (str.rfind ("/") + 1, string::npos);
  }
  
};

Map::Map (const string & xmldoc) throw (std::exception)
{
    XMLParser *parser = XMLParser::GetInstance ();
    string path (xmldoc);
    int i = 0;
    
    m_main_id = m_expl_id = 0;

    parser->LoadDoc (xmldoc);

    AbsolutePath(path);

    for (const Node *n = parser->getNode("//layer"); 
	 n != NULL; n = n->get_next_sibling())
      {
	if (parser->isTextNode(n))
	  continue;

	if (parser->getAttribute(n, "type") == "ground")
	  m_main_id = i;
	else if (parser->getAttribute(n, "type") == "explosion")
	  m_expl_id = i;

	m_layers.push_back(Surface(path + parser->getText(n).c_str()));
	m_layers.back().DisplayFormatAlpha();
	AbsolutePath(path);
	i++;
      }

    if (!m_main_id || !m_expl_id)
      throw MapException("No \"main\" or \"explosion\" layer type found");

    parser->FreeDoc ();
    m_init_draw = true;
}

void
Map::draw (void)
{
    Camera & camera = Camera::GetRef ();
    const Rectangle & camera_box = camera.GetCameraBox ();
    rectangle from = { 0, 0, 0, 0 };
    rectangle to = from;
    rectangle *adr = NULL;
    int scroll = camera.GetSpeed ();
    int width = (scroll == 0) ? camera_box.w : abs (scroll);
    int w = camera_box.w - abs (scroll);
    Surface tmp (Rectangle (0, 0, w, camera_box.h));

    // If borner right or borner left of camera 
    // try to go out of the world, do nothing.
    if (IsOutOfWorldX (camera_box.x + camera_box.w) ||
	IsOutOfWorldX (camera_box.x))
      {
	  // And canceling last Camera movement
	  camera.CancelMove();
	  return;
      }

    if ((!scroll) && (!m_init_draw))
      return;

    // A king of "slicing" of the part which won't change
    // (just move).

    from.x = (scroll > 0) ? scroll : 0;
    from.w = w;
    from.h = camera_box.h;

    if (scroll < 0)
      {
	  to.x = -scroll;
	  to.w = w;
	  to.h = from.h;
	  adr = &to;
      }

    //TODO: It's possible to shift camera on itself 
    // just from right to left... why ? 
    tmp.Blit (camera.GetCamera (), NULL, &from);
    tmp.Flip ();

    camera.UpdateCamera (tmp, adr);

    // Computing and extract only the new part to draw
    // (tipically only a number of "scroll" pixels)

    // Note: if camera_box.x == 0 then scroll is
    // necessary equals to zero too.

    from.x = (scroll > 0) ?
	((camera_box.x + camera_box.w) - scroll) : camera_box.x;

    from.w = width;

    to.x = (scroll <= 0) ? 0 : w;
    to.w = width;
    to.h = from.h;

    for (unsigned int i = 0; i < m_layers.size(); i++)
      {
	// We need to exclude explosion layer on drawing
	// (just need for compute)
	if (i == m_expl_id)
	  continue;
	camera.UpdateCamera (m_layers[i], &to, &from);
      }
   
    camera.ToRedraw(Rectangle(0, 0, camera_box.w, camera_box.h));
    m_init_draw = false;
}

