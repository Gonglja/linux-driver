# README



|路径|平台|完成？|备注|
|-|-|-|-|
|01_helloworld|x86|:heavy_check_mark:|ubuntu下完成helloworld|
|02_hello_drv|x86|:heavy_check_mark:|真正意义上的第一个字符驱动，用户与系统交互数据并打印|
|02_hello_drv_arm|arm|:heavy_check_mark:|移植到arm侧，功能保持不变|
|03_led|arm|:heavy_check_mark:|添加led灯，包括物理地址到虚拟地址的映射，file_operations 文件操作接口的学习|
|04_led_device-tree|arm|:heavy_check_mark:|在03基础上添加 通过设备树去控制led，读取设备树中的内容，并完成对led的控制|
|05_led_dts_gpio|arm|:heavy_check_mark:|在驱动中添加gpio子系统|
|06_led_dts_pinctrl_platform|arm|:heavy_check_mark:|添加pinctrl子系统，其目的是配置io的复用和电气特性；由于其在platfrom下会自动初始化，我么们只需要配置好设备树就可以了，遂在驱动外层 包裹 platform。|
|07_buzzer_dts_pinctrl_platform|arm|:heavy_check_mark:|上述都是基于led的，修改为 板子上的蜂鸣器。|
|08||||
|||||
|||||
|||||
|||||
