#include <map/map.h>
#include <map/map_exception.h>
#include <interface/hmi.h>
#include <interface/camera.h>
#include <game/timer.h>
#include <tools/game_exception.h>

#include <gtkmm/main.h>
#include <cstdio>
#include "editor_ui.h"

namespace
{
    Map *world = NULL;

    void on_init (void)
    {
	Timer::GetRef ().Reset ();
	HMI::GetRef ().set_external_tip ();
	HMI::GetRef ().SetCursor (HMI::CURSOR_MAIN,
				  "../../data/menu/cursor_main.png");
	world = new Map ("../../data/maps/testMap1/config.xml");
    }

    bool RefreshWorld (void)
    {
	if (!world)
	    return true;
	Camera::GetRef ().Refresh ();
	world->draw ();
	HMI::GetRef ().RefreshOutput ();

	return true;
    }

};


int
main (int argc, char *argv[])
{

    Main kit (argc, argv);
    EditorUI gui (800, 600);

    gui.signal_init ().connect (sigc::ptr_fun (&on_init));

    Glib::signal_timeout ().connect (sigc::ptr_fun (RefreshWorld), 20);

    Main::run (gui);

    return 0;
}
