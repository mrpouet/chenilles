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
};

HMI::HMI (void)
{
    m_cursor = NULL;
}

void
HMI::Init (void)
throw (SDLException)
{
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER))
	throw SDLException ();
    SDL_SetEventFilter (Events_Filter);
}

void
HMI::SetVideoMode (int width, int height)
{
    m_screen = Surface (SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE
					  | SDL_HWACCEL | SDL_HWPALETTE
					  | SDL_DOUBLEBUF));
    m_screen.LockFree ();

}

void
HMI::SetCursor (const string & icon)
{
    if (m_cursor)
	delete m_cursor;

    m_cursor = new Surface (icon);

    SDL_WarpMouse (m_screen.GetWidth () / 2, m_screen.GetHeight () / 2);

    m_cursor->DisplayFormatAlpha ();

    SDL_ShowCursor (0);
}

void
HMI::HandleEvent (const SDL_Event & event)
{
    return;
}


void
HMI::RefreshOutput (void)
{
    Camera & camera = Camera::GetRef ();

    RefreshMousePos ();

    camera.m_camera.Flip ();
    m_screen.Blit (camera.m_camera);
    m_screen.Blit (*m_cursor, m_tip);
    m_screen.Flip ();
}

HMI::~HMI (void)
{
    if (m_cursor)
	delete m_cursor;
    SDL_Quit ();
}
