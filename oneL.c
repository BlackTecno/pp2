#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

//MAX SIZE OF MATRIX
#define MAX_SIZE 2000

void mult(int size, int *threads);
double get_seconds();

//GLOBAL MATRIXIES (Matricies?)
volatile float a[MAX_SIZE][MAX_SIZE];
volatile float b[MAX_SIZE][MAX_SIZE];
volatile float c[MAX_SIZE][MAX_SIZE];


int main(int argc, char **argv) {
	int size, threads, i, j, check = 0;
	double start, end, time;

	while (check == 0) {
		printf("Enter <size of Matrix>, <Nuber of Threads>:");
		scanf("%d, %d", size, threads);

		if (size > MAX_SIZE || size <= 0) {
			printf("Incorrect size for matix, please input a number between 1 and %d\n", MAX_SIZE);}
		else {
			check = 1; }


		srand(time(NULL));
	}


	//MAKE MATRIX
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			a[i][j] = (float)rand() / 10000;
			b[i][j] = (float)rand() / 10000;
		}
	}

	start = get_seconds();

	/* Timed matrix-multiplication */
	mult(size, &threads);

	/* Get end_time & calculate exec_time*/
	end = get_seconds();
	time = end - start;
}

void mult(int size, int *threads) {
	int i, j, k;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			
			c[i][j] = 0;

			#pragma omp parallel for shared(a, b, c, size) scedule(static)
			for (k = 0; k < size; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}


double get_seconds()
{
	struct timeval tvs;
//	struct timezone tzs;

	gettimeofday(&tvs, NULL);

	return((double)tvs.tv_sec + (double)(tvs.tv_usec * 1.e-6));
}