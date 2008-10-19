#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cstddef>

namespace Chenilles
{
    //FIXME: Add thread-safe later
    template < class T > class Singleton
    {

      public:

	static T *GetInstance (void)
	{
	    if (!singleton)
		singleton = new T ();
	    return singleton;
	}

	static T & GetRef (void)
	{
	    return *GetInstance ();
	}

	static void CleanUp (void)
	{
	    if (singleton)
	      delete singleton;
	}

      protected:
	static T *singleton;

	~Singleton ()
	{
	    singleton = NULL;
	}


    };
    
    template < class T > T * Singleton < T >::singleton = NULL;
};



#endif /*      __SINGLETON_H__ */
