/*
 * (naive) parallel implementation of RankSort algorithm
 *
 * @author: Apan Qasem <apan@txstate.edu>
 * @date: 04/02/20
 */


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<sys/time.h>


#define REPS 100

#define VAL_RANGE 1024


double t0;
double mysecond() {
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp, &tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

t0 = mysecond();
t0 = (mysecond() - t0) * 1.e3;
printf("parallel loop = %3.2f ms\n", t0);

/*
 * apply rank sort to values and place sorted values in sorted_values
 */
void ranksort(double *values, long long N, double *sorted_values)
{

  unsigned *ranks = (unsigned *)malloc(sizeof(unsigned) * N);
  // find ranks
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < i; j++)
    {
      if (values[i] >= values[j])
        ranks[i]++;
      else
        ranks[j]++;
    }
  }

// sort values
#pragma omp parallel for
  for (i = 0; i < N; i++)
    sorted_values[ranks[i]] = values[i];

  return;
}

void display(double *values, long long N)
{
  int i;
  for (i = 0; i < N; i++)
    printf("%3.2f ", values[i]);
  printf("\n");
  return;
}



int main(int argc, char* argv[]) {

 int **a, **b;

  long long N = atoi(argv[1]);        //inputs
  unsigned threads = atoi(argv[2]);   //threads

  if (argc < 3) {
    printf("usage: \n");
    printf("       ./ranksort N threads\n");
    exit(0);
  }
  


  a = (int **) malloc(sizeof(int *) * N);
  b = (int **) malloc(sizeof(int *) * N);  

int i, j, k;
  for (i = 0; i < N; i++) {
    a[i] = (int *) malloc(sizeof(int) * N);
    b[i] = (int *) malloc(sizeof(int) * N);
  }


    t0 = mysecond();
#pragma omp parallel for private(j,i)
  for (k = 0; k < REPS; k++) {
    for (j = 0; j < N; j++) 
      for (i = 0; i < N; i++)
	a[i][j] = b[i][j] * 17;
  }

  t0 = (mysecond() - t0) * 1.e3;
  printf("parallel loop = %3.2f ms\n", t0);

  return 0;

  // number of threads to be used in the parallel run
  omp_set_num_threads(threads);

  double *values = (double *)malloc(sizeof(double) * N);
  double *sorted_values = (double *)malloc(sizeof(double) * N);

  for (int i = 0; i < N; i++)
    values[i] = rand() / (double)(RAND_MAX / VAL_RANGE);

  ranksort(values, N, sorted_values);
  printf("result = %3.2f\n", sorted_values[N - 1]);
}
