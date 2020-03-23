#include "fibdriver.h"

int fib_init(void)
{
	int result;

	printk(KERN_NOTICE "Fib Driver is going to be initialized...");
	result = register_chrdev( 0, DEVICE_NAME, &fib_fops);
	
	if (result < 0)
	{
		printk(KERN_WARNING "Cannot initialize Fib Driver. Error Code: %i", result);
		return result;
	}

	device_file_major_number = result;
	printk(KERN_NOTICE "Fib Driver has registered character device with major number %i", device_file_major_number);
	
	return 0;
}
   
void fib_exit(void)
{
    if (device_file_major_number != 0)
    {
		printk(KERN_NOTICE "Fib Driver device file has been unregistered.");
        unregister_chrdev(device_file_major_number, DEVICE_NAME);
    }
}

long fib_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct FibRequest* request;
	int i;

	request = ioctl_param;

	printk(KERN_NOTICE "Recieved %d", request->num);

	printk(KERN_NOTICE "Calculating %d", 0);
	request->mem[0] = 1;
	if (request->num > 1)
	{
		printk(KERN_NOTICE "Calculating %d", 1);
		request->mem[1] = 1;
	}

	for (i = 2; i < request->num; ++i)
	{
		printk(KERN_NOTICE "Calculating %d", i);
		request->mem[i] = request->mem[i - 1] + request->mem[i - 2];
	}

	return 0;
}