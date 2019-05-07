/*
 * Brody Williams
 * CS 5379 Spring 2019 - Dr. Chen
 * Programming Project 2 Q1.i
 */

/* Code taken from Introduction to Parallel Computing by Ananth Grama et. al */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/* Maximum matrix dimension size */
#define MAX_DIM 5000

/* Prototypes */
void matrix_multiply(int dim_size, int *num_threads);
double get_seconds();
void init_matrices(int dim_size);

/* Global matrices (float arrays similar to q2)*/
volatile float a[MAX_DIM][MAX_DIM];
volatile float b[MAX_DIM][MAX_DIM];
volatile float c[MAX_DIM][MAX_DIM];

int main(int argc, char **argv)
{
	/* Initialize variables */
	int dim_size = 0, num_threads, retval;
	double start, end, exec_time;

	/* Parse command-line args */
	while((retval = getopt(argc, argv, "d:")) != -1)
	{
		switch(retval)
		{
			case 'd':
				dim_size = atoi(optarg);
				break;

			default:
				printf("Unrecognized switch!\n");
				return -1;
				break;
		}
	}

	if(dim_size <= 0 || dim_size > MAX_DIM)
	{
		printf("Please provide a valid matrix dimension size!\n");
		return -1;
	}
	
	/* Initalize matrices */
	init_matrices(dim_size);

	/* Get start_time */
	start = get_seconds();

	/* Timed matrix-multiplication */
	matrix_multiply(dim_size, &num_threads);

	/* Get end_time & calculate exec_time*/
	end = get_seconds();
	exec_time = end - start;

	/* Print results */
	printf("********************************************************************\n");
	printf("Test Results - Matrix Multiplication - Innermost Loop Parallelized\n");
	printf("Threads: %d Matrix Dimensions: %d x %d\n", num_threads, dim_size, dim_size);
	printf("********************************************************************\n");
	printf("Matrix Computation Time %f\n", exec_time);
	printf("********************************************************************\n");
	 
	return 0;
}

void matrix_multiply(int dim_size, int * num_threads)
{
	int i, j, k;
	
	/* Using the clauses specified by example 7.14 */

	for(i = 0; i < dim_size; i++)
	{
		for(j = 0; j < dim_size; j++)
		{
			c[i][j] = 0;
			
			#pragma omp parallel shared(a, b, c, dim_size)
			{
				*num_threads = omp_get_num_threads();
				
				#pragma omp for schedule(static)
				for(k = 0; k < dim_size; k++)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			} 
		}
	}
}	


void init_matrices(int dim_size)
{
	int i, j;
	srand(time(NULL));

	/* Initalize matrices with random values */
	for(i = 0; i < dim_size; i++)
	{
		for(j = 0; j < dim_size; j++)
		{
			a[i][j] = (float)rand() / 32768.0;
			b[i][j] = (float)rand() / 32768.0;		
		}
	}
}

double get_seconds()
{
	struct timeval tvs;
	struct timezone tzs;

	gettimeofday(&tvs, &tzs);

	return((double) tvs.tv_sec + (double) (tvs.tv_usec * 1.e-6));
}
