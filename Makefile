obj-m += fibdriver.o
all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
load:
		sudo insmod fibdriver.ko
unload:
		sudo rmmod fibdriver