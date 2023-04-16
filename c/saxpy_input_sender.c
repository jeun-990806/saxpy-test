#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include "tools.h"
#include "UDP.h"

#define CONNECTION 0
#define TRANSMISSION 1
#define RECEPTION 2
#define TCP 0
#define UDP 1

void init_arr(float *arr, int size, float value){
	for(int i=0; i<size; i++)
		arr[i] = value;
}

int main(int argc, char *argv[]){
	int server_sock;
	struct sockaddr_in server_addr;
	socklen_t server_addr_len;
	
	int n; float a;
	float *x, *y;
	
	int executions;
	double **records;
	clock_t start;

	int protocol = TCP;
	
	if(argc < 6){
		printf("Usage: ./saxpy_input_sender host_ip, port, n, a, number_of_executions [optional: TCP/UDP, default is TCP]\n");
		exit(1);
	}

	n = atoi(argv[3]);
	a = atof(argv[4]);
	x = (float *)malloc(sizeof(float)*n);
	y = (float *)malloc(sizeof(float)*n);

	executions = atoi(argv[5]);
	records = (double **)malloc(sizeof(double *)*3);
	for(int i=0; i<3; i++)
		records[i] = (double *)malloc(sizeof(double)*executions);

	if(argc == 7 && (strcmp(argv[6], "UDP") == 0)) protocol = UDP;

	server_addr_len = sizeof(server_addr);
	memset(&server_addr, 0, server_addr_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	for(int i=0; i<executions; i++){
		init_arr(x, n, 0.01);
		init_arr(y, n, 0.001);
		if(protocol == TCP)
			server_sock = socket(PF_INET, SOCK_STREAM, 0);
		else if(protocol == UDP)
			server_sock = socket(PF_INET, SOCK_DGRAM, 0);
		printf("Start saxpy... (%d/%d)\n", i+1, executions);

		start = clock();
		connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
		records[CONNECTION][i] = get_seconds(start, clock());
		printf("Connection: %lfs\n", records[CONNECTION][i]);
		
		start = clock();
		if(protocol == TCP){
			send(server_sock, (void *)&n, sizeof(int), MSG_MORE);
			send(server_sock, (void *)&a, sizeof(float), MSG_MORE);
			send(server_sock, x, sizeof(float)*n, MSG_MORE);
			send(server_sock, y, sizeof(float)*n, 0);
		}else if(protocol == UDP){
			sendto(server_sock, (void *)&n, sizeof(int), 0, (struct sockaddr *)&server_addr, server_addr_len);
			sendto(server_sock, (void *)&a, sizeof(float), 0, (struct sockaddr *)&server_addr, server_addr_len);
			sendall(server_sock, x, n, sizeof(float), (struct sockaddr *)&server_addr, server_addr_len);
			sendall(server_sock, y, n, sizeof(float), (struct sockaddr *)&server_addr, server_addr_len);
		}
		records[TRANSMISSION][i] = get_seconds(start, clock());
		printf("Transmission: %lfs\n", records[TRANSMISSION][i]);
		if(protocol == TCP) shutdown(server_sock, SHUT_WR);
	
		start = clock();
		if(protocol == TCP)
			recv(server_sock, y, sizeof(float)*n, MSG_WAITALL);
		else if(protocol == UDP){
			sleep(2);
			recvall(server_sock, y, n, sizeof(float), (struct sockaddr *)&server_addr, &server_addr_len);
		}
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
