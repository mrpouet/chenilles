#include <cstdio>
#include <cmath>

#include <timer.h>
#include <game/game.h>

#include "drawable_object.h"

// Earth gravity constant
const double DrawableObject::gravity_force = 9.81;

// Air density.
// We considering a constant temperature (in degrees) of 20 °C
// so we can find air density by:
// rh0 = 1.293 * (273 / (273 + 20))
const double DrawableObject::air_density   = 1.2047;

// Delta t for euler method
const double DrawableObject::delta_t       = 0.02;

DrawableObject::DrawableObject(void)
{
  m_x.Clear();
  m_y.Clear();
  m_last_move = Timer::GetRef().Read() / 1000.0;
}

bool
DrawableObject::hasCollidingWith(const DrawableObject& object)
{
  Point p_this = getTopLeftCorner();
  Point p_obj = object.getTopLeftCorner();

  return Rectangle(p_this.x, p_this.y, getWidth(), getHeight()).
    Intersect(Rectangle(p_obj.x, p_obj.y, 
	      object.getWidth(), object.getHeight()));
}

void
DrawableObject::ComputeNewXY(void)
{
  double dt = (Timer::GetRef().Read() / 1000.0) - m_last_move;
  Point contact = getMiddleBottom();

  dt = delta_t;

  if (world->isTheVacuum(contact))
    ComputeFallXY(dt);
  else if (isMoving())
    ComputeSlopeXY(contact, dt);

  //printf("Moving (%lf, %lf) --> (%lf, %lf)\n", oldpos_x, oldpos_y, 
  // newpos_x, newpos_y);

  m_last_move = Timer::GetRef().Read() / 1000.0;
}

// Thanks to fr.wikipedia.org
// for great explinations of these physics law ;)
void
DrawableObject::ComputeFallXY(double dt)
{
  double mass = getMass();

  // Newton second law:
  // sigma(Fext) = mass * acceleration_vector.
  // where the lvalue is the sum of all external forces of the object.
  // on y axys: weight - air_force = mass * ay.

  double air_force_factor = 0.5 * air_density * getAeroFactor() 
    * getSurfaceResistance();
  double weight_force = mass * gravity_force;

  // on y axys: m.y'' + k.y' = m.g
  // same form as euler equation : a.y'' + b.y' + c.y = d
  // with a = m , b = k, c = 0, and d = m.g .
  // so y'' = (d - b.y' - c.y) / a .

  m_y.ComputeEulerEquation(mass, air_force_factor, 0, 
			   weight_force, dt);
}

bool 
DrawableObject::FindFootsContactOnGround(Point &contact)
{
  int y1  = getMiddleBottom().y;
  int y2  = y1 - 1;
  int x1 = getTopLeftCorner().x;

  for (int x = x1; x <= (x1 + m_width); x++)
    {
      if (world->isTheGroundXY(x, y1) &&
	  world->isEmpty(x, y2))
	{
	  contact.x = x;
	  contact.y = y1;
	  return true;
	}
    }
  return false;
}

bool
DrawableObject::FindContactOnGround(Point &contact)
{
  int y1 = getMiddleBottom().y;
  int x1 = getTopLeftCorner().x + m_width;
  int x2 = x1 - 1;

  if (FindFootsContactOnGround(contact))
    return true;
  
  // Right
  for (int y = y1; y >= (y1 - m_height); y--)
    {
      if (world->isTheGroundXY(x1, y) &&
	  world->isEmpty(x2, y))
	{
	  contact.x = x1;
	  contact.y = y;
	  return true;
	}
    }
  x1 -= m_width;
  x2 = x1 + 1;

  // Left
  for (int y = y1; y >= (y1 - m_height); y--)
    {
      if (world->isTheGroundXY(x1, y) &&
	  world->isEmpty(x2, y))
	{
	  contact.x = x1;
	  contact.y = y;
	  return true;
	}
    }

  return false;
}

void
DrawableObject::ComputeSlopeXY(Point& contact, double dt)
{
  double angle = 0.0;

  FindContactOnGround(contact);

  angle = world->computeAngle(contact);

  printf("%s: angle %lf° -> %lf rads/contact (%d, %d)\n", 
	 __func__, (angle * 180) / M_PI, angle, contact.x, contact.y);

  // polar coords  => algebric coords
  // rho * exp(it) => rho * cos(t) + i * rho * sin(t).
  // where rho is speed strenght and t is slope
  m_x.setSpeed(m_max_speed * cos(angle), dt);
  m_y.setSpeed(m_max_speed * sin(angle), dt);
}
