#ifndef __DRAWABLE_OBJECT_H__
#define __DRAWABLE_OBJECT_H__

#define _XOPEN_SOURCE 600
#include <cmath>
#include <rectangle.h>
#include <point.h>
#include <game/game.h>
#include <tools/base.h>
#include <tools/euler_vector.h>

class DrawableObject
{
 public:

  DrawableObject();

  virtual ~DrawableObject() {};

  virtual void refresh(void) = 0;

  virtual double getSurfaceResistance(void) const = 0;

  virtual double getMass(void) const = 0;

  virtual double getAeroFactor(void) const  = 0;

  virtual bool isMoving(void) const = 0;

  bool hasCollidingWith(const DrawableObject& object);


  inline void setMaxSpeed(double max_speed)
  {
    m_max_speed = max_speed;
  }

  inline Uint16 getWidth(void) const
  {
    return m_width;
  }

  inline Uint16 getHeight(void) const
  {
    return m_height;
  }
  
  inline Point getTopLeftCorner(void) const
  { 
    Sint16 x = static_cast<Sint16>(round(m_x.x0 * Game::PIXELS_PER_METER));
    Sint16 y = static_cast<Sint16>(round(m_y.x0 * Game::PIXELS_PER_METER));
    return Point(x, y);
  }

  inline void setTopLeftCorner(const Point& pos)
  {
    m_x.x0 = (double)pos.x / Game::PIXELS_PER_METER;
    m_y.x0 = (double)pos.y / Game::PIXELS_PER_METER;
  }

  inline Point getMiddleBottom(void) const
  {
    return (getTopLeftCorner() + Point(m_width / 2, m_height));
  }

  void ComputeNewXY(void);


 protected:
  Uint16 m_width;

  Uint16 m_height;

 private:

  static const double gravity_force;

  static const double air_density;

  static const double delta_t;

  EulerVector m_x; 

  EulerVector m_y;

  double m_last_move;
  
  double m_max_speed;

  void ComputeFallXY(double dt);

  void ComputeSlopeXY (Point& contact, double dt);

  bool FindFootsContactOnGround(Point& contact);
  
  bool FindContactOnGround(Point &contact);

};

#endif
