#include "euler_vector.h"

EulerVector::EulerVector(void)
{
  Clear();
}

void
EulerVector::setSpeed(double speed, double delta_t)
{
  x1 = speed;
  x0 = x0 + x1 * delta_t;
}


// All coeff are know , so we need to compute
// explicitly x2.
// and find x1 and x0 using euler method
// so  V(t + delta_t) = V(t) + A(t) * delta_t
// and X(t + delta_t) = X(t) + V(t) * delta_t
void
EulerVector::ComputeEulerEquation(double a, double b, double c, double d, 
				  double delta_t)
{
  x2 = (d - b * x1 - c * x0) / a;
  x1 = x1 + x2 * delta_t;
  x0 = x0 + x1 * delta_t;
}
 
