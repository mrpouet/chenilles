#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <point.h>
#include <rectangle.h>
#include <singleton.h>

namespace Chenilles
{

    class Camera:public Singleton < Camera >
    {

      public:

        const static Uint16 SCROLL_MOUSE;

	const static Uint16 SENSIT_MARGIN;

	// Refresh Camera position.
        // (only needed for scroll)
	void Refresh (void);

	// Access to Camera Objective 
	// (which represent the physical location on Camera)
	// @return A Rectangle which represents the objective
	inline const Rectangle & getObjective (void) const
	{
	    return m_objective;
	}

	inline void setObjectiveCoords(Sint16 x, Sint16 y)
	{
	  m_objective.x = x;
	  m_objective.y = y;
	}

	inline bool isVisible (const Rectangle& bounding_box) const
	{
	  return m_objective.Intersect(bounding_box);
	}

	inline bool wasScrolled(void) const
	{
	  return m_last_scroll.x || m_last_scroll.y;
	}

	// Cancel the last scroll movement
	inline void unScroll (void)
	{
	  m_objective.Translate (-m_last_scroll);
	  m_last_scroll.x = m_last_scroll.y = 0;
	}

      private:
	Camera ();

	friend class Singleton < Camera >;

	Rectangle m_objective;

	Point m_last_scroll;

    };

};

#endif
