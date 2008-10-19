#include <camera.h>
#include <tools/base.h>

#define MIN_SCROLL_MARGIN 50

Camera::Camera ()
{
    m_camera_box.x = m_camera_box.y = 0;
    m_camera_box.w = HMI::GetInstance ()->m_screen.GetWidth ();
    m_camera_box.h = HMI::GetInstance ()->m_screen.GetHeight ();

    m_camera = Surface::CreateRGB (m_camera_box);
    m_camera.DisplayFormatAlpha ();
    m_pixel_per_scroll = 0;

    m_resized = false;

}

void
Camera::Refresh (void)
{
    const HMI& hmi = HMI::GetRef();
    Point tip = hmi.m_tip;
    m_resized = false;

    // Screen was resized
    if (m_camera_box.w != hmi.m_screen.GetWidth())
      {

	m_camera_box.w = hmi.m_screen.GetWidth();
	m_camera_box.h = hmi.m_screen.GetHeight();

	m_camera.Resize(m_camera_box.w, m_camera_box.h);
	ToRedraw(Rectangle(0, 0, m_camera_box.w, m_camera_box.h));
	m_resized = true;
      }

    if (hmi.m_current_cursor == HMI::NO_CURSOR)
	return;

    if (tip.x >= (m_camera_box.w - MIN_SCROLL_MARGIN))
	m_pixel_per_scroll = 5;
    else if (tip.x <= MIN_SCROLL_MARGIN)
	m_pixel_per_scroll = -5;
    else
	m_pixel_per_scroll = 0;
    m_camera_box.x += m_pixel_per_scroll;

}
