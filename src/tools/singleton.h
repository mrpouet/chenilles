#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<class T>
class Singleton
{
 protected:
  static T* singleton;

 protected:
  ~Singleton()
    {
      singleton = NULL;
    }

 public:
  static T* GetInstance(void)
  {
    if (!singleton)
	singleton = new T();
    return singleton;
  }

  static void CleanUp(void) { if (singleton) delete singleton; };
  
};

template<typename T>
T * Singleton<T>::singleton = NULL;

#endif	/*	__SINGLETON_H__	*/
