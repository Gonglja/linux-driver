ARCH=x86
CROSS_COMPILE=

KVERSION=$(shell uname -r)
KERN_DIR=/lib/modules/$(KVERSION)/build


all: app
	make -C $(KERN_DIR) M=`pwd` modules



app:
	gcc -o hello_drv_test hello_drv_test.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -rf hello_drv_test
	
	
obj-m 	+= hello_drv.o
