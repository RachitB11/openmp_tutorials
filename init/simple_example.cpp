#include <stdio.h>
#include <omp.h>

// THIS CODE IS JUST FOR ILLUSTRATION. DO NOT COMPILE

/* NOTES:
- If the data is allocated in parallel scope then its gonna be allocated in the stack
 private to the thread
- If its declared the scope, it sits in the heap and is sharable between threads.
- Here A is in the heap and ID is in the stack
- Under the hood its creating pthreads and packages everything in the scope of the
  parallel construct. It then runs the method in n-1 new threads and 1 main thread.
  Check the pthreads example in the c++ turorials I did.
*/


int main()
{
  double A[1000]

  // Requests some number of threads
  omp_set_num_threads(4);

  // Says gimme a bunch of threads. Since we didn't specify the number
  // it will use the default number of threads (which it may get from the env variable
  // OMP_NUM_THREADS)
  #pragma omp parallel
  // The scope defines the structured block
  {
    // This gives the identifier for the thread
    int ID=omp_get_thread_num();
    // Some random method
    pooh(ID,A);
  }
  printf("All done\n");

  return 0;
}
