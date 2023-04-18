#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "tools.h"
#include "UDP.h"

#define TCP 0
#define UDP 1

void saxpy(int n, float a, float *x, float *y){
        for(int i=0; i<n; i++)
                y[i] = a*x[i]+y[i];
}

int main(int argc, char *argv[]){
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;
	char client_ip_address[INET_ADDRSTRLEN+1];

	int n; float a;
	float *x, *y;

	int protocol = TCP;

	if(strcmp(argv[2], "UDP") == 0) protocol = UDP;

	if(protocol == TCP) server_sock = socket(PF_INET, SOCK_STREAM, 0);
	else if(protocol == UDP) server_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(server_sock == -1){
		printf("socket() error\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		printf("bind() error\n");
		exit(1);
	}
	if(protocol == TCP) listen(server_sock, 100);
	client_addr_size = sizeof(client_addr);

	x = (float *)malloc(sizeof(float));
	y = (float *)malloc(sizeof(float));
	
	while(1){
		if(protocol == TCP){
			client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
			inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip_address, INET_ADDRSTRLEN);
			printf("Connection request from %s\n", client_ip_address);
		}

		if(protocol == TCP){
			recv(client_sock, (void *)&n, sizeof(int), MSG_WAITALL);
			if(n == -1) break;
			recv(client_sock, (void *)&a, sizeof(float), MSG_WAITALL);
			x = realloc(x, sizeof(float)*n);
			y = realloc(y, sizeof(float)*n);
			recv(client_sock, x, sizeof(float)*n, MSG_WAITALL);
			recv(client_sock, y, sizeof(float)*n, MSG_WAITALL);
		}else if(protocol == UDP){
			recvfrom(server_sock, (void *)&n, sizeof(int), MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_size);
			if(n == -1) break;
			recvfrom(server_sock, (void *)&a, sizeof(float), MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_size);
			
			x = realloc(x, sizeof(float)*n);
			y = realloc(y, sizeof(float)*n);
			recvall(server_sock, x, n, sizeof(float), (struct sockaddr *)&client_addr, &client_addr_size);
			recvall(server_sock, y, n, sizeof(float), (struct sockaddr *)&client_addr, &client_addr_size);
		}
		printf("Received data: n=%d, a=%f, x[]={%f, ..., %f}, y[]={%f, ..., %f}\n", n, a, x[0], x[n-1], y[0], y[n-1]);
		
		saxpy(n, a, x, y);
		printf("saxpy() result: y[]={%f, ..., %f}\n", y[0], y[n-1]);
		
		if(protocol == TCP){
			send(client_sock, y, sizeof(float)*n, 0);
			if(close(client_sock) == 0)
				printf("Result transmission is done. Disconnect.\n");
		}else if(protocol == UDP){
			sendall(server_sock, y, n, sizeof(float), (struct sockaddr *)&client_addr, client_addr_size);
			printf("Result transmission is done.\n");
		}
	}
	close(server_sock);
	return 0;
}
