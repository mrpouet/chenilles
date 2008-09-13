#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <graphics/surface.h>
#include <tools/singleton.h>

#include "hmi.h"

class Camera:public Singleton < Camera >
{

  public:

    // Refresh Camera position, depending of cursor location.
    void Refresh (void);

    // Blits 'src' Surface object to Camera
    inline void UpdateCamera (const Surface & src, rectangle * dstrect = NULL,
			      rectangle * srcrect = NULL)
    {
	m_camera.Blit (src, dstrect, srcrect);
    }

    // Get Rectangle which represents Camera current position.
    const Rectangle & GetCameraBox (void) const
    {
	return m_camera_box;
    }

    // Access to Camera Surface in Read Only.
    const Surface & GetCamera (void) const
    {
	return m_camera;
    }

    // Get the last camera scroll speed (in pixels per scroll)
    inline int GetSpeed (void) const
    {
	return m_pixel_per_scroll;
    }

    // Cancel the last Movement
    inline void CancelMove (void)
    {
	m_camera_box.x -= m_pixel_per_scroll;
    }


  private:
    Camera ();
    friend class Singleton < Camera >;
    friend class HMI;
    Rectangle m_camera_box;
    Surface m_camera;
    int m_pixel_per_scroll;

};

#endif
