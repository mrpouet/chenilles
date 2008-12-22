#include <camera.h>
#include <hmi.h>
#include <tools/base.h>

// Speed (in pixels) when we scroll (with mouse) 
const Uint16 Camera::SCROLL_MOUSE = 20;

// Sensitivity margin from the screen border
const Uint16 Camera::SENSIT_MARGIN = 40;

Camera::Camera ():
  m_objective(0, 0, 
	      HMI::GetRef().m_screen.GetWidth(),
	      HMI::GetRef().m_screen.GetHeight())
{

}

void
Camera::Refresh (void)
{
    const HMI &hmi = HMI::GetRef();
    const Point &tip = hmi.m_tip;

    m_last_scroll.x = m_last_scroll.y = 0;

    // When cursor is disable scroll is too.
    // (it's absurd to scroll with no cursor).
    if (hmi.m_current_cursor == HMI::NO_CURSOR)
      return;

    // Horizontal scroll
    if (tip.x <= SENSIT_MARGIN)
      m_last_scroll.x = -SCROLL_MOUSE;
    else if (tip.x >= (m_objective.w - SENSIT_MARGIN))
      m_last_scroll.x = SCROLL_MOUSE;

    // Vertical scroll
    if (tip.y <= SENSIT_MARGIN)
      m_last_scroll.y = -SCROLL_MOUSE;
    else if (tip.y >= (m_objective.h - SENSIT_MARGIN))
      m_last_scroll.y = SCROLL_MOUSE;
    m_objective.Translate(m_last_scroll);

}
