
**现象与10信号量实验一致**
打开第一个进程，设置将所有的灯关闭，之后后台运行，此时线程会等待10s，10s后关闭句柄
打开第二个进程，设置将所有的灯打开，之后该进程会被休眠，直到上一个进程的句柄关闭后，唤醒该线程，该线程会继续执行，将灯打开。
```shell
# ./led_test -w 0 &
# open file /dev/led ok
status:0
[ 3710.108890][  T163] kernel recvdata size:1 
[ 3710.108971][  T163] kernel recvdata size:1 0

# 
# 
# ./led_test -w ff &
# 
# 
# 
# 
# open file /dev/led ok
status:255
[ 3720.111600][  T164] kernel recvdata size:1 
[ 3720.111676][  T164] kernel recvdata size:1 ff

[1]-  Done                       ./led_test -w 0
# 
# 
[2]+  Done                       ./led_test -w ff

```