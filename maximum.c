#include<stdio.h>
#include<stdlib.h>
#include <omp.h>
#include<time.h>
#define SIZE 1000000
int searchMax(int *a,int threads);
 int main(void)
   {    int i=0, my_max, a[SIZE], threads;
       double  start, end;
      srand(time(0));
      printf("Give me the number of threads you want to use:\n");
      scanf("%d", &threads);
      printf("\n");
  // Check processors and threads
  int processors = omp_get_num_procs ();	// Available processors
  printf ("Array size = %d\nProcesses = %d\nProcessors = %d\n",
	  SIZE, threads, processors);
  if (threads > processors)
    {
      printf
	("Warning: %d threads requested, will run_omp on %d processors available\n",
	 threads, processors);
      omp_set_num_threads (threads);
    }
  int max_threads = omp_get_max_threads ();	// Max available threads
  if (threads > max_threads)	// Requested threads are more than max available
    {
      printf ("Error: Cannot use %d threads, only %d threads available\n",
	      threads, max_threads);
      return 1;
    }
      for (i = 0; i < SIZE; i++)
         a[i]=rand();
       //Find maximum value 
        
        start = omp_get_wtime();

        printf("Maximum value in the matrix is : %d\n", searchMax(a , threads));

        end = omp_get_wtime();

        printf("Elapsed = %lf\n", end - start);

        printf("Success !\n");
        return 0;
   }
   int searchMax(int *a,int threads){

    int max[threads], my_max, i;

    omp_set_num_threads(threads);

    #pragma omp parallel
    {
        int id, n, begin, stop;

        int length = (sizeof(max)/sizeof(int));

        n = SIZE/length;                // step = SIZE/number of threads.

        id = omp_get_thread_num();       // id is one of 0, 1, ..., (threads -1)

        begin = id * n;

        if( id != (threads - 1) )
        {
            stop = begin + n;
        }
        else
        {
            stop = SIZE;
        }

        my_max = a[begin];

        for (i = begin+1; i < stop; i++ )
        {
            if(a[i] > my_max )
                my_max = a[i];
        }

        max[id] = my_max;	// Store result in max[id]   
      }
      // ----------------------------------------
      // Post processing: Find actual maximum
      // ----------------------------------------
      my_max = max[0];
         
      for (i = 1; i < threads; i++)
         if ( max[i] > my_max )
            my_max = max[i];

    return my_max;
}
