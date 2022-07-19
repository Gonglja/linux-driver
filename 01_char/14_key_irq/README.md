
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


**实验结果**如下

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


参考 https://to-run-away.blog.csdn.net/article/details/88080880?spm=1001.2014.3001.5502
