# udp-echo-server-singlethread

## g_inet_socket_address_new(address, port)
소켓 주소(address + port) 생성자
## g_inet_address_new_any(family)
address 생성자
## g_socket_new(family, stream/datagram, tcp/udp, &err)
socket 생성자
## g_socket_get_fd(socket)
get fd from socket
## g_io_channel_unix_new(fd)
io_channel 생성자
## g_io_channel_read_chars(iochannel, buf, sizeofbuf, &readlen, &err)
iochannel로부터 문자열 읽기
## g_io_add_watch(iochannel, event, callback, param)
like epoll add : 이벤트 등록
## g_idle_add(callback, param)
즉시 실행 이벤트 등록
iochannel을 input으로 받지 않는것으로보아 main thread에서만 등록하는것 같다.
## g_timeout_add(interval_time, callback, parma)
타이머 이벤트 등록
iochannel을 input으로 받지 않는것으로보아 main thread에서만 등록하는것 같다.


# udp-send-recv-other-thread

`sender thread` udp send data to `receiver thread` per 1 seconds infinitely until 10 times then close socket

`receiver thread` receive from `sender thread` and print console log

`main thread` is sleeping

binary data를 수신받기 위해서 g_io_channel을 encoding 해제해야한다. (defualt UTF-8)
``` c
g_io_channel_set_encoding(channel, NULL, &error); // if you want to receive a binary data
```


# self test
```
$ nc -u localhost port 
```


