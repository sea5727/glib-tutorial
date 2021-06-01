# event-loop
this is sample code that drawing console using timeout_source in main_thread and thread1, thread2


# async_queue
비동기 큐를 사용해 스레드간 데이터를 전송 할 수 있다.   
prefix : g_async_queue_

메소드는 직관적이며, 함수명에 unlock 이라고 붙은 경우가 종종 있는데, 소스코드를 확인하면 mutex를 걸어주냐, 안걸어주냐의 차이이다. 


# condition variable

pthread_cond 관련 함수들과 기능이 같다.
wait 함수 호출시 lock을 해제하며, signal을 받아 wait에서 깨어난다면, 다시 lock을 점유하게 된다.
