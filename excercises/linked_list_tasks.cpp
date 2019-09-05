#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
   int data;
   int fibdata;
   struct node* next;
};

int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

void processwork(struct node* p)
{
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;

    // head = struct node* malloc(sizeof(struct node));
    head = new struct node;
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i=0; i< N; i++) {
       // temp  = struct node* malloc(sizeof(struct node));
       temp  = new struct node;
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
     double start, end;
     struct node *p=NULL;
     struct node *temp=NULL;
     struct node *head=NULL;

	 printf("Process linked list\n");
     printf("  Each linked list node will be processed by function 'processwork()'\n");
     printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);

     p = init_list(p);
     head = p;

     start = omp_get_wtime();

     // Solution using simple while loop
     // while (p != NULL) {
     // processwork(p);
     // p = p->next;
     // }

     // Solution using tasks
     //Means that we have threads
     #pragma omp parallel
     {
       // Means that a single thread will be responsible for setting up the tasks. 
       // Once all tasks are queued up the threads start picking up the tasks
       // The implicit single block barrier is complete once all the tasks are completed
       #pragma omp single
       {
         for(p; p; p=p->next)
         {
           // Need the first private here because we don't want a clash in the p
           // value being accessed. Since p is a shared variable it can cause a
           // race condition. So we chose the current value and pass that as a
           // private variable to this task block.
           #pragma omp task firstprivate(p)
           processwork(p);
         }
       }

     }

     end = omp_get_wtime();
     p = head;
	 while (p != NULL) {
        printf("%d : %d\n",p->data, p->fibdata);
        temp = p->next;
        free (p);
        p = temp;
     }
	 free (p);

     printf("Compute Time: %f seconds\n", end - start);

     return 0;
}
