#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "tools.h"

#define CONNECTION 0
#define TRANSMISSION 1
#define RECEPTION 2

int main(int argc, char *argv[]){
	int server_sock;
	struct sockaddr_in server_addr;
	
	int n; float a;
	float *x, *y;
	
	int executions;
	double **records;
	clock_t start;
	
	if(argc != 6){
		printf("Usage: ./saxpy_input_sender host_ip, port, n, a, number_of_executions\n");
		exit(1);
	}

	n = atoi(argv[3]);
	a = atof(argv[4]);
	x = (float *)malloc(sizeof(float)*n);
	y = (float *)malloc(sizeof(float)*n);
	for(int i=0; i<n; i++){
		x[i] = 0.001;
		y[i] = 0.01;
	}

	executions = atoi(argv[5]);
	records = (double **)malloc(sizeof(double *)*3);
	for(int i=0; i<3; i++)
		records[i] = (double *)malloc(sizeof(double)*executions);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	for(int i=0; i<executions; i++){
		server_sock = socket(PF_INET, SOCK_STREAM, 0);
		printf("Start saxpy... (%d/%d)\n", i+1, executions);
		start = clock();
		connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
		records[CONNECTION][i] = get_seconds(start, clock());
		printf("Connection: %lfs\n", records[CONNECTION][i]);
		
		start = clock();
		send(server_sock, (void *)&n, sizeof(int), MSG_MORE);
		send(server_sock, (void *)&a, sizeof(float), MSG_MORE);
		send(server_sock, x, sizeof(float)*n, 0);
		send(server_sock, y, sizeof(float)*n, 0);
		records[TRANSMISSION][i] = get_seconds(start, clock());
		printf("Transmission: %lfs\n", records[TRANSMISSION][i]);
		shutdown(server_sock, SHUT_WR);
	
		start = clock();
		recv(server_sock, y, sizeof(float)*n, MSG_WAITALL);
		records[RECEPTION][i] = get_seconds(start, clock());
		printf("Reception: %lfs\n", records[RECEPTION][i]);
		printf("Saxpy is ended. Done in %lfs.\n", records[CONNECTION][i]+records[TRANSMISSION][i]+records[RECEPTION][i]);
		close(server_sock);
#ifdef DEBUG
		printf("y[] = {%f, %f, %f, ..., %f}\n", y[0], y[1], y[2], y[n-1]);
#endif
	}

	printf("======================  Summary\n");
	printf("Averages:\n");
	printf("\tConnection time: %lfs\n", get_average(records[CONNECTION], executions));
	printf("\tTransmission time: %lfs\n", get_average(records[TRANSMISSION], executions));
	printf("\tReception time: %lfs\n", get_average(records[RECEPTION], executions));
	printf("Standard deviations:\n");
	printf("\tConnection time: %lf\n", get_sd(records[CONNECTION], executions));
	printf("\tTransmission time: %lf\n", get_sd(records[TRANSMISSION], executions));
	printf("\tReception time: %lf\n", get_sd(records[RECEPTION], executions));
	printf("===============================\n");

	free(x);
	free(y);
	for(int i=0; i<3; i++)
		free(records[i]);
	free(records);

	return 0;
}
