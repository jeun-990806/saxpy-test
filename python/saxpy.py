import sys
import time

def saxpy(n, a, x, y):
    for i in range(n):
        y[i] = a*x[i] + y[i]

def main():
    n = int(sys.argv[1])
    a = float(sys.argv[2])

    x = [0.01 for _ in range(n)]
    y = [0.001 for _ in range(n)]
    
    print('Start saxpy...')
    start = time.time()
    saxpy(n, a, x, y)
    end = time.time()
    print('Saxpy is ended. Done in', round(end-start, 7));

main()

