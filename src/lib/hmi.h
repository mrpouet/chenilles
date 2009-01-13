#ifndef __HMI_H__
#define __HMI_H__

#include <map>
#include <queue>

#include <SDL_events.h>
#include <surface.h>
#include <singleton.h>

namespace Chenilles
{

    class HMI:public Singleton < HMI >
    {
      public:

	typedef enum
	{
	    NO_CURSOR = -1,
	    CURSOR_TIP,
	    CURSOR_TARGET
	} CursorType;

	typedef enum
	{
	    MOUSE_NONE,
	    MOUSE_LCLICK,
	    MOUSE_MCLICK,
	    MOUSE_RCLICK
	} Mouse_t;

	// Init HMI module.
	// @throw exception
	static void Init (void);

	// Init the screen Surface
	// Depends: Init()
	// @param width The screen width
	// @param height The screen height
	// @param resizable Screen must to be resizable ?
	// (false as default)
	void SetVideoMode (int width, int height, bool resizable = false);

	inline void BlitOnScreen (const Surface & src, rectangle * dstrect =
				  NULL, rectangle * srcrect = NULL)
	{
	    m_screen.Blit (src, dstrect, srcrect);
	}

	inline void BlitOnScreen (const Surface & src, Rectangle * dstrect =
				  NULL, Rectangle * srcrect = NULL)
	{
	    BlitOnScreen (src, reinterpret_cast < rectangle * >(dstrect),
			  reinterpret_cast < rectangle * >(srcrect));
	}

	inline void ScreenBlitOn (Surface & dest, Rectangle * dstrect = NULL,
				  Rectangle * srcrect = NULL) const
	{
	    dest.Blit (m_screen, reinterpret_cast < rectangle * >(dstrect),
		       reinterpret_cast < rectangle * >(srcrect));
	}

	inline void ToRedraw (const Rectangle & region)
	{
	    m_region_queue.push (region);
	}

	// Blank the screen.
	inline void Clear (void)
	{
	    m_screen.FillRect (Color (0, 0, 0));
	}

	// Lock the screen from any refresh
	inline void LockRefresh (void)
	{
	    m_lock = true;
	}

	// Unlock the screen from any refresh
	// (all refresh are now autorized)
	inline void UnlockRefresh (void)
	{
	    m_lock = false;
	}


	// Set the cursor pixmap
	// @param type The cursor type.
	// @param icon pixmap which 'll define the cursor type
	// Note: a CursorType is definable once time.
	// Depending: SetVideoMode (it's absurd without it)
	void SetCursor (CursorType type, const std::string & icon);

	// Please be sure to called SetCursor on the following type
	// to call this method.
	// @param type The cursor type to switch to.
	inline void SwitchToCursor (CursorType type)
	{
	    m_current_cursor = type;
	}

	// Handle an event which concern the mouse.
	// @param event a catch SDL_Event event object
	// @return The clicked type mouse button
	// TODO: Must we modify it to include keyboard ?
	Mouse_t HandleEvents (const SDL_Event & event, Point& tip);

	// Refresh all output devices
	void RefreshOutput (void);

	// We need to use external tip position ?
	// Note: Use if you want to share the HMI cursor
	// with another thread for example Gtk
	// (due to thread-safe)
	inline void set_external_tip (void)
	{
	    m_external_tip = true;
	}

	inline void SetPointer (const Point & tip)
	{
	    m_tmp_tip = tip;
	}

      private:

	typedef std::map < CursorType, Surface > CursorContainer;
	typedef std::queue < Rectangle > RegionQueue;

	HMI (void);

	~HMI (void);

	inline void RefreshMousePos (void)
	{
	    if (m_external_tip)
		m_tip = m_tmp_tip;
	    else
		SDL_GetMouseState (&m_tip.x, &m_tip.y);
	}

	friend class Singleton < HMI >;

	friend class Camera;

	Surface m_screen;

	Surface m_cursor_cache;

	CursorType m_current_cursor;

	Point m_tip;

	Point m_tmp_tip;

	bool m_external_tip;

	CursorContainer m_cursors;

	RegionQueue m_region_queue;

	bool m_lock;

    };

};

#endif
