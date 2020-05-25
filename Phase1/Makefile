obj-m += piddriver.o
device_file = "/dev/piddriver"
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./pidtest
load:
	-@sudo rmmod piddriver 2>/dev/null || true
	-@sudo rm -f $(device_file) 2>/dev/null || true
	sudo insmod piddriver.ko
	sudo mknod $(device_file) c `grep piddriver /proc/devices | cut -d" " -f1` 1
unload:
	-sudo rmmod piddriver
	-sudo rm -f $(device_file)
pid:
	gcc pid.c -o pidtest
	./pidtest
