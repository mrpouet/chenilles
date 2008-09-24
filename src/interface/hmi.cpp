#include <cstdlib>
#include <new>
#include <iostream>
#include <tools/base.h>
#include <game/timer.h>
#include <SDL.h>

#include "camera.h"

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

    void ComputeRect (Rectangle& r, int w, int h)
    {
	int rborder = r.x + r.w;
	int bborder = r.y + r.h;

	if (rborder > w)
	    r.w -= (rborder - w);
	if (bborder > h)
	    r.h -= (bborder - h);
	if (r.x < 0)
	    r.x = 0;
    }

};

HMI::HMI (void)
{
    m_cursor = NULL;
    m_current_cursor = CURSOR_MAIN;
    m_external_tip = false;
}

//FIXME: It's more logic (and fast...) to ignore ALL SDL_Events
// except these we interesting in.
// (we don't know all events)
void
HMI::Init (void)
throw (SDLException)
{
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER))
	throw SDLException ();
    SDL_SetEventFilter (Events_Filter);
}

//FIXME: Replace these flags by testing current configuration
// with SDL_GetVideoInfo() (more efficient and "portable")
void
HMI::SetVideoMode (int width, int height)
{
    m_screen = Surface (SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE
					  | SDL_HWACCEL | SDL_HWPALETTE
					  | SDL_DOUBLEBUF));
    m_screen.LockFree ();

}

void
HMI::SetCursor (CursorType type, const string& icon)
{

    if (m_cursor)
	delete m_cursor;

    m_cursor = new Surface (icon);

    m_current_cursor = type;

    m_cursor->DisplayFormatAlpha ();

    m_tip.x = m_screen.GetWidth() / 2;
    m_tip.y = m_screen.GetHeight() / 2;

    SDL_ShowCursor (SDL_DISABLE);

}

void
HMI::HandleEvent (const SDL_Event& event)
{
    return;
}

//FIXME: Please refresh the cursor rects ONLY if 
// it moved (more efficient).
void
HMI::RefreshOutput (void)
{
    Camera & camera = Camera::GetRef ();
    Camera::redraw_queue & queue = camera.m_redraw_queue;
    rectangle rect = { 0, 0, 0, 0 };
    Rectangle r (0, 0, m_cursor->GetWidth (), m_cursor->GetHeight ());

    int size = !queue.empty ()? static_cast<int>(queue.size () + 2) : 2;
    rectangle *rects = NULL;
    int i;

    //FIXME: not the most efficient
    rects = (rectangle *) malloc (size * sizeof (rectangle));

    if (!rects)
	throw bad_alloc ();

    // "Catch" and exec all redraw events
    for (i = 0; !queue.empty (); i++)
      {

	  rects[i] = queue.front ().GetSDLRect ();
	  camera.m_camera.UpdateRect (queue.front ());
	  m_screen.Blit (camera.m_camera, &rects[i], &rects[i]);
	  queue.pop ();
      }

    // Delete old cursor
    r.x = m_tip.x;
    r.y = m_tip.y;
    ComputeRect (r, m_screen.GetWidth (), m_screen.GetHeight ());
    rect = r.GetSDLRect ();
    rects[i++] = rect;

    m_screen.Blit (camera.m_camera, &rect, &rect);

    // Compute the new cursor rectangle position
    RefreshMousePos();
    r.x = m_tip.x;
    r.y = m_tip.y;
    ComputeRect (r, m_screen.GetWidth (), m_screen.GetHeight ());

    // Draw cursor
    m_screen.Blit (*m_cursor, m_tip);
    rects[i++] = r.GetSDLRect ();

    m_screen.UpdateRects (size, rects);

    free (rects);
}

HMI::~HMI (void)
{
    if (m_cursor)
	delete m_cursor;
    SDL_Quit ();
}
