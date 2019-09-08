/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define N        10000000

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
   int i;
   for (i=0;i<length;i++) {
     randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
     *(a+i) = ((double) randy)/((double) RAND_MOD);
   }
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
   int i;  double sum = 0.0;
   for (i=0;i<length;i++)  sum += *(a+i);
   return sum;
}

int main()
{
  double *A, sum, runtime;
  int numThreads, flag = 0, flg_tmp;

  A = (double *)malloc(N*sizeof(double));

  runtime = omp_get_wtime();

  //  Here we are trying to do pairwise synchronization.
  //  I want the consumer in this case the sum_array to consume
  // and add to sum only when the data is available.
  // Originally I have to wait for the entire array to be filled
  // before I can start summing.

  // OpenMP lacks pairwise synchronization constructs that work between threads
  // Pair wise sync:
  // - Use a shared flad variable.
  // - Reader spins waiting for the new flag variable (Like a callback)
  // - A spin is basically an infinite loop listening for the flag changes
  // - Use flushes to force memory updates to and from memory

  // Setup the sections so that one section is a producer and the other is a consumer
  // This requires there be 2 threads atleast
  #pragma omp parallel sections
  {
    // Producer
    #pragma omp section
    {
      printf("Producer section thread id: %d\n", omp_get_thread_num());
      fill_rand(N, A);        // Producer: fill an array of data
      // This basically says hey I'm done producing so make A visible to the other
      // thread namely the consumer
      #pragma omp flush
      // This prevents the update from being halfway
      #pragma omp atomic write
      flag = 1;
      // This says hey I'm making the flag visible so anyone listening, here you
      // go
      #pragma omp flush(flag)
    }

    // Consumer
    #pragma omp section
    {
      printf("Consumer section thread id: %d\n", omp_get_thread_num());
      // Get the current value of flag first
      #pragma omp flush(flag)
      // While the flag has not been updated keep on listening to the flag.
      // This is basically called a spin lock.
      // A spin lock is an infinitely looping thread that listens for changes in variables.
      // Its like the spin locks in ROS that listen for changes in ROS msgs and trigger
      // the callbacks
      while(flag==0)
      {
        // If this flush didn't exist its possible that you would keep on looking
        // at the register value and keep on seeing 0. This forces the compiler
        // to go through the memory hierarchy and grab a fresh value of flag
        #pragma omp flush(flag)
        //Ensures we don't read midway through an update
        #pragma omp atomic read
          flg_tmp = flag;
        if(flg_tmp==1) break;
      }
      // Now flush all so that the value of A is refreshed
      #pragma omp flush
      sum = Sum_array(N, A);  // Consumer: sum the array
    }
  }

  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n",runtime,sum);
}
