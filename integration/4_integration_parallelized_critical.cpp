#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 4

// Intgrate 4/(1+x^2) from 0 to 1. Should give pi.
int main()
{
  static long num_steps = 100000000;
  double step;
  double pi,sum=0.0;

  step = 1/(double) num_steps;

  // Requests some number of threads
  omp_set_num_threads(NUM_THREADS);

  // Variable to store the actual number of threads which may be lesser than the one requested
  int num_threads_actual;

  // Init time
  double start_t = omp_get_wtime();

  #pragma omp parallel
  // The scope defines the structured block
  {
    double x;
    // Even though you request a number of threads you may not get all of them.
    // So you need to check the number of threads before.
    int nthrds = omp_get_num_threads();
    // This gives the identifier for the thread
    int ID=omp_get_thread_num();

    // For the main thread with root ID set the shared variable using the local variable
    if(ID==0) num_threads_actual = nthrds;

    double partial_sum = 0.0;
    // This is called round robin cycling (ID 0: 0,4,8,.. ID 1: 1,5,9,...)
    for(int i=ID; i<num_steps; i+=nthrds)
    {
      x = (i+0.5)*step;
      partial_sum+=4/(1.0+(x*x));
    }

    // Blocking the thread till the critical section becomes available for this thread
    // Really need to think about the where the critical section is set or you might just
    // serialize the algorithm
    #pragma omp critical
    {
      sum += partial_sum;
    }
  }
  double end_t = omp_get_wtime();

  // Since the step size is same for all heights the result is
  pi = step*sum;
  printf("pi: %lf in time %lf seconds\n",pi, end_t-start_t);
  return 0;
}
