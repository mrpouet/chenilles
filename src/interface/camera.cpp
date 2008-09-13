#include "camera.h"

#define MIN_SCROLL_MARGIN 50

Camera::Camera ()
{
    m_camera_box.x = m_camera_box.y = 0;
    m_camera_box.w = HMI::GetInstance ()->m_screen.GetWidth ();
    m_camera_box.h = HMI::GetInstance ()->m_screen.GetHeight ();

    m_camera = Surface (m_camera_box);
    m_camera.DisplayFormatAlpha ();
    m_pixel_per_scroll = 0;

}

void
Camera::Refresh (void)
{
    HMI::GetInstance ()->RefreshMousePos ();
    Point tip = HMI::GetInstance ()->m_tip;

    if (tip.x >= (m_camera_box.w - MIN_SCROLL_MARGIN))
	m_pixel_per_scroll = 5;
    else if (tip.x <= MIN_SCROLL_MARGIN)
	m_pixel_per_scroll = -5;
    else
	m_pixel_per_scroll = 0;
    m_camera_box.x += m_pixel_per_scroll;

}
