all:clean hello

hello:
	arm-none-eabi-gcc --specs=rdimon.specs -lgcc -lc -lm -lrdimon hello.c -o hello

clean:
	rm -rf hello
