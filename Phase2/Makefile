obj-m += blpdriver.o
device_file = "/dev/blpdriver"
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./blptest
load:
	-@sudo rmmod blpdriver 2>/dev/null || true
	-@sudo rm -f $(device_file) 2>/dev/null || true
	sudo insmod blpdriver.ko
	sudo mknod $(device_file) c `grep blpdriver /proc/devices | cut -d" " -f1` 1
unload:
	-sudo rmmod blpdriver
	-sudo rm -f $(device_file)
blp:
	gcc blp.c -o blptest
