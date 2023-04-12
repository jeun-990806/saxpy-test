#include <math.h>
#include <time.h>

double get_seconds(clock_t t0, clock_t t1){
	return (double)(t1-t0)/CLOCKS_PER_SEC;
}

double get_average(double *arr, int n){
	double sum = 0;
	for(int i=0; i<n; i++) sum += arr[i];
	return sum/n;
}

double get_sd(double *arr, int n){
	double sum = 0;
	double avg = get_average(arr, n);
	for(int i=0; i<n; i++) sum += pow(arr[i]-avg, 2);
	return sqrt(sum/n);
}







