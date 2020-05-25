#include "piddriver.h"

int pid_init(void)
{
	int result;

	printk(KERN_NOTICE "Pid Driver is going to be initialized...");
	result = register_chrdev( 0, DEVICE_NAME, &pid_fops);
	
	if (result < 0)
	{
		printk(KERN_WARNING "Cannot initialize Pid Driver. Error Code: %i", result);
		return result;
	}

	device_file_major_number = result;
	printk(KERN_NOTICE "Pid Driver has registered character device with major number %i", device_file_major_number);
	
	return 0;
}
   
void pid_exit(void)
{
	unregister_chrdev(device_file_major_number, DEVICE_NAME);
	printk(KERN_NOTICE "Pid Driver device file has been unregistered.");
}

long pid_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct PidRequest request;
	struct task_struct* tsk;
	copy_from_user(&request, (void*)ioctl_param, sizeof(struct PidRequest));

	printk(KERN_NOTICE "Recieved %d", request.pid);
	tsk = pid_task(find_vpid(request.pid), PIDTYPE_PID);
	printk(KERN_NOTICE "Process name: %s", tsk->comm);
	printk(KERN_NOTICE "Recieved %d", request.pid);
	
	

	//copy_to_user(request.mem, temp_mem, request.pid * sizeof(int));
	//kfree(temp_mem);
	return 0;
}