from socket import *
import sys

import pickle

def saxpy(n, a, x, y):
    for i in range(n):
        y[i] = a*x[i] + y[i]


def main():
    port = int(sys.argv[1])

    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('127.0.0.1', port))
    server_socket.listen()

    while(True):
        client_socket, client_addr = server_socket.accept()
        data = client_socket.recv(4096)
        while len(data) > 0:
            tmp = client_socket.recv(4096)
            data += tmp
            if len(tmp) == 0:
                break
        n, a, x, y = pickle.loads(data)
        
        print('Received data: n=' + str(n) + ', a=' + str(a) + ', x[]={' + str(x[0]) + ', ..., ' + str(x[n-1]) + '}, y[]={' + str(y[0]) + ', ..., ' + str(y[n-1]) + '}')
        saxpy(n, a, x, y)

        if client_socket.sendall(pickle.dumps(y)) is not None:
            print('ERROR')
        client_socket.close()

main()
