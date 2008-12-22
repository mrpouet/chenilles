#ifndef __FALL_VECTOR_H__
#define __FALL_VECTOR_H__


struct FallVector
{
  double x2, x1, x0;

  FallVector();

  inline void Clear(void)
  {
    x2 = x1 = x0 = 0.0;
  }

  void ComputeEulerEquation(double a, double b, double c, double d, 
			    double delta_t);
  
};

#endif
