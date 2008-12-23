#include <cstdlib>
#include <xml_parser.h>
#include <camera.h>
#include <hmi.h>
#include <map.h>
#include <game_exception.h>
#include <tools/base.h>
#include <cstdio>

using
    Glib::ustring;

namespace
{
    inline void
    AbsolutePath (ustring & str)
    {
	str.erase (str.rfind ("/") + 1, ustring::npos);
    }

    class MapException: public GameException
    {
      public:

	explicit MapException (const std::string & msg) throw ():
	  GameException (msg)
	{
	}

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
    CreateFromXML (xmldoc);
}

void
Map::CreateFromXML (const string & xmldoc)
{
    XMLParser *parser = XMLParser::GetInstance ();
    ustring path (xmldoc);
    ustring attr;

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
	  path += parser->getText (n);
	  m_layers.push_back (Surface::CreateFromFile (path));
	  attr = parser->getAttribute (n, "type");

	  layer_add_vfunc (path);

	  if (attr != "explosion")
	      m_layers.back ().DisplayFormatAlpha ();
	  AbsolutePath (path);

	  // layers iterator are already to the sentinel of the container
	  // so we just need to decreasing it.
	  // (to access to the last element).
	  if (attr == "ground")
	      m_main_it--;
	  else if (attr == "explosion")
	      m_expl_it--;

      }

    if ((m_main_it == m_layers.end ()) || (m_expl_it == m_layers.end ()))
	throw MapException ("No \"main\" or \"explosion\" layer type found");

    parser->FreeDoc ();
    XMLParser::CleanUp ();

    m_init_draw = true;
}

void
Map::draw (void)
{
    Camera & camera = Camera::GetRef ();
    HMI & hmi = HMI::GetRef ();
    const Rectangle & objective = camera.getObjective ();
    rectangle from = { 0, 0, 0, 0 };
    rectangle to = from;
    Uint16 out_of_range_width = 0;
    Sint16 obj_x = 0;

    // If borner right or borner left of camera 
    // try to go out of the world, do nothing.
    if (IsOutOfWorldY (objective.y + objective.h) ||
	IsOutOfWorldY (objective.y))
      {
	  // And canceling last Camera movement
	  camera.unScroll ();
	  return;
      }

    if (camera.wasScrolled ())
	m_init_draw = true;

    if (!m_init_draw)
	return;

    // FIX IT !
    // Avoiding integer overflow (really underflow)
    //obj_x = (objective.x <= -objective.w) ? WidthOfWorld() - objective.x :
    //objective.x;

    camera.setObjectiveCoords (obj_x % WidthOfWorld (), objective.y);

    from = objective.GetSDLRect ();
    to.w = objective.w;
    to.h = objective.h;

    if (IsOutOfWorld(objective.x) ||
	IsOutOfWorld(objective.x + objective.w))
      {
	if ((objective.x + objective.w) > WidthOfWorld())
	  camera.setObjectiveCoords(objective.x - WidthOfWorld(), objective.y);

	if (objective.x < 0)
	  {

	    out_of_range_width = -objective.x;
	    to.x = out_of_range_width;
	    to.w -= out_of_range_width;
	    from.x = 0;
	    from.w = to.w;
	    
	    redrawRegion(&to, &from);

	    from.x = WidthOfWorld() - out_of_range_width;
	    from.w = out_of_range_width;
	    
	    to.x = 0;
	    to.w = out_of_range_width;

	    redrawRegion(&to, &from);
	  }
	
      }
    else
      redrawRegion(&to, &from);

    printf("camera X: %d\n", objective.x);

    hmi.ToRedraw (Rectangle (0, 0, objective.w, objective.h));
    m_init_draw = false;
}

void
Map::redrawRegion (rectangle *to, rectangle *from)
{
  for (LayerList::const_iterator it = m_layers.begin ();
       it != m_layers.end (); it++)
    {
      if (it == m_expl_it)
	continue;
      HMI::GetRef().BlitOnScreen (*it, to, from);
    }
}

double
Map::computeAngle (const Point & pos)
{
    Point tmppos (pos.x + 5, pos.y);

    while (isTheVacuum (tmppos))
	tmppos.y++;
    while (isTheGround (tmppos))
	tmppos.y--;
    return pos.Tangente (tmppos);
}
