#include "blpdriver.h"


int blp_init(void)
{
	int result;
	unsigned int l;
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
	printk(KERN_NOTICE "Boro binam");
    syscall_table = (void *)kallsyms_lookup_name("sys_call_table"); 
    if(syscall_table == NULL)  
    {      
        printk(KERN_ERR"Syscall table is not found\n");   
        return ; 
    } 
    printk("Syscall table found: %p\n",syscall_table); 
    pte = lookup_address((long unsigned int)syscall_table,&l);  
    pte->pte |= _PAGE_RW;

	unprotect_memory();

    original_open =  syscall_table[__NR_openat];
    syscall_table[__NR_openat] = blp_open;
	protect_memory();

	printk(KERN_NOTICE "Open Syscall has been changed!", device_file_major_number);

	return 0;
}
   
void blp_exit(void)
{

	int result;
	unsigned int l;
	unsigned long cr0;
	unsigned long ptr;
    unsigned long *p;
	pte_t *pte;     

	syscall_table = (void *)kallsyms_lookup_name("sys_call_table"); 
    if(syscall_table == NULL)
    {      
        printk(KERN_ERR"Syscall table is not found\n");   
        return; 
    } 
    printk("Syscall table found: %p\n",syscall_table); 
    pte = lookup_address((long unsigned int)syscall_table,&l);  
    pte->pte |= _PAGE_RW;

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
	printk(KERN_NOTICE "SALAM DADASH %s", filename);
	return original_open(dirfd, filename, flags, mode);
}