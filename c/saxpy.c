/* Author: Kim Jieun 
 * Date of creation: 2023-04-07
 * Usage: ./saxpy n a
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tools.h"

void saxpy(int n, float a, float *x, float *y){
	for(int i=0; i<n; i++)
		y[i] = a*x[i]+y[i];
}

int main(int argc, char *argv[]){
	int n; float a;
	float *x, *y;

	int executions;
	double *records;
	clock_t start, end;

	if(argc != 4){
		printf("Usage: ./saxpy n a number_of_executions\n");
		exit(1);
	}

	n = atoi(argv[1]);
	a = atof(argv[2]);
	x = (float *)malloc(sizeof(float)*n);
	y = (float *)malloc(sizeof(float)*n);
	for(int i=0; i<n; i++){
		x[i] = 0.001;
		y[i] = 0.01;
	}
	executions = atoi(argv[3]);
	records = (double *)malloc(sizeof(double)*executions);
	
	for(int i=0; i<executions; i++){
		printf("Start saxpy...(%d/%d)\n", i+1, executions);
		start = clock();
		saxpy(n, a, x, y);
		end = clock();
		printf("Saxpy is ended. ");
		records[i] = get_seconds(start, end);
		printf("Done in %lfs.\n", records[i]);
	}
	printf("======================  Summary\n");
	printf("Average: %lfs\n", get_average(records, executions));
	printf("Standard deviation: %lf\n", get_sd(records, executions));
	printf("===============================\n");

	free(x);
	free(y);
	free(records);

	return 0;
}
