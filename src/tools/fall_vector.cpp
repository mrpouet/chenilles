#include "fall_vector.h"


FallVector::FallVector(void)
{
  Clear();
}

// All coeff are know , so we need to compute
// explicitly x2.
// and find x1 and x0 using euler method
// so  V(t + delta_t) = V(t) + A(t) * delta_t
// and X(t + delta_t) = X(t) + V(t) * delta_t
void
FallVector::ComputeEulerEquation(double a, double b, double c, double d, 
			       double delta_t)
{
  x2 = (d - b * x1 - c * x0) / a;
  x1 = x1 + x2 * delta_t;
  x0 = x0 + x1 * delta_t;
}
 
