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
	unregister_chrdev(device_file_major_number, DEVICE_NAME);
	printk(KERN_NOTICE "Fib Driver device file has been unregistered.");
}

long fib_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct FibRequest request;
	int i;
	int* temp_mem;
	
	copy_from_user(&request, (void*)ioctl_param, sizeof(struct FibRequest));

	printk(KERN_NOTICE "Recieved %d", request.num);
	temp_mem = kmalloc(request.num * sizeof(int), GFP_KERNEL);

	printk(KERN_NOTICE "Calculating %d", 0);
	temp_mem[0] = 1;
	if (request.num > 1)
	{
		printk(KERN_NOTICE "Calculating %d", 1);
		temp_mem[1] = 1;
	}

	for (i = 2; i < request.num; ++i)
	{
		printk(KERN_NOTICE "Calculating %d", i);
		temp_mem[i] = temp_mem[i - 1] + temp_mem[i - 2];
	}

	copy_to_user(request.mem, temp_mem, request.num * sizeof(int));
	kfree(temp_mem);
	return 0;
}