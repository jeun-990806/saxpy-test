#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "tools.h"

void saxpy(int n, float a, float *x, float *y){
        for(int i=0; i<n; i++)
                y[i] = a*x[i]+y[i];
}

int main(int argc, char *argv[]){
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;
	int n; float a;
	float *x, *y;

	clock_t start, end, tmp;
	int read_bytes, write_bytes;

	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(server_sock, 100);
	client_addr_size = sizeof(client_addr);

	x = (float *)malloc(sizeof(float));
	y = (float *)malloc(sizeof(float));
	
	while(1){
		read_bytes = 0; write_bytes = 0;
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
		read_bytes += recv(client_sock, (void *)&n, sizeof(int), MSG_WAITALL);
		if(n == -1) break;
		read_bytes += recv(client_sock, (void *)&a, sizeof(float), MSG_WAITALL);
		x = realloc(x, sizeof(float)*n);
		y = realloc(y, sizeof(float)*n);

		read_bytes += recv(client_sock, x, sizeof(float)*n, MSG_WAITALL);
		read_bytes += recv(client_sock, y, sizeof(float)*n, MSG_WAITALL);
	
		printf("Received data: n=%d, a=%f, x[]={%f, ..., %f}, y[]={%f, ..., %f} (Total: %dBytes)\n", n, a, x[0], x[n-1], y[0], y[n-1], read_bytes);
		
		saxpy(n, a, x, y);

		printf("saxpy() result: y[]={%f, ..., %f}\n", y[0], y[n-1]);

		write_bytes = send(client_sock, y, sizeof(float)*n, 0);
		printf("Send %dBytes\n", write_bytes);
		close(client_sock);
	}
	close(server_sock);
	return 0;
}
