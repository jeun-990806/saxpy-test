# saxpy-test
---

## C
### Computation offloading
1. 서버 실행
```shell
./saxpy_input_receiver <port> <protocol>
```

2. 클라이언트 실행
```shell
./saxpy_input_sender <host_ip> <port> <n> <a> <execution_times> <protocol>
```

* 예시: UDP로, n=1000, a=2인 saxpy를 10번 오프로딩하는 방법은 다음과 같다.


서버
 ```shell
 ./saxpy_input_receiver 55555 UDP
 ```
클라이언트
 ```shell
 ./saxpy_input_sender 127.0.0.1 55555 1000 2 10 UDP
 ```
