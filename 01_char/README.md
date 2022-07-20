# README

驱动参考[【正点原子】I.MX6U嵌入式Linux驱动开发指南V1.6.pdf](../docs/【正点原子】I.MX6U嵌入式Linux驱动开发指南V1.6.pdf)



|路径|平台|完成？|备注|
|-|-|-|-|
|01_helloworld|arm|:heavy_check_mark:|arm下完成helloworld，如果要x86下用则需将编译链修改即可|
|02_hello_drv|x86|:heavy_check_mark:|真正意义上的第一个字符驱动，用户与系统交互数据并打印|
|02_hello_drv_arm|arm|:heavy_check_mark:|移植到arm侧，功能保持不变|
|03_led|arm|:heavy_check_mark:|添加led灯，包括物理地址到虚拟地址的映射，file_operations 文件操作接口的学习|
|04_led_device-tree|arm|:heavy_check_mark:|在03基础上添加 通过设备树去控制led，读取设备树中的内容，并完成对led的控制|
|05_led_dts_gpio|arm|:heavy_check_mark:|在驱动中添加gpio子系统|
|06_led_dts_pinctrl_platform|arm|:heavy_check_mark:|添加pinctrl子系统，其目的是配置io的复用和电气特性；由于其在platfrom下会自动初始化，我么们只需要配置好设备树就可以了，遂在驱动外层 包裹 platform。|
|07_buzzer_dts_pinctrl_platform|arm|:heavy_check_mark:|上述都是基于led的，修改为 板子上的蜂鸣器。|
|08_led_atomic|arm|:heavy_check_mark:| 添加原子变量，当设备被占用时，再次打开会报错<br> insmod led.ko后，通过命令打开./led_test -w 1 & 之后再次使用该命令打开，则会报错|
|09_led_spinlock|arm|:heavy_check_mark:|使用自旋锁，保护一个设备状态变量，当设备打打开时，检测设备状态，如果被打开了，则解锁退出，否则更新设备状态，解锁|
|10_led_semaphore|arm|:heavy_check_mark:|使用信号量，当打开设备时，获取信号量，如果信号量被使用，进程休眠等待信号量释放被唤醒，信号未使用，则直接打开设备，关闭设备时，释放信号量|
|11_led_mutex|arm|:heavy_check_mark:|与10信号量现象一致|
|12_key_input|arm|:heavy_check_mark:|首先配置要捕获哪个按键，接着直接读，当按键按下时，会返回当前按键的值|
|13_led_timer|arm|:heavy_check_mark:|加载驱动后，默认开启定时器；后面通过`./led_test` app可以去配置该定时器（打开、关闭、配置周期）|
|14_key_irq|arm|:heavy_check_mark:| 加载驱动后，默认开启定时器；后面通过按按键key0发出中断，被cpu捕获，调用中断处理函数；中断处理中则是打开定时器，超时后执行回调函数，即打印消息; 通过应用层代码调用read，返回按键值。|
|15_key_blockio|arm|:heavy_check_mark:| 与实验14一致，不过 14 cpu占用百分百，这个使用了`wait_queue_head_t`技术，cpu占用直线下降。|


