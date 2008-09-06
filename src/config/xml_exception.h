#ifndef __XML_EXCEPTION_H__
#define __XML_EXCEPTION_H__

#include <tools/game_exception.h>
#include <tools/base.h>

class XMLException:public GameException
{
  public:

    explicit XMLException (const string & msg) throw ():GameException (msg)
    {}

    ~XMLException () throw ()
    {}

    const char *what (void) const throw ()
    {
	return "XML parser exception (XMLException)";
    }

};

#endif
