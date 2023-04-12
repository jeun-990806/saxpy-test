#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

double to_sec(clock_t t0, clock_t t1){
	return (double)(t1-t0)/CLOCKS_PER_SEC;
}

int main(int argc, char *argv[]){
	int server_sock;
	struct sockaddr_in server_addr;
	clock_t start, end, tmp;

	int n = atoi(argv[3]);
	float a = atof(argv[4]);
	float *x, *y;

	x = (float *)malloc(sizeof(float)*n);
	y = (float *)malloc(sizeof(float)*n);

	for(int i=0; i<n; i++){
		x[i] = 0.001;
		y[i] = 0.01;
	}

	int rb, offset;

	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	start = clock();
	tmp = clock();
	connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("Connection: %lfs\n", to_sec(tmp, clock()));
	tmp = clock();
	send(server_sock, (void *)&n, sizeof(int), MSG_MORE);
	send(server_sock, (void *)&a, sizeof(float), MSG_MORE);
	send(server_sock, x, sizeof(float)*n, 0);
	send(server_sock, y, sizeof(float)*n, 0);

	printf("Send all inputs: %lfs\n", to_sec(tmp, clock()));
	shutdown(server_sock, SHUT_WR);
	offset = 0;
	tmp = clock();
	recv(server_sock, y, sizeof(float)*n, MSG_WAITALL);
	printf("Receive result: %lfs\n", to_sec(tmp, clock()));
	end = clock();
	printf("Done in %lfs.\n", (double)(end-start)/CLOCKS_PER_SEC);
	close(server_sock);
	printf("y[] = {%f, %f, %f, ..., %f}\n", y[0], y[1], y[2], y[n-1]);
	free(x);
	free(y);

	return 0;
}
