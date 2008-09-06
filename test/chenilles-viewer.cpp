#include <iostream>
#include <map/map.h>
#include <map/map_exception.h>
#include <tools/game_exception.h>

#include <SDL.h>


int
main (void)
{
    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER);

    Surface screen (SDL_SetVideoMode (1024, 512, 24,
				      SDL_HWSURFACE | SDL_DOUBLEBUF));
    screen.LockFree ();
    bool game = true;
    SDL_Event event;

    try
    {
	Map map (string (DATAROOTDIR) + "/maps/testMap1/config.xml");

	while (game)
	  {

	      while (SDL_PollEvent (&event))
		{
		    if (event.type == SDL_QUIT)
		      {
			  game = false;
			  break;
		      }
		}

	      map.Draw (screen);
	      screen.Flip ();
	  }
    }
    catch (const GameException & e)
    {
	cerr << "Caught exception:" << endl
	     << "what()\t: " << e.what () << endl
	     << "error()\t: " << e.error () << endl;
    }
    catch (const std::exception & e)
    {
	cerr << "Caught exception:" << endl
	     << "what()\t: " << e.what () << endl;
    }

    SDL_Quit ();
}
