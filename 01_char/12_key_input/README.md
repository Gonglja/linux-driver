目前插入、卸载驱动正常。设备树可正常识别出来
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