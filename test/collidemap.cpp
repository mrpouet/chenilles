#include <iostream>

#include <graphics/animation.h>
#include <game/timer.h>

#include <SDL.h>
#include <unistd.h>

namespace
{
    void waitDelay (Uint32 ms)
    {
	Uint32 tmp = Timer::GetInstance ()->Read () + ms;

	while (Timer::GetInstance ()->Read () < tmp)
	    ;;
    }

};


int
main (void)
{
    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER);

    // Screen
    Surface screen (SDL_SetVideoMode (1024, 512, 24,
				      SDL_HWSURFACE | SDL_DOUBLEBUF));
    string ext (".png");
    string Marche ("Marche/");
    // The map stylesheets
    Surface main ("main.png");
    Surface background ("background.png");
    Surface foreground ("foreground.png");

    // The worms Animation
    Animation motion (screen);
    SDL_Event event;
    bool started = true, game = true;
    Uint32 update_time = 0, current_time = 0, start = 0;

    // Time Frame rate
    Uint32 FrameMax_time = 1000 / 50;
    Uint32 rest_time = 0;
    // Frames
    Uint32 count = 0;
    // FPS average
    float avg = 0;

    // Worms foots
    Point foots (0, 280);

    // Worms left-top corner
    Point location (foots.x - 26 / 2, foots.y - 33);

    // Loading Animation Sprites
    for (char i = '1'; i <= '7'; i++)
	motion.newSprite ((Marche + "marche" + i + ext).c_str (), 120);

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
    update_time = Timer::GetInstance ()->Read ();

    while (game)
      {

	  start = Timer::GetInstance ()->Read ();

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

	  if (started)
	    {

		// Worms is in the vacuum
		if (main.GetRGBA (foots).GetA () == 0)
		    foots.y += 5;
		else
		    foots.x += 1;

		// Worms needs to be on the floor
		// (and not into it)
		while (main.GetRGBA (foots).GetA () == 255)
		    foots.y--;
		location.y = foots.y - motion.GetHeight ();
		location.x = foots.x - motion.GetWidth () / 2;
		motion.setLocation (location);
	    }

	  screen.Blit (background);
	  motion.Update ();

	  current_time = Timer::GetInstance ()->Read () - start;

	  if (current_time < FrameMax_time)
	      rest_time = FrameMax_time - current_time;
	  waitDelay (rest_time);
	  screen.Flip ();
	  count++;

	  current_time = Timer::GetInstance()->Read ();
	  if (current_time > (update_time + 5000))
	    {
	      avg = count / ((current_time - update_time) / 1000.f);
		cout << avg << " fps in 5 seconds" << endl;
		update_time = current_time;
		count = 0;
	    }

      }

    SDL_Quit ();
    return 0;
}
