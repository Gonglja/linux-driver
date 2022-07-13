
打开第一个进程，设置将所有的灯关闭，之后后台运行，此时线程会等待10s，10s后关闭句柄
打开第二个进程，设置将所有的灯打开，之后该进程会被休眠，直到上一个进程的句柄关闭后，唤醒该线程，该线程会继续执行，将灯打开。
```shell
# ./led_test -w ff &
# open file /dev/led ok
status:255
[ 2715.525911][  T153] kernel recvdata size:1 
[ 2715.525991][  T153] kernel recvdata size:1 ff

# 
# ./led_test -w 0 &
# open file /dev/led ok
status:0
[ 2725.528680][  T154] kernel recvdata size:1 
[ 2725.528755][  T154] kernel recvdata size:1 0

[1]-  Done                       ./led_test -w ff
```