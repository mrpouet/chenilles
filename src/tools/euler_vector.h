#ifndef __FALL_VECTOR_H__
#define __FALL_VECTOR_H__


struct EulerVector
{
  double x2, x1, x0;

  EulerVector();

  inline void Clear(void)
  {
    x2 = x1 = x0 = 0.0;
  }

  void setSpeed(double speed, double delta_t);

  void ComputeEulerEquation(double a, double b, double c, double d, 
			    double delta_t);
  
};

#endif
