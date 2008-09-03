#include <graphics/animation.h>
#include <game/timer.h>

#include <SDL.h>

int
main (void)
{
    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER);

    // Screen
    Surface screen (SDL_SetVideoMode (1024, 512, 24,
				      SDL_HWSURFACE | SDL_DOUBLEBUF));
    string ext (".png");

    // The map stylesheets
    Surface main ("main.png");
    Surface background ("background.png");
    Surface foreground ("foreground.png");

    // The worms Animation
    Animation motion (screen);
    SDL_Event event;
    bool started = true, game = true;

    // Frame rate
    int rate = 50;

    // Worms foots
    Point foots (0, 280);

    // Worms left-top corner
    Point location (foots.x - 26 / 2, foots.y - 33);

    // Loading Animation Sprites
    for (char i = '1'; i <= '7'; i++)
      motion.newSprite ((string("vers") + i + ext).c_str (), 130);

    // We need to Lockfree the screen Surface, because SDL_Quit
    // clean up all initialized subsystem, and will free
    // the screen surface during VIDEO clean up.
    // (otherwises double free, or segfault).
    screen.LockFree ();

    background.DisplayFormat ();
    background.Blit (main);
    background.Blit (foreground);
    background.Flip ();

    // Timer design pattern, initializing
    Timer::GetInstance ()->Reset ();

    motion.Start ();

    // When we blit a surface at a point,
    // this point is ALWAYS this left-top corner.
    motion.setLocation (location);

    while (game)
      {

	  //FIXME: use Timer in the future
	  // (SDL_Delay ask app to sleep for a while).
	  SDL_Delay (1000 / rate);

	  if (started)
	    {

		// Worms is in the vacuum
		if (main.GetRGBA (foots).GetA () == 0)
		    foots.y += 5;
		else
		    foots.x++;

		// Worms needs to be on the floor
		// (and not into it)
		while (main.GetRGBA (foots).GetA () == 255)
		    foots.y--;
		location.y = foots.y - motion.GetHeight ();
		location.x = foots.x - motion.GetWidth () / 2;
		motion.setLocation (location);


		screen.Blit (background);
		motion.Update ();

		screen.Flip ();
	    }

	  while (SDL_PollEvent (&event))
	    {
		if (event.type == SDL_QUIT)
		  {
		      game = false;
		      break;
		  }

		// ONLY needed to debug the worms path
		// on the map.
		else if ((event.type == SDL_KEYDOWN) &&
			 (event.key.keysym.sym == SDLK_p))
		  {
		      started ? motion.Stop () : motion.Start ();
		      started = !started;
		  }
	    }

      }

    SDL_Quit ();
    return 0;
}
