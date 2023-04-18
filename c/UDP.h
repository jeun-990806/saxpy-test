#include <stdio.h>
#include <sys/socket.h>

ssize_t sendall(int socket, void *buffer, size_t length, size_t element_size, struct sockaddr *address, socklen_t address_len){
	ssize_t send_bytes = 0, total = 0;
	int offset = 0;

	while(1){
		send_bytes = sendto(socket, buffer+offset, (length-offset)*element_size, 0, address, address_len);

		if(send_bytes == -1) return -1;
		if(send_bytes + total >= length*element_size) return send_bytes + total;
		total += send_bytes;
		offset += send_bytes / element_size;
	}
}

ssize_t recvall(int socket, void *buffer, size_t length, size_t element_size, struct sockaddr *address, socklen_t *address_len){
	ssize_t recv_bytes = 0, total = 0;
	int offset = 0;
	
	while(1){
		recv_bytes = recvfrom(socket, buffer+offset, (length-offset)*element_size, 0, address, address_len);

		if(recv_bytes == -1) return -1;
		if(recv_bytes + total >= length*element_size) return recv_bytes + total;
		total += recv_bytes;
		offset += recv_bytes / element_size;
		if(offset >= length) return total;
	}
}

