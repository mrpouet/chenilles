#ifndef __HMI_H__
#define __HMI_H__

#include <graphics/surface.h>
#include <graphics/sdl_exception.h>
#include <tools/singleton.h>

#include <SDL_events.h>

// Human Machine Interface
class HMI:public Singleton<HMI>
{
  public:

    typedef enum
    {
	CURSOR_MAIN,
	CURSOR_TARGET
    } CursorType;

    // Init HMI module and SDL subsystems
    // which will be use.
    static void Init (void) throw (SDLException);

    // Init the screen Surface
    // Depending: Init()
    void SetVideoMode (int width, int height);

    // Set/Change the screen cursor
    // Last cursor is automatic replaced by the new pointer
    // Depending: SetVideoMode (it's absurd without it)
    void SetCursor (CursorType type, const string & icon);

    void HandleEvent (const SDL_Event& event);

    // Refresh ALL Output devices
    void RefreshOutput (void);

    // We need to use external tip position ?
    // Note: Use if you want to share the HMI cursor
    // with another thread for example in ArtEditor
    // (due to thread-safe)
    inline void set_external_tip(void)
    {
      m_external_tip = true;
    }

    inline void SetPointer(const Point& tip)
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

    friend class Singleton<HMI>;
    friend class Camera;
    // The screen Surface
    Surface m_screen;
    // Cursor icon
    Surface *m_cursor;
    // The Tip of the cursor
    Point m_tip;
    Point m_tmp_tip;
    // Would you like to specified pointer pos yourself ?
    // (don't ask X11).
    bool m_external_tip;
    // The current Cursor Type
    CursorType m_current_cursor;
};

#endif
