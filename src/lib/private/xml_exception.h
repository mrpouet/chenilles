#ifndef __XML_EXCEPTION_H__
#define __XML_EXCEPTION_H__

#if !defined __XML_PARSER_H__ && !defined __XML_WRITER_H__
#error "This is a private header file, don't use it !"
#else
#include <game_exception.h>

namespace Chenilles
{

    class XMLException:public GameException
    {
      public:

	explicit XMLException (const std::string & msg) throw ():
        GameException (msg)
	{}

	~XMLException () throw () {};

	const char *what (void) const throw ()
	{
	    return "XML parser exception (XMLException)";
	}

    };

};

#endif /* !defined  __XML_PARSER_H__ && !defined __XML_WRITER_H__ */
#endif /* __XML_EXCEPTION_H__ */

