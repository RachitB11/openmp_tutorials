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

     // Solution using pragma for but getting the linked list first
     int count = 0;
     while (p != NULL)
     {
       p = p->next;
       count++;
     }
     // printf("%d\n",count);
     p = head;
     struct node** parr = new struct node*[count];
     count = 0;
     while (p != NULL)
     {
       parr[count] = p;
       p = p->next;
       count++;
     }
     // omp_set_num_threads(6);
     #pragma omp parallel for default(none) firstprivate(count) shared(parr) schedule(static,1)
       for(int i=0; i<count;i++)
       {
       // printf("%d\n",p->data);
         processwork(parr[i]);
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
