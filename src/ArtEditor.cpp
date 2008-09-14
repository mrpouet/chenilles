#include <iostream>

#include <map/map.h>
#include <map/map_exception.h>
#include <interface/hmi.h>
#include <interface/camera.h>
#include <game/timer.h>
#include <tools/game_exception.h>

#include <SDL_events.h>

int
main (void)
{

    HMI::Init ();
    bool game = true;
    SDL_Event event;
    string data (DATAROOTDIR);
    Timer::GetInstance ()->Reset ();
    Uint32 update_time = 0, rate = 50, framecount = 0;
    Uint32 start = 0;
    float avg = 0;
    Uint32 rate_ticks = 1000 / rate;
    Sint32 delay = 0;

    try
    {
	HMI::GetInstance ()->SetVideoMode (800, 512);

	Map map (data + "/maps/testMap1/config.xml");

	HMI::GetInstance ()->SetCursor (data + "/menu/cursor_main.png");

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
		}

	      Camera::GetInstance ()->Refresh ();
	      map.draw ();

	      if (Timer::GetInstance ()->Read () > (update_time + 1000))
		{
		    avg = framecount
			/ ((Timer::GetInstance ()->Read () - update_time) /
			   1000.f);
		    cout << avg << " frames in 1 second" << endl;
		    update_time = Timer::GetInstance ()->Read ();
		    framecount = 0;
		}

	      delay = rate_ticks - (Timer::GetInstance ()->Read () - start)
		  - TIMER_RESOLUTION;
	      if (delay > 0)
		  SDL_Delay (delay);

	      HMI::GetInstance ()->RefreshOutput ();
	      framecount++;


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

    HMI::CleanUp ();
    Camera::CleanUp ();
    Timer::CleanUp ();

    return 0;
}
