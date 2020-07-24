#include "blpdriver.h"

int blp_init(void)
{
	int result;

	printk(KERN_NOTICE "Blp Driver is going to be initialized...");
	result = register_chrdev( 0, DEVICE_NAME, &blp_fops);
	
	if (result < 0)
	{
		printk(KERN_WARNING "Cannot initialize Blp Driver. Error Code: %i", result);
		return result;
	}

	device_file_major_number = result;
	printk(KERN_NOTICE "Blp Driver has registered character device with major number %i", device_file_major_number);

	original_open = syscall_table[__NR_open];
	syscall_table[__NR_open] = blp_open;

	printk(KERN_NOTICE "Open Syscall has been changed!", device_file_major_number);

	return 0;
}
   
void blp_exit(void)
{
	if (syscall_table[__NR_open] != blp_open)
		printk(KERN_ALERT "The open syscall has been changed by another module!");

	syscall_table[__NR_open] = original_open;

	unregister_chrdev(device_file_major_number, DEVICE_NAME);
	printk(KERN_NOTICE "Blp Driver device file has been unregistered.");
}

long blp_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	return 0;
}

asmlinkage int blp_open(const char* filename, int flags, int mode)
{
	printk(KERN_NOTICE "SALAM DADASH");
	return original_open(filename, flags, mode);
}