#include <iostream>
#include <map/map.h>

int
main (int argc, char *argv[])
{

  try 
    {
      Map map("../data/maps/testMap1/config.xml");
    }
  catch(const std::exception& e)
    {
      cerr << "Caught exception:" << endl
	   << "what(): " << e.what() << endl;
    }
  return 0;
}
