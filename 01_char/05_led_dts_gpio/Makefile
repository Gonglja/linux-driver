export ARCH=arm
export CROSS_COMPILE=/opt/s5pv210/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-

KERN_DIR=/home/u/workspace/os/linux
OUT_DIR=/home/u/workspace/os/output

all: app
	make -C $(KERN_DIR) M=`pwd` O=$(OUT_DIR) modules

app:
	$(CROSS_COMPILE)gcc -o led_test led_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` O=$(OUT_DIR) modules clean
	rm -rf modules.order
	rm -rf *_test
	
	
obj-m 	+= led.o
