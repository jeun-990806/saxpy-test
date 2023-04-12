/* Author: Kim Jieun 
 * Date of creation: 2023-04-07
 * Usage: ./saxpy n a
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void saxpy(int n, float a, float *x, float *y){
	for(int i=0; i<n; i++)
		y[i] = a*x[i]+y[i];
}

int main(int argc, char *argv[]){
	int n; float a;
	float *x, *y;
	clock_t start, end;

	n = atoi(argv[1]);
	a = atof(argv[2]);
	x = (float *)malloc(sizeof(float)*n);
	y = (float *)malloc(sizeof(float)*n);
	for(int i=0; i<n; i++){
		x[i] = 0.001;
		y[i] = 0.01;
	}

	printf("Start saxpy...\n");
	start = clock();
	saxpy(n, a, x, y);
	end = clock();
	printf("Saxpy is ended. ");
	printf("Done in %lfs.\n", (double)(end-start)/CLOCKS_PER_SEC);
	return 0;
}
