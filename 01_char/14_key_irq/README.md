
设备树改变详见 
https://github.com/Gonglja/linux/commit/ab11bf7f145b305e191abb08d1a051c591fdac7c


```c
/{ 
    keys: keys {
		compatible = "smart210,keys";
		pinctrl-name = "default";
		pinctrl-0 = <&pinctrl_keys>;
		status="okay";

		interrupt-parent = <&gph2>;
		interrupts = <0 IRQ_TYPE_EDGE_BOTH>;
		...
	};
};

```

主要添加了
```
		interrupt-parent = <&gph2>;
		interrupts = <0 IRQ_TYPE_EDGE_BOTH>;
```
- interrupt-parent 表示中断的父节点是`gph2`
- interrupts 
	- 第一个值`0`为哪个`gph2`下的第`0`个 io，也即`gph2-0`
	- 第二个值`IRQ_TYPE_EDGE_BOTH`指的是中断触发方式，在这里设置为双边沿，具体可查看`include/dt-bindings/interrupt-controller/irq.h`


~~**实验结果**如下~~

```shell
# ls
driver
# cat /proc/interrupts 
           CPU0       
 34:          0       VIC  18 Edge      fa200000.dma-controller
 35:          0       VIC  19 Edge      e0900000.dma-controller
 36:          0       VIC  20 Edge      e0a00000.dma-controller
 39:       1394       VIC  23 Edge      samsung_time_irq
 46:          0       VIC  30 Edge      e0200000.pinctrl
 58:        205       VIC  10 Edge      e2900000.serial
 62:          0       VIC  14 Edge      e1800000.i2c
 72:          1       VIC  24 Edge      ec000000.hsotg, dwc2_hsotg:usb1
144:       3306      gph0   7 Level     eth0
Err:          0
# cd driver/
# insmod key.ko 
[   27.601386][  T108] key: loading out-of-tree module taints kernel.
[   27.602270][  T108] key_init
[   27.603035][  T108] keys node has been found!
[   27.603128][  T108] gpio-pin num[0] = 221 
[   27.603304][  T108] keydev.keyirq[i].irqnum 0x91
[   27.603391][  T108] gpio-pin num[1] = 222 
[   27.605697][  T108] keydev.keyirq[i].irqnum 0x0
[   27.612351][  T108] gpio-pin num[2] = 223 
[   27.617755][  T108] keydev.keyirq[i].irqnum 0x0
[   27.619059][  T108] gpio-pin num[3] = 224 
[   27.623202][  T108] keydev.keyirq[i].irqnum 0x0
[   27.628679][  T108] gpio-pin num[4] = 229 
[   27.631967][  T108] keydev.keyirq[i].irqnum 0x0
[   27.637199][  T108] gpio-pin num[5] = 230 
[   27.640718][  T108] keydev.keyirq[i].irqnum 0x0
[   27.645300][  T108] gpio-pin num[6] = 231 
[   27.650244][  T108] keydev.keyirq[i].irqnum 0x0
[   27.654064][  T108] gpio-pin num[7] = 232 
[   27.658954][  T108] keydev.keyirq[i].irqnum 0x0
[   27.662855][  T108] irq 0 request failed!
[   27.667496][  T108] irq 0 request failed!
[   27.670933][  T108] irq 0 request failed!
[   27.675003][  T108] irq 0 request failed!
[   27.679792][  T108] irq 0 request failed!
[   27.683152][  T108] irq 0 request failed!
[   27.687799][  T108] irq 0 request failed!
[   27.691308][  T108] major:251
# [   27.896279][    C0] num:221 key_val:1

# 
# 
# [   33.376279][    C0] num:221 key_val:0
[   33.496276][    C0] num:221 key_val:1
[   35.636277][    C0] num:221 key_val:0
[   36.806276][    C0] num:221 key_val:1
[   37.916284][    C0] num:221 key_val:0
[   39.806275][    C0] num:221 key_val:1
[   40.466281][    C0] num:221 key_val:0
[   40.596274][    C0] num:221 key_val:1
[   40.726286][    C0] num:221 key_val:0
[   40.846287][    C0] num:221 key_val:1
[   40.956293][    C0] num:221 key_val:0
[   41.046287][    C0] num:221 key_val:1
[   41.166274][    C0] num:221 key_val:0
[   41.256286][    C0] num:221 key_val:1

# 
# cat /proc/interrupts 
           CPU0       
 34:          0       VIC  18 Edge      fa200000.dma-controller
 35:          0       VIC  19 Edge      e0900000.dma-controller
 36:          0       VIC  20 Edge      e0a00000.dma-controller
 39:       1655       VIC  23 Edge      samsung_time_irq
 46:          0       VIC  30 Edge      e0200000.pinctrl
 58:        478       VIC  10 Edge      e2900000.serial
 62:          0       VIC  14 Edge      e1800000.i2c
 72:          1       VIC  24 Edge      ec000000.hsotg, dwc2_hsotg:usb1
144:       3547      gph0   7 Level     eth0
145:         15      gph2   0 Edge      KEY0
Err:          0
# rmmod key
[   55.857130][  T110] key_exit
# 

```


**实验结果如下**，首先加载该模块，当按键按下后，触发内核中断，在中断任务中打开定时器，定时器时间到后执行定时器回调任务，更新按键值，更新按键状态；当App通过read调用内核中read函数时，返回当前按键值，App将之输出到终端。
```shell
# ls
driver
# cd driver/
# ls
buzzer.ko       hello_drv.ko    key.ko          led.ko
buzzer_test     hello_drv_test  key_test        led_test
# insmod key.ko 
[ 2807.966999][  T111] key: loading out-of-tree module taints kernel.
[ 2807.967834][  T111] key_init
[ 2807.968626][  T111] keys node has been found!
[ 2807.968717][  T111] gpio-pin num[0] = 221 
[ 2807.968896][  T111] keydev.keyirq[i].irqnum 0x91
[ 2807.968986][  T111] gpio-pin num[1] = 222 
[ 2807.971312][  T111] keydev.keyirq[i].irqnum 0x0
[ 2807.980023][  T111] gpio-pin num[2] = 223 
[ 2807.980120][  T111] keydev.keyirq[i].irqnum 0x0
[ 2807.984659][  T111] gpio-pin num[3] = 224 
[ 2807.989713][  T111] keydev.keyirq[i].irqnum 0x0
[ 2807.993423][  T111] gpio-pin num[4] = 229 
[ 2807.998217][  T111] keydev.keyirq[i].irqnum 0x0
[ 2808.002175][  T111] gpio-pin num[5] = 230 
[ 2808.006962][  T111] keydev.keyirq[i].irqnum 0x0
[ 2808.010926][  T111] gpio-pin num[6] = 231 
[ 2808.015937][  T111] keydev.keyirq[i].irqnum 0x0
[ 2808.019680][  T111] gpio-pin num[7] = 232 
[ 2808.023825][  T111] keydev.keyirq[i].irqnum 0x0
[ 2808.029268][  T111] irq 0 request failed!
[ 2808.032475][  T111] irq 0 request failed!
[ 2808.037148][  T111] irq 0 request failed!
[ 2808.040621][  T111] irq 0 request failed!
[ 2808.044690][  T111] irq 0 request failed!
[ 2808.049470][  T111] irq 0 request failed!
[ 2808.052839][  T111] irq 0 request failed!
[ 2808.057467][  T111] major:251
# [ 2808.264785][    C0] num:221 key_val:0

# ./key_test 
open /dev/key success.
[ 2815.470608][  T113] kernel senddata ok!
[ 2825.804772][    C0] num:221 key_val:1
[ 2825.804852][  T113] kernel senddata ok!
key value = 0x1
[ 2825.984769][    C0] num:221 key_val:0
[ 2825.984850][  T113] kernel senddata ok!
[ 2829.194772][    C0] num:221 key_val:1
[ 2829.194853][  T113] kernel senddata ok!
key value = 0x1
[ 2829.374771][    C0] num:221 key_val:0
[ 2829.374853][  T113] kernel senddata ok!
[ 2831.084771][    C0] num:221 key_val:1
[ 2831.084853][  T113] kernel senddata ok!
key value = 0x1
[ 2831.924771][    C0] num:221 key_val:0
[ 2831.924850][  T113] kernel senddata ok!
[ 2832.684768][    C0] num:221 key_val:1
[ 2832.684847][  T113] kernel senddata ok!
key value = 0x1
[ 2832.894778][    C0] num:221 key_val:0
[ 2832.894857][  T113] kernel senddata ok!
[ 2833.814771][    C0] num:221 key_val:1
[ 2833.814854][  T113] kernel senddata ok!
key value = 0x1
[ 2834.424768][    C0] num:221 key_val:0
[ 2834.424850][  T113] kernel senddata ok!
[ 2835.104769][    C0] num:221 key_val:1
[ 2835.104851][  T113] kernel senddata ok!
key value = 0x1
[ 2835.244768][    C0] num:221 key_val:0
[ 2835.244850][  T113] kernel senddata ok!
[ 2835.854771][    C0] num:221 key_val:1
[ 2835.854850][  T113] kernel senddata ok!
key value = 0x1
[ 2835.964776][    C0] num:221 key_val:0
[ 2835.964856][  T113] kernel senddata ok!
[ 2836.444769][    C0] num:221 key_val:1
[ 2836.444848][  T113] kernel senddata ok!
key value = 0x1
[ 2836.534771][    C0] num:221 key_val:0
[ 2836.534852][  T113] kernel senddata ok!
[ 2837.054772][    C0] num:221 key_val:1
[ 2837.054850][  T113] kernel senddata ok!
key value = 0x1
[ 2837.144771][    C0] num:221 key_val:0
[ 2837.144857][  T113] kernel senddata ok!
[ 2837.564769][    C0] num:221 key_val:1
[ 2837.564847][  T113] kernel senddata ok!
key value = 0x1
[ 2837.664769][    C0] num:221 key_val:0
[ 2837.664849][  T113] kernel senddata ok!
[ 2837.984770][    C0] num:221 key_val:1
[ 2837.984848][  T113] kernel senddata ok!
key value = 0x1
[ 2838.094773][    C0] num:221 key_val:0
[ 2838.094854][  T113] kernel senddata ok!
^C
# rmmod key
[ 2849.650562][  T114] key_exit
# 


```

参考 https://to-run-away.blog.csdn.net/article/details/88080880?spm=1001.2014.3001.5502
