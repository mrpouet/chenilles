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
  Point p;

  // Current computer is too fast.
  if (dt < delta_t)
    dt = delta_t;

  if (world->isTheVacuum(getMiddleBottom()))
    ComputeFallXY(dt);
  else if (isMoving())
    ComputeSlopeXY(dt);

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

// Thanks to necropotame explinations about this part
void
DrawableObject::ComputeSlopeXY(double dt)
{

  double angle = world->computeAngle(getMiddleBottom(), Game::PIXELS_PER_METER);
  double cos_angle = cos(angle);
  double vx = 0.0;
  double vy = 0.0;

  printf("angle %lf°\n", (angle * 180) / M_PI);

  if (angle > 0.0)
    {
      vx = m_max_speed / cos_angle;
      vy = m_max_speed * tan(angle);
    }
  else
    {
      vx = m_max_speed * cos_angle * cos_angle;
      vy = m_max_speed * cos_angle * sin(angle);
    }

  m_x.setSpeed(vx, dt);
  m_y.setSpeed(vy, dt);

}
