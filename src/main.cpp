#include <iostream>
#include <game_exception.h>
#include <units/unit.h>
#include <game/game.h>
#include <hmi.h>
#include <camera.h>
#include <timer.h>

#include <SDL/SDL.h>


int
main (void)
{
    try
    {
	HMI::GetRef ().Init ();
	HMI::GetRef ().SetVideoMode (1024, 640);
	
	
	HMI::GetRef().SetCursor(HMI::CURSOR_TIP, "../data/ui/cursor_main.png");
	HMI::GetRef().SwitchToCursor(HMI::CURSOR_TIP);
	Unit unit(Point(100, 0),
		  "../data/teams/lombric/constructor/config.xml");
	world = new Map ("../data/maps/SceneOfCrime_medium/config.xml");
	SDL_Event event;
	bool b = true;
	
	Timer::GetRef().Reset();
	
	//unit.startWalk(Point(0, 0));

	while (b)
	  {
	      while (SDL_PollEvent (&event))
		{
		    if (event.type == SDL_QUIT)
		      {
			  b = false;
			  break;
		      }
		}
      
	      Camera::GetRef ().Refresh ();
	      world->draw();
	      unit.refresh();
	      SDL_Delay(10);

	      HMI::GetRef ().RefreshOutput ();
	  }
    }
    catch (const GameException & e)
    {
	cerr << "Caught exception:" << endl
	    << "what()  : " << e.what () << endl
	    << "error() : " << e.error () << endl;
    }
    catch (const std::exception & e)
    {
	cerr << "Caught exception:" << endl
	    << "what() : " << e.what () << endl;
    }

    free(world);
    Camera::CleanUp();
    Timer::CleanUp();
    HMI::CleanUp();
    return 0;
}
