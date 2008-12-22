#include <timer.h>
#include <hmi.h>
#include <camera.h>
#include <animation.h>
#include <xml_parser.h>
#include <game_exception.h>
#include <tools/base.h>

#define DEFAULT_WIDTH_CACHE 64
#define DEFAULT_HEIGHT_CACHE 64

namespace
{
    class AnimationException:public GameException
    {
      public:
	AnimationException (const string & msg) throw ():GameException (msg)
	{
	};

	inline const char *what (void) const throw ()
	{
	    return "Animation Exception (AnimationException)";
	}
    };

    inline string FileDir (const string & path)
    {
	return path.substr (0, path.rfind ('/') + 1);
    }

};

Animation::Animation (const std::string & xmldoc):
  m_sprite_cache(DEFAULT_WIDTH_CACHE, DEFAULT_HEIGHT_CACHE)
{
    XMLParser & parser = XMLParser::GetRef ();
    int delay = 0;
    string path = FileDir (xmldoc);

    m_active = false;
    m_visible = true;
    m_force_draw = true;
    m_last_update_time = 0;

    parser.LoadDoc (xmldoc);

    for (const Node * n =
	 parser.getFirstChild (parser.getNode ("//animation")); n != NULL;
	 n = parser.NextSibling (n))
      {
	  if (parser.isTextNode (n))
	      continue;
	  if ((delay = parser.getIntAttribute (n, "delay")) < 0)
	      throw AnimationException (string (xmldoc) +
					" contains a negative" 
					" image delay (which is absurd)");
	  path += parser.getText (n);
	  m_frames.push_back (Sprite (path.c_str (), delay));
	  path = FileDir (path);
      }

    m_current_frame = m_frames.begin ();

    parser.FreeDoc ();
    XMLParser::CleanUp ();
}

void
Animation::start (void)
{
    if (m_frames.empty ())
	return;
    m_current_frame = m_frames.begin ();
    m_active = true;
    m_visible = true;
    forceDraw ();
    m_last_update_time = Timer::GetInstance ()->Read ();
}

void
Animation::update (bool redraw_with_cache)
{
    Uint32 current_time;
    const Camera & camera = Camera::GetRef ();
    HMI & hmi = HMI::GetRef();
    Point obj_pos(camera.getObjective ().x, camera.getObjective().y);

    Rectangle dstrect (m_location.x, m_location.y, 
		       getWidth (), getHeight ());
    Rectangle tmp;

    if (camera.wasScrolled () || redraw_with_cache)
	forceDraw ();

    if (!m_force_draw || !m_visible || m_frames.empty ())
	return;

    if (camera.isVisible (dstrect))
      {
	  dstrect.Translate (- obj_pos);

	  tmp.w = m_sprite_cache.m_geometry.w;
	  tmp.h = m_sprite_cache.m_geometry.h;
	  
	  if (!camera.wasScrolled())
	    {
	      // Erase old Sprite on screen
	      hmi.BlitOnScreen(m_sprite_cache.m_cache, 
			       &m_sprite_cache.m_geometry,
			       &tmp);
	      hmi.ToRedraw(m_sprite_cache.m_geometry);
	    }
	  // Save pixels grid in cache
	  m_sprite_cache.m_geometry = dstrect;

	  hmi.ScreenBlitOn(m_sprite_cache.m_cache, NULL, &dstrect);

	  // Blit new sprite frame on screen
	  hmi.BlitOnScreen (m_current_frame->m_surface, &dstrect);
	  hmi.ToRedraw (dstrect);

      }

    if (!m_active)
      {
	  m_force_draw = false;
	  return;
      }

    current_time = Timer::GetInstance ()->Read ();

    if (current_time < (m_last_update_time + m_current_frame->m_delay))
	return;
    m_last_update_time = current_time;

    ((m_current_frame + 1) != m_frames.end ())? m_current_frame++ :
	(m_current_frame = m_frames.begin ());
}
