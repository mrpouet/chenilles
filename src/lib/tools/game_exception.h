#ifndef __GAME_EXCEPTION_H__
#define __GAME_EXCEPTION_H__

#include <exception>
#include <string>

#include "base.h"

class GameException:public exception
{
  public:

    explicit GameException (const string & msg) throw ():m_msg (msg)
    {}

    virtual ~GameException () throw ()
    {}

    virtual const char *what (void) const throw ()
    {
	return "Game runtine exception (GameException)";
    }

    virtual const char *error (void) const throw ()
    {
	return m_msg.c_str ();
    }

  private:
    string m_msg;

};

#endif
