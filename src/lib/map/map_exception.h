#ifndef __MAP_EXCEPTION_H__
#define __MAP_EXCEPTION_H__

#include <tools/game_exception.h>

class MapException:public GameException
{
  public:

    explicit MapException (const string & msg) throw ():GameException (msg)
    {}

     ~MapException () throw ()
    {}

    const char *what (void) const throw ()
    {
	return "Map exception (MapException)";
    }

};

#endif
