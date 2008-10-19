#include <timer.h>
#include <tools/base.h>

void
Timer::Constructor (void)
{
    m_paused = false;
    m_begin_time = 0;
    m_pause_begin_time = 0;
    m_total_pause_time = 0;
}

void
Timer::Pause (void)
{
    if (m_paused)
	return;
    m_paused = true;
    m_pause_begin_time = SDL_GetTicks ();
}

void
Timer::Continue (void)
{
    if (!m_paused)
	return;
    m_total_pause_time += (SDL_GetTicks () - m_pause_begin_time);
    m_paused = false;
}
