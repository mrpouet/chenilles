#include <cstdlib>
#include <xml_parser.h>
#include <camera.h>
#include <hmi.h>
#include <map.h>
#include <game_exception.h>
#include <tools/base.h>
#include <cstdio>
#include <cmath>
#include <SDL/SDL_byteorder.h>


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

    // Avoiding integer overflow (really underflow)
    obj_x = (objective.x <= -objective.w) ? WidthOfWorld() + objective.x :
      objective.x;

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

bool 
Map::contiguousPoint(const Point& pos, Point &p)
{
  Color c = m_main_it->GetPixColor(Point(0, 0));
  printf("%s: couleur (r: %u, v: %u, b: %u, a: %u)\n", __func__
	 , c.GetR(), c.GetG(), c.GetB(), c.GetA());

  // Top left pixel
  if (!isEmpty(pos.x - 1, pos.y - 1) &&
      (isEmpty(pos.x, pos.y - 1) ||
       isEmpty(pos.x - 1, pos.y)))
    {
      p.x = pos.x - 1;
      p.y = pos.y - 1;
      return true;
    }
  // Top pixel
  if (!isEmpty(pos.x, pos.y - 1) &&
      (isEmpty(pos.x - 1, pos.y - 1) ||
       isEmpty(pos.x + 1, pos.y - 1)))
    {
      p.x = pos.x;
      p.y = pos.y - 1;
      return true;
    }

  // Top right pixel
  if (!isEmpty(pos.x + 1, pos.y - 1) &&
      (isEmpty(pos.x, pos.y - 1) ||
       isEmpty(pos.x - 1, pos.y - 1)))
    {
      p.x = pos.x + 1;
      p.y = pos.y - 1;
      return true;
    }

   // Right pixel
    if (!isEmpty(pos.x + 1, pos.y) &&
	(isEmpty(pos.x + 1, pos.y - 1) ||
	 isEmpty(pos.x + 1, pos.y + 1)))
      {
	p.x = pos.x + 1;
	p.y = pos.y;
	return true;
      }
    
    // Bottom right pixel
    if (!isEmpty(pos.x + 1, pos.y + 1) &&
	(isEmpty(pos.x + 1, pos.y) ||
	 isEmpty(pos.x, pos.y + 1)))
      {
	p.x = pos.x + 1;
	p.y = pos.y + 1;
	return true;
      }

    // Bottom pixel
    if (!isEmpty(pos.x, pos.y + 1) &&
	(isEmpty(pos.x - 1, pos.y + 1) ||
	 isEmpty(pos.x + 1, pos.y + 1)))
      {
	p.x = pos.x;
	p.y = pos.y + 1;
	return true;
      }

    // Bottom left pixel
    if (!isEmpty(pos.x - 1, pos.y + 1) &&
	(isEmpty(pos.x - 1, pos.y) ||
	 isEmpty(pos.x, pos.y + 1)))
      {
	p.x = pos.x - 1;
	p.y = pos.y + 1;
	return true;
      }

    // Left pixel
    if (!isEmpty(pos.x - 1, pos.y) &&
	(isEmpty(pos.x - 1, pos.y - 1) ||
	 isEmpty(pos.x - 1, pos.y + 1)))
      {
	p.x = pos.x - 1;
	p.y = pos.y;
	return true;
      }
    return false;
}

double
Map::computeAngle (const Point & pos, Uint8 born)
{
  Point p2(pos.x, pos.y - born / 2);
  Point p1(p2.x, p2.y - born);
  double delta_x = 0.0;
  double delta_y = 0.0;


  while ((p1.x < (pos.x + born)) && isTheVacuum(p1))
    p1.x++;
  while ((p2.x < (pos.x + born)) && isTheVacuum(p2))
    p2.x++;

  if ((p2.x == p1.x) && (p1.x == (pos.x + born)))
    {
      Point p3 (p2);

      while (isTheVacuum(p3))
	p3.y++;
      delta_y = (p3.y - pos.y);
      delta_x = born;

      printf("hauteur %lf\n", pos.Distance(p3));

      return atan(delta_y / delta_x);
    }
  else if (p2.x == p1.x)
    return -M_PI_2;
  return 0.0;
}
