#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <queue>
#include <surface.h>
#include <singleton.h>
#include <hmi.h>

namespace Chenilles
{

    class Camera:public Singleton < Camera >
    {

      public:

	// Refresh Camera position.
        // (only needed for scroll)
	void Refresh (void);

	// Blits a surface to Camera.
	// @param src The Surface which we want to blit
	// @param dstrect The rectangle destination
	// @param srcrect The rectangle source
	inline void UpdateCamera (const Surface & src, rectangle * dstrect =
				  NULL, rectangle * srcrect = NULL)
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

	// Get the last camera scroll (in pixels per scroll)
	//TODO: stupid name ?
	inline int GetSpeed (void) const
	{
	    return m_pixel_per_scroll;
	}

	// Cancel the last Movement
	inline void CancelMove (void)
	{
	    m_camera_box.x -= m_pixel_per_scroll;
	}

	// Redraw a region on camera.
	// region'll really drawed when the camera'll be 
	// draw on the screen.
	// @param dstrect The rectangle to redraw on camera
	inline void ToRedraw (const Rectangle & dstrect)
	{
	    m_region_queue.push (dstrect);
	}

	// Camera was resized ?
	inline bool IsResized (void) const
	{
	    return m_resized;
	}

      private:
	Camera ();

	friend class Singleton < Camera >;

	friend class HMI;

	typedef std::queue < Rectangle > RegionQueue;

	Rectangle m_camera_box;

	Surface m_camera;

	int m_pixel_per_scroll;

	RegionQueue m_region_queue;

	bool m_resized;

    };

};

#endif
