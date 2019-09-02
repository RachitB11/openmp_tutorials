#include <stdio.h>
#include <omp.h>


int main()
{
  // Says gimme a bunch of threads. Since we didn't specify the number
  // it will use the default number of threads (which it may get from the env variable
  // OMP_NUM_THREADS)
  #pragma omp parallel
  // The scope defines the structured block
  {
    // This gives the identifier for the thread
    int ID=omp_get_thread_num();
    printf("hello(%d)",ID);
    printf(" world(%d)\n",ID);
  }
  return 0;
}
