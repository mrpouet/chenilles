#include <iostream>
#include <vector>
#include <SDL.h>
#include <timer.h>
#include <hmi.h>
#include <camera.h>
#include <game_exception.h>
#include <tools/base.h>

#define DEFAULT_WIDTH_CACHE 21
#define DEFAULT_HEIGHT_CACHE 21

namespace
{
    inline int Events_Filter (const SDL_Event * event)
    {
	return ((event->type == SDL_MOUSEMOTION) ||
		(event->type == SDL_MOUSEBUTTONUP) ||
		(event->type == SDL_MOUSEBUTTONDOWN)) ? 0 : 1;
    }

    // Necessary for Surface::UpdateRects which doesn't check
    // out of ranges of screen (apparently due to SDL_UpdateRects).
    // otherwises X error, BadValue is throwed,
    // (integer parameter out of range for operation).
    // Note: see http://www.libsdl.org/cgi/docwiki.cgi/SDL_UpdateRects.

    //TODO: moved this subroutine to Surface ?
    // (needed to use Surface::UpdateRects in general)
    void ComputeRect (rectangle & r, int w, int h)
    {
	int rborder = r.x + r.w;
	int bborder = r.y + r.h;
	int tmp = 0;

	r.x = (r.x < 0) ? 0 : r.x;
	r.y = (r.y < 0) ? 0 : r.y;

	if (rborder > w)
	  {
	      // Fix overflow due to Rectangle width 
	      // unsigned integer on 16 bits.
	      // thanks to vladisback bug report ;)
	      tmp = r.w - rborder - w;
	      r.w = (tmp < 0) ? 0 : tmp;
	  }
	if (bborder > h)
	  {
	      tmp = r.h - bborder - h;
	      r.h = (tmp < 0) ? 0 : tmp;
	  }

    }

    class IOException:public GameException
    {
      public:
	IOException (const string & msg) throw ():GameException (msg)
	{
	};

	inline const char *what (void) const throw ()
	{
	    return "Human Machine Interface I/O Exception (IOException)";
	}
    };

};

HMI::HMI (void)
{
    m_current_cursor = NO_CURSOR;
    m_external_tip = false;
    m_lock = false;
    m_cursor_cache = Surface::CreateRGB (Rectangle (0, 0,
						    DEFAULT_WIDTH_CACHE,
						    DEFAULT_HEIGHT_CACHE));
}

HMI::~HMI (void)
{
    SDL_Quit ();
}

//FIXME: It's more logic (and fast...) to ignore ALL SDL_Events
// except these we interesting in.
// (we don't know all events)
void
HMI::Init (void)
{
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER))
	throw IOException (SDL_GetError ());
    SDL_SetEventFilter (Events_Filter);
}

//FIXME: Replace these flags by testing current configuration
// with SDL_GetVideoInfo() (more efficient and "portable")
void
HMI::SetVideoMode (int width, int height, bool resizable)
{
    Uint32 flags = SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE
	| SDL_DOUBLEBUF;

    if (resizable)
	flags |= SDL_RESIZABLE;
    m_screen = Surface (SDL_SetVideoMode (width, height, 32, flags));
    m_cursor_cache.DisplayFormatAlpha ();
}

void
HMI::SetCursor (CursorType type, const string & icon)
{

    CursorContainer::iterator it;

    // NO_CURSOR is undefinable.
    // Typically, it's not Alpha Surface,
    // but it's nothing (so, not drawable).
    if (type == NO_CURSOR)
	return;

    it = m_cursors.find (type);

    // CursorType is already defined
    if (it != m_cursors.end ())
	return;

    m_cursors[type] = Surface::CreateFromFile (icon);

    // When HMI::RefreshOutput is called,
    // current cursor is directly drawed to the
    // screen itself.
    m_cursors[type].DisplayFormatAlpha ();

    m_tip.x = m_screen.GetWidth () / 2;
    m_tip.y = m_screen.GetHeight () / 2;

}

void
HMI::HandleEvent (const SDL_Event & event)
{
    // Informe and fix temporaly warning.
    // (unsed event parameter)
    cerr << __func__ << "(event=" << event.type
	<< "): Not implemented yet !" << endl;
    return;
}

void
HMI::RefreshOutput (void)
{
    static Rectangle r;
    rectangle rect = { 0, 0, 0, 0 };
    vector<Rectangle> rects;

    // Switched Cursor didn't defined
    if ((m_current_cursor != NO_CURSOR)
	&& m_cursors.find (m_current_cursor) == m_cursors.end ())
	throw IOException ("Cursor specified type never defined"
			   " using HMI::SetCursor");

    if (m_current_cursor != NO_CURSOR)
      {
	r.w = m_cursors[m_current_cursor].GetWidth();
	r.h = m_cursors[m_current_cursor].GetHeight();
      }

    // Extract all redraw region from queue.
    for (; !m_region_queue.empty (); m_region_queue.pop ())
      rects.push_back(m_region_queue.front ());

    // Delete old cursor using cursor cache surface
    if (!Camera::GetRef ().wasScrolled ())
      {
	  r.x = m_tip.x;
	  r.y = m_tip.y;

	  if (!SDL_ShowCursor(SDL_DISABLE))
	    {
	      rect = r.GetSDLRect ();
	      BlitOnScreen (m_cursor_cache, &rect);
	      rects.push_back(r);
	    }
      }

    if (m_current_cursor != NO_CURSOR)
      {
	  // Compute the new cursor rectangle position
	  RefreshMousePos ();
	  r.x = m_tip.x;
	  r.y = m_tip.y;
	  rect = r.GetSDLRect ();

	  rects.push_back(r);

	  // "Compute" new cache
	  m_cursor_cache.Blit (m_screen, NULL, &rect);
	  m_cursor_cache.UpdateRect (&rect);

	  // Draw new cursor
	  m_screen.Blit (m_cursors[m_current_cursor], m_tip);
      }
    
    // Apply all redraw region
    for (vector<Rectangle>::const_iterator it = rects.begin(); 
	 it != rects.end(); it++)
      m_screen.UpdateRect (*it);
}
