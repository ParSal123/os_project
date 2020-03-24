obj-m += fibdriver.o
device_file = "/dev/fibdriver"
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
load:
	-@sudo rmmod fibdriver 2>/dev/null || true
	-@sudo rm -f $(device_file) 2>/dev/null || true
	sudo insmod fibdriver.ko
	sudo mknod $(device_file) c `grep fibdriver /proc/devices | cut -d" " -f1` 1
unload:
	-sudo rmmod fibdriver
	-sudo rm -f $(device_file)
fib:
	gcc fib.c -o fibtest
	./fibtest
