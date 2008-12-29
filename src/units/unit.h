#ifndef __UNIT_H__
#define __UNIT_H__

#include <string>
#include <animation.h>
#include <object/drawable_object.h>
#include <tools/base.h>


class Unit: public DrawableObject
{

  public:

    Unit (const Point & foots, const std::string & xmldoc);

    virtual ~Unit () {};

    void startWalk (const Point & dest);

    void stopWalk ();

    void refresh (void);

    inline Point getPos(void) const
    {
      return getTopLeftCorner();
    }
    
    /*
    inline Point getFoots(void) const
    {
      return getMiddleBottom();
      }*/

    inline bool isWalking(void) const
    {
      return m_walking;
    }

    bool isMoving(void) const
    {
      return isWalking();
    }

    // Mass (in kg)
    inline double getMass(void) const
    {
      return 5.0;
    }
    
    // "surface" (in m²) in contact with air force
    inline double getSurfaceResistance(void) const
    {
      return 1.728;
    }
    
    // Experimental Aerodynamic coefficiant (SI)
    inline double getAeroFactor(void) const
    {
      return 0.05;
    }

  private:
    string m_name;

    Point m_dest;

    Animation m_body;
    
    bool m_walking;

};

#endif
