#include <stdio.h>
#include <omp.h>
#include <mutex>
#define NUM_THREADS 4

std::mutex sum_mutex;

// Intgrate 4/(1+x^2) from 0 to 1. Should give pi.
int main()
{
  static long num_steps = 100000000;
  double step;
  double pi,sum=0.0;

  static int num_threads = 4;


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
    double x, partial_sum=0.0;
    // Even though you request a number of threads you may not get all of them.
    // So you need to check the number of threads before.
    int nthrds = omp_get_num_threads();
    // This gives the identifier for the thread
    int ID=omp_get_thread_num();

    // For the main thread with root ID set the shared variable using the local variable
    if(ID==0) num_threads_actual = nthrds;

    // This is called round robin cycling (ID 0: 0,4,8,.. ID 1: 1,5,9,...)
    for(int i=ID; i<num_steps; i+=nthrds)
    {
      x = (i+0.5)*step;
      partial_sum+=4/(1.0+(x*x));
    }

    // Approach 1 to mutex
    // std::lock_guard<std::mutex> guard(sum_mutex);
    // sum+=partial_sum;

    // Approach 2 to mutex
    sum_mutex.lock();
    sum+=partial_sum;
    sum_mutex.unlock();
  }

  double end_t = omp_get_wtime();

  // Since the step size is same for all heights the result is
  pi = step*sum;
  printf("pi: %lf in time %lf seconds\n",pi, end_t-start_t);
  return 0;
}
