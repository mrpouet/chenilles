#ifndef __HMI_H__
#define __HMI_H__

#include <map>
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

	// Blank the screen.
	inline void Clear (void)
	{
	    m_screen.FillRect (Color (0, 0, 0));
	    m_screen.Flip ();
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

	void HandleEvent (const SDL_Event & event);

	// Refresh ALL Output devices
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

	CursorType m_current_cursor;

	Point m_tip;

	Point m_tmp_tip;

	bool m_external_tip;

	typedef std::map < CursorType, Surface > CursorContainer;

	CursorContainer m_cursors;

	bool m_lock;

    };

};

#endif
