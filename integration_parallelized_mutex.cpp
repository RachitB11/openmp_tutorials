#include <stdio.h>
#include <omp.h>
#include <mutex>

std::mutex sum_mutex;

// Intgrate 4/(1+x^2) from 0 to 1. Should give pi.
int main()
{
  static long num_steps = 100000;
  double step;
  double pi,sum=0.0;

  static int num_threads = 4;


  step = 1/(double) num_steps;

  // Requests some number of threads
  omp_set_num_threads(num_threads);

  // Set the partition size in each thread
  static long partition_size = num_steps/num_threads;

  // Init time
  double start_t = omp_get_wtime();

  #pragma omp parallel
  // The scope defines the structured block
  {
    double x;

    // This gives the identifier for the thread
    int ID=omp_get_thread_num();
    long begin = ID*partition_size;
    long end = ((ID+1)*partition_size);
    double partial_sum=0.0;
    for(int i=begin; i<end; i++)
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
