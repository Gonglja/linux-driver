## 更新前

~~目前插入、卸载驱动正常。设备树可正常识别出来~~
设备树如下，通过 of_get_next_child 获取子节点
```c
/{
    keys: keys {
		compatible = "smart210,keys";
		pinctrl-name = "default";
		pinctrl-0 = <&pinctrl_keys>;
		status="okay";
		button1 {
			gpio-pin = <&gph2 0 0>;
		};		
		
		button2 {
			gpio-pin = <&gph2 1 0>;
		};		
		
		button3 {
			gpio-pin = <&gph2 2 0>;
		};		
		
		button4 {
			gpio-pin = <&gph2 3 0>;
		};		
		
		button5 {
			gpio-pin = <&gph3 0 0>;
		};		
		
		button6 {
			gpio-pin = <&gph3 1 0>;
		};		
		
		button7 {
			gpio-pin = <&gph3 2 0>;
		};		
		
		button8 {
			gpio-pin = <&gph3 3 0>;
		};
	};
};


&pinctrl0 {
    ...
    pinctrl_keys: keysgrp {
		samsung,pins = "gph2-0", "gph2-1", "gph2-2", "gph2-3", 
					   "gph3-0", "gph3-1", "gph3-2", "gph3-3";
		samsung,pin-function = <EXYNOS_PIN_FUNC_INPUT>;
		samsung,pin-pud = <S3C64XX_PIN_PULL_UP>;
		samsung,pin-drv = <EXYNOS4_PIN_DRV_LV1>;
	};
}

```


```shell
# insmod  key.ko 
[ 1286.690792][  T119] key_init
[ 1286.691541][  T119] keys node has been found!
[ 1286.691632][  T119] gpio-pin num[0] = 221 
[ 1286.691699][  T119] gpio-pin num[1] = 222 
[ 1286.691757][  T119] gpio-pin num[2] = 223 
[ 1286.691816][  T119] gpio-pin num[3] = 224 
[ 1286.692584][  T119] gpio-pin num[4] = 229 
[ 1286.696743][  T119] gpio-pin num[5] = 230 
[ 1286.705111][  T119] gpio-pin num[6] = 231 
[ 1286.705201][  T119] gpio-pin num[7] = 232 
[ 1286.709890][  T119] major:250
# rmmod key
[ 1288.260512][  T121] key_exit
# 

```



---

## 更新后

通过 `./key_test -w ff `设置8个按键均可被捕获，
此时通过`./key_test -r ` 就可以等待捕获，直到有按键被按下，打印出按键编号，也即是当button1 按下时，打印0x1，当button2按下时，打印0x2 ...

| bits | buttonx |
| ---- | ------- |
| 0    | button1 |
| 1    | button2 |
| 2    | button3 |
| 3    | button4 |
| 4    | button5 |
| 5    | button6 |
| 6    | button7 |
| 7    | button8 |

具体日志看下面

```shell
# 
# ls
buzzer.ko       hello_drv.ko    key.ko          led.ko
buzzer_test     hello_drv_test  key_test        led_test
# insmod key.ko 
[   49.052784][  T109] key: loading out-of-tree module taints kernel.
[   49.053730][  T109] key_init
[   49.054554][  T109] keys node has been found!
[   49.054648][  T109] gpio-pin num[0] = 221 
[   49.054718][  T109] gpio-pin num[1] = 222 
[   49.054780][  T109] gpio-pin num[2] = 223 
[   49.056577][  T109] gpio-pin num[3] = 224 
[   49.062772][  T109] gpio-pin num[4] = 229 
[   49.065408][  T109] gpio-pin num[5] = 230 
[   49.072627][  T109] gpio-pin num[6] = 231 
[   49.073227][  T109] gpio-pin num[7] = 232 
[   49.077400][  T109] major:251
# ./key_test -w ff
open file /dev/key ok
status:255
[   66.365454][  T110] kernel recvdata size:1 
[   66.365533][  T110] kernel recvdata size:1 buf:0xff
# ./key_test -r 
open file /dev/key ok
[   79.272283][  T111] kernel senddata ok!
APP read size: 0
APP read buff: 0x1
# ./key_test -r 
open file /dev/key ok
[   82.443276][  T112] kernel senddata ok!
APP read size: 0
APP read buff: 0x2
# ./key_test -r 
open file /dev/key ok
[   83.986593][  T113] kernel senddata ok!
APP read size: 0
APP read buff: 0x4
# ./key_test -r 
open file /dev/key ok
[   85.502276][  T114] kernel senddata ok!
APP read size: 0
APP read buff: 0x8
# ./key_test -r 
open file /dev/key ok
[   87.175377][  T115] kernel senddata ok!
APP read size: 0
APP read buff: 0x10
# ./key_test -r 
open file /dev/key ok
[   88.528172][  T116] kernel senddata ok!
APP read size: 0
APP read buff: 0x20
# ./key_test -r 
open file /dev/key ok
[   89.975625][  T117] kernel senddata ok!
APP read size: 0
APP read buff: 0x40
# ./key_test -r 
open file /dev/key ok
[   92.662628][  T118] kernel senddata ok!
APP read size: 0
APP read buff: 0x80
# rmmod key
[  106.491140][  T119] key_exit
# 
# 

```
