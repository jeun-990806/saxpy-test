from socket import *
import sys
import time

import pickle

from tools import *

def saxpy(n, a, x, y):
    for i in range(n):
        y[i] = a*x[i] + y[i]

def get_average(arr, n):
    return sum(arr)/n

def main():
    n = int(sys.argv[3])
    a = float(sys.argv[4])
    executions = int(sys.argv[5])
    
    x = [0.01 for _ in range(n)]
    y = [0.001 for _ in range(n)]
    
    records = [{'connection': 0, 'transmission': 0, 'reception': 0} for _ in range(executions)]

    server_addr = sys.argv[1]
    server_port = int(sys.argv[2])
    
    for i in range(executions):
        print('Start saxpy... (' + str(i+1) + '/' + str(executions) + ')')
        server_socket = socket(AF_INET, SOCK_STREAM)
        
        time_marker = time.time()
        server_socket.connect((server_addr, server_port))
        records[i]['connection'] = time.time() - time_marker
        print('Connection:', records[i]['connection'])

        time_marker = time.time()
        
        server_socket.sendall(pickle.dumps([n, a, x, y]))
        server_socket.shutdown(SHUT_WR)
        records[i]['transmission'] = time.time() - time_marker
        print('Transmission:', records[i]['transmission'])
    
        time_marker = time.time()
        data = server_socket.recv(4096)
        while len(data) > 0:
            tmp = server_socket.recv(4096)
            if len(tmp) == 0:
                break
            else:
                data += tmp
        result = pickle.loads(data)
        records[i]['reception'] = time.time() - time_marker
        print('Reception:', records[i]['reception'])
        print('y[] = {' + str(result[0]) + ', ..., ' + str(result[n-1]) + '}')
        server_socket.close()

    print('======================  Summary')
    print('Averages:')
    print('\tConnection time:', get_average([record['connection'] for record in records], executions))
    print('\tTransmission time:', get_average([record['transmission'] for record in records], executions))
    print('\tReception time:', get_average([record['reception'] for record in records], executions))
    print('===============================')
    

main()
