#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 4
#define PAD 8

/*
NOTE:
- It is super important to note here that the there exists a shared cache but every
processor has its own cache line.
- A core can house 2 threads (See htop, num threads are usually double of the cores)
- So in a multithreaded process if the global elements being accessed in the structured code
region then there is a possibility that the cache line will have to flip flop between cores
to account for the changes. This is known as false sharing. They are sharing the same cache
but the modifications cause switches in the cache lines and hence added compiler work.
- Previously all of the sum_array will lie on the same cache line.
- A dirty work around is to allocate space so that the elements that will be accessed
will not lie on the same cache line
- So here we PAD it with 8 assuming a 64 byte cache line size
*/

// Intgrate 4/(1+x^2) from 0 to 1. Should give pi.
int main()
{
  static long num_steps = 100000000;
  double step;
  double pi,sum_array[NUM_THREADS][PAD];

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

    sum_array[ID][0] = 0.0;
    // This is called round robin cycling (ID 0: 0,4,8,.. ID 1: 1,5,9,...)
    for(int i=ID; i<num_steps; i+=nthrds)
    {
      x = (i+0.5)*step;
      sum_array[ID][0]+=4/(1.0+(x*x));
    }
  }

  double sum=0.0;
  for(int i=0;i<num_threads_actual;i++) sum+=sum_array[i][0];

  double end_t = omp_get_wtime();

  // Since the step size is same for all heights the result is
  pi = step*sum;
  printf("pi: %lf in time %lf seconds\n",pi, end_t-start_t);
  return 0;
}
