#ifndef __GAME_EXCEPTION_H__
#define __GAME_EXCEPTION_H__

#include <exception>
#include <string>

namespace Chenilles
{

    class GameException:public std::exception
    {
      public:

      GameException (const std::string & msg) throw ():m_msg (msg)
	{}

	virtual ~ GameException () throw ()
	{}

	virtual const char *what (void) const throw ()
	{
	    return "Runtine game exception (GameException)";
	}

	virtual const char *error (void) const throw ()
	{
	    return m_msg.c_str ();
	}

      private:
	std::string m_msg;

    };

};

#endif
