#ifndef __SDL_EXCEPTION_H__
#define __SDL_EXCEPTION_H__

#include <SDL_error.h>
#include <tools/game_exception.h>

class SDLException:public GameException
{
  public:

    SDLException () throw ():GameException (string ("none"))
    {}

     ~SDLException () throw ()
    {}

    const char *what (void) const throw ()
    {
	return "SDL exception (SDLException)";
    }

    const char *error (void) const throw ()
    {
	return SDL_GetError ();
    }
};

#endif

