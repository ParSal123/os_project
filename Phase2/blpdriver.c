#include "blpdriver.h"

int blp_init(void)
{
	int result;
	unsigned int level;
	unsigned long cr0;
	unsigned long ptr;
    unsigned long *p;
	pte_t *pte;     

    
	printk(KERN_NOTICE "Blp Driver is going to be initialized...");
	result = register_chrdev( 0, DEVICE_NAME, &blp_fops);
	
	if (result < 0)
	{
		printk(KERN_WARNING "Cannot initialize Blp Driver. Error Code: %i", result);
		return result;
	}

	device_file_major_number = result;
	printk(KERN_NOTICE "Blp Driver has registered character device with major number %i", device_file_major_number);
    syscall_table = (void*)kallsyms_lookup_name("sys_call_table"); 
    if(!syscall_table)  
    {      
        printk(KERN_ERR "Syscall table could not be found!");
        return; 
    } 
    printk("Syscall table found: %p\n",syscall_table); 
    pte = lookup_address((long unsigned int)syscall_table, &level);  
    pte->pte |= _PAGE_RW;

	unprotect_memory();
    original_open =  syscall_table[__NR_openat];
    syscall_table[__NR_openat] = blp_open;
	getuid_call = syscall_table[__NR_getuid];
	protect_memory();

	printk(KERN_NOTICE "Open Syscall has been changed!", device_file_major_number);

	return 0;
}
   
void blp_exit(void)
{
	if (syscall_table[__NR_openat] != blp_open)
		printk(KERN_ALERT "The open syscall has been changed by another module!");

	syscall_table[__NR_openat] = original_open;

	unregister_chrdev(device_file_major_number, DEVICE_NAME);
	printk(KERN_NOTICE "Blp Driver device file has been unregistered.");
}

long blp_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	return 0;
}

asmlinkage int blp_open(int dirfd, const char* filename, int flags, int mode)
{
	int i, file_index = -1, user_index, file_level, user_level;
	int total_files = sizeof(filenames)/sizeof(char*);
	int total_users = sizeof(users)/sizeof(int);
	int current_user = getuid_call();
	for (i = 0; i < total_files; i++)
		if (strcmp(filename, filenames[i])==0)
		{
			file_index = i;
			break;
		}
	if (file_index == -1)
		file_level = 0;
	else file_level = file_sl[file_index];
	for (i = 0; i < total_users; i++)
		if (users[i] == current_user)
		{
			user_index = i;
			break;
		}
	if (user_index == -1)
		user_level = 0;
	else user_level = user_sl[user_index];

	if ((flags == O_RDONLY || flags == O_RDWR) && user_level < file_level)
	{
		printk(KERN_ERR "Could not read file, because the user SL is less than the file SL.");
		return -1;
	}
	if ((flags == O_WRONLY || flags == O_RDWR) && user_level > file_level)
	{
		printk(KERN_ERR "Could not write file, because the user SL is more than the file SL.");
		return -1;
	}
	return original_open(dirfd, filename, flags, mode);
}
