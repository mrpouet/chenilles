#include <iostream>
#include <cstdlib>
#include <config/xml_parser.h>
#include <interface/camera.h>

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

    // Absurd (use ConvertSurface instead)
    m_sky.DisplayFormatAlpha ();
    m_ground.DisplayFormatAlpha ();
    m_foreground.DisplayFormatAlpha ();

    parser->FreeDoc ();
}

void
Map::draw (void) const
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
	  camera.CancelMove();
	  return;
      }

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

    camera.UpdateCamera (m_sky, &to, &from);
    camera.UpdateCamera (m_ground, &to, &from);
    camera.UpdateCamera (m_foreground, &to, &from);

}
