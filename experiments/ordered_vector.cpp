#include<vector>
#include<iostream>
#include<omp.h>
#include<stdlib.h>

#define VEC_SIZE 1000000000
#define NUM_THREADS 4

int main()
{
  std::vector<int> vec;
  vec.reserve(VEC_SIZE);
  size_t *prefix;

  double start_t = omp_get_wtime();

  // Single thread push_back
  // for(int i=0;i<VEC_SIZE;i++) vec.push_back(i);

  // Multi threaded push_back where order matters
  // omp_set_num_threads(NUM_THREADS);
  // #pragma omp parallel
  // {
  //     int ithread  = omp_get_thread_num();
  //     int nthreads = omp_get_num_threads();
  //     #pragma omp single
  //     {
  //         prefix = new size_t[nthreads+1];
  //         prefix[0] = 0;
  //     }
  //     std::vector<int> vec_private;
  //     #pragma omp for schedule(static) nowait
  //     for(int i=0; i<VEC_SIZE; i++) {
  //         vec_private.push_back(i);
  //     }
  //     prefix[ithread+1] = vec_private.size();
  //     #pragma omp barrier
  //     #pragma omp single
  //     {
  //         for(int i=1; i<(nthreads+1); i++) prefix[i] += prefix[i-1];
  //         vec.resize(vec.size() + prefix[nthreads]);
  //     }
  //     std::copy(vec_private.begin(), vec_private.end(), vec.begin() + prefix[ithread]);
  // }
  // delete[] prefix;

  // Multi threaded but order does not matter
  omp_set_num_threads(NUM_THREADS);
  #pragma omp declare reduction (merge : std::vector<int> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
  #pragma omp parallel for reduction(merge: vec)
  for(int i=0; i<VEC_SIZE; i++) vec.push_back(i);

  double end_t = omp_get_wtime();

  // for(int i=0; i<vec.size(); i++)
  // {
  //   std::cout<<vec[i]<<" ";
  // }
  // std::cout<<std::endl;

  std::cout<<"Time taken : "<<end_t-start_t<<std::endl;

}
