#include <game/timer.h>

#include "animation.h"

Animation::Animation (const Surface & dest):
m_dest (dest)
{
    m_active = false;
    m_last_update_time = 0;
}

void
Animation::Start (void)
{
    if (m_frames.empty ())
	return;
    m_it = m_frames.begin ();
    m_active = true;
    m_last_update_time = Timer::GetInstance ()->Read ();
}

void
Animation::Update (void)
{
    Uint32 current_time;

    m_dest.Blit (m_it->m_surface, m_location);

    if (!m_active)
	return;

    current_time = Timer::GetInstance ()->Read ();

    if (current_time < (m_last_update_time + m_it->m_delay))
	return;
    m_last_update_time = current_time;

    ((m_it + 1) != m_frames.end ())? m_it++ : (m_it = m_frames.begin ());
}
