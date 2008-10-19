#include <cstdlib>
#include <iostream>
#include <new>
#include <SDL.h>
#include <timer.h>
#include <camera.h>
#include <game_exception.h>
#include <tools/base.h>

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

};

HMI::HMI (void)
{
    m_current_cursor = NO_CURSOR;
    m_external_tip = false;
    m_lock = false;
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
      throw GameException (SDL_GetError());
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

    m_cursors[type] = Surface::CreateFromFile(icon);

    // When HMI::RefreshOutput is called,
    // current cursor is directly drawed to the
    // screen itself.
    m_cursors[type].DisplayFormatAlpha ();

    m_tip.x = m_screen.GetWidth () / 2;
    m_tip.y = m_screen.GetHeight () / 2;

    SDL_ShowCursor (SDL_DISABLE);

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
    Camera & camera = Camera::GetRef ();
    Camera::RegionQueue & queue = camera.m_region_queue;
    rectangle rect = { 0, 0, 0, 0 };
    Rectangle r;

    int size = static_cast < int >(queue.size ());
    rectangle *rects = NULL;
    int i;

    if (m_lock)
      return;

    size += (m_current_cursor != NO_CURSOR) ? 2 : 0;

    r.w = (m_current_cursor != NO_CURSOR) ?
	m_cursors[m_current_cursor].GetWidth () : 0;

    r.h = (m_current_cursor != NO_CURSOR) ?
	m_cursors[m_current_cursor].GetHeight () : 0;

    //FIXME: not the most efficient

    if (size)
      {
	  rects = (rectangle *) malloc (size * sizeof (rectangle));

	  if (!rects)
	      throw bad_alloc ();
      }

    // "Catch" and exec all redraw events
    for (i = 0; !queue.empty (); i++)
      {

	  rects[i] = queue.front ().GetSDLRect ();
	  camera.m_camera.UpdateRect (queue.front ());
	  m_screen.Blit (camera.m_camera, &rects[i], &rects[i]);
	  queue.pop ();
      }

    if (m_current_cursor != NO_CURSOR)
      {
	  // Delete old cursor
	  r.x = m_tip.x;
	  r.y = m_tip.y;
	  
	  rect = r.GetSDLRect ();
	  m_screen.Blit (camera.m_camera, &rect, &rect);

	  ComputeRect (rect, m_screen.GetWidth (), m_screen.GetHeight ());
	  rects[i++] = rect;

	 

	  // Compute the new cursor rectangle position
	  RefreshMousePos();
	  r.x = m_tip.x;
	  r.y = m_tip.y;
	  rect = r.GetSDLRect();
	  ComputeRect (rect, m_screen.GetWidth (), m_screen.GetHeight ());

	  // Draw cursor
	  m_screen.Blit (m_cursors[m_current_cursor], m_tip);
	  
	  rects[i++] = rect;

      }

    m_screen.UpdateRects(size, rects);

    free (rects);
}

  
