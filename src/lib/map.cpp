#include <cstdlib>
#include <xml_parser.h>
#include <camera.h>
#include <map.h>
#include <game_exception.h>
#include <tools/base.h>

namespace
{
    inline void AbsolutePath (string & str)
    {
	str.erase (str.rfind ("/") + 1, string::npos);
    }

    class MapException:public GameException
    {
      public:

	explicit MapException (const std::string & msg) throw ():
	  GameException (msg)
	{}

	const char *what (void) const throw ()
	{
	    return "Map exception (MapException)";
	}

    };
};

Map::Map ()
{
    m_main_it = m_expl_it = m_layers.end ();
    m_init_draw = true;
}

Map::Map (const string & xmldoc)
{
    XMLParser *parser = XMLParser::GetInstance ();
    string path (xmldoc);

    m_main_it = m_expl_it = m_layers.end ();

    parser->LoadDoc (xmldoc);

    for (const Node * n = parser->getNode ("//author"); n != NULL;
	 n = parser->NextSibling (n))
      {
	  // n must be NULL, on optionnal nodes.
	  // (specified in the DTD XML).
	  if (parser->isTextNode (n) || (n == NULL))
	      continue;
	  m_infos.push_back (parser->getText (n));

      }

    AbsolutePath (path);

    for (const Node * n = parser->getNode ("//layer");
	 n != NULL; n = parser->NextSibling (n))
      {
	  if (parser->isTextNode (n))
	      continue;

	  m_layers.push_back (Surface::CreateFromFile (path
						       +
						       parser->getText (n).
						       c_str ()));
	  m_layers.back ().DisplayFormatAlpha ();
	  AbsolutePath (path);

	  // layers iterator are already to the sentinel of the container
	  // so we just need to decreasing it.
	  // (to access to the last element).
	  if (parser->getAttribute (n, "type") == "ground")
	      m_main_it--;
	  else if (parser->getAttribute (n, "type") == "explosion")
	      m_expl_it--;

      }

    if ((m_main_it == m_layers.end ()) || (m_expl_it == m_layers.end ()))
	throw MapException ("No \"main\" or \"explosion\" layer type found");

    parser->FreeDoc ();
    XMLParser::CleanUp ();
    m_init_draw = true;

}

// Exactly the same complexity as std::list copy constructor
// (linear time of size list).
// We need to do this, due to iterators member.

Map & Map::operator= (const Map & map)
{

    m_layers.clear ();

    m_infos = map.m_infos;

    m_init_draw = map.m_init_draw;

    m_main_it = m_expl_it = m_layers.end ();

    for (LayerList::const_iterator it = map.m_layers.begin ();
	 it != map.m_layers.end (); it++)
      {
	  m_layers.push_back (*it);

	  if (it == map.m_main_it)
	      m_main_it--;
	  else if (it == map.m_expl_it)
	      m_expl_it--;
      }

    return *this;
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
    Surface tmp = Surface::CreateRGB (Rectangle (0, 0, w, camera_box.h));

    // If borner right or borner left of camera 
    // try to go out of the world, do nothing.
    if ((IsOutOfWorldX (camera_box.x + camera_box.w) && (scroll > 0)) ||
	(IsOutOfWorldX (camera_box.x) && (scroll < 0)))
      {
	  // And canceling last Camera movement
	  camera.CancelMove ();
	  return;
      }

    if (camera.IsResized ())
	m_init_draw = true;

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

    for (LayerList::const_iterator it = m_layers.begin ();
	 it != m_layers.end (); it++)
      {
	  if (it == m_expl_it)
	      continue;
	  camera.UpdateCamera (*it, &to, &from);
      }

    camera.ToRedraw (Rectangle (0, 0, camera_box.w, camera_box.h));
    m_init_draw = false;
}
