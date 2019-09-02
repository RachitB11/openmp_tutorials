#include <stdio.h>
#include <omp.h>

// Intgrate 4/(1+x^2) from 0 to 1. Should give pi.
int main()
{
  static long num_steps = 100000;
  double step;
  double x,pi,sum=0.0;
  step = 1/(double) num_steps;

  double start_t = omp_get_wtime();

  for(int i=0; i<num_steps; i++)
  {
    x = (i+0.5)*step;
    sum+=4/(1.0+(x*x));
  }

  double end_t = omp_get_wtime();

  // Since the step size is same for all heights the result is
  pi = step*sum;
  printf("pi: %lf in time %lf seconds\n",pi, end_t-start_t);
  return 0;
}
