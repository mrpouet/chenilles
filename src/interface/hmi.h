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

    // Init HMI module and SDL subsystems
    // which will be use.
    static void Init (void) throw (SDLException);

    // Init the screen Surface
    // Depending: Init()
    void SetVideoMode (int width, int height);

    // Set/Change the screen cursor
    // Last cursor is automatic replaced by the new pointer
    // Depending: SetVideoMode (it's absurd without it)
    void SetCursor (const string & icon);

    void HandleEvent (const SDL_Event & event);

    // Refresh ALL Output devices
    void RefreshOutput (void);

  private:

      HMI (void);
     ~HMI (void);

    inline void RefreshMousePos (void)
    {
	SDL_GetMouseState (&m_tip.x, &m_tip.y);
    }

    friend class Singleton<HMI>;
    friend class Camera;
    // The screen Surface
    Surface m_screen;
    // Cursor icon
    Surface *m_cursor;
    // Tip of the cursor
    Point m_tip;

};

#endif
