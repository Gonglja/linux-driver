export ARCH=arm
export CROSS_COMPILE=/opt/s5pv210/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-

KERN_DIR=/home/u/workspace/os/linux
OUT_DIR=/home/u/workspace/os/output

all: $(subst .c,,$(wildcard *_test.c))
	make -C $(KERN_DIR) M=`pwd` O=$(OUT_DIR) modules

%_test: %_test.c
	$(CROSS_COMPILE)gcc -o $@ $<

clean:
	make -C $(KERN_DIR) M=`pwd` O=$(OUT_DIR) modules clean
	rm -rf modules.order
	rm -rf *_test
	
	
obj-m 	+= buzzer.o
