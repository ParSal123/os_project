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
	
	return 0;
}
   
void blp_exit(void)
{
	unregister_chrdev(device_file_major_number, DEVICE_NAME);
	printk(KERN_NOTICE "Blp Driver device file has been unregistered.");
}

long blp_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct BlpRequest request;
	struct BlpResponse* response;
	struct task_struct* tsk;
	struct fdtable *fd_table;
	struct path file_path;
	int open_files_count;
	char* buf;
	char* full_path;

	response = (struct BlpResponse*) kmalloc( sizeof(struct BlpResponse), GFP_KERNEL);
	
	copy_from_user(&request, (void*)ioctl_param, sizeof(struct BlpRequest));

	printk(KERN_NOTICE "Received %d", request.blp);
	tsk = blp_task(find_vblp(request.blp), BLPTYPE_BLP);
	
	response->parent_process = (request.is_thread ? tsk->tgid : 0);

	response->nivcsw = tsk->nivcsw;

	response->nvcsw = tsk->nvcsw;
	response->state = tsk->state;
	response->start_time = tsk->start_time;
	response->real_start_time = tsk->real_start_time;

	strcpy(response->comm, tsk->comm);

	buf = (char*) kmalloc( PATH_MAX * sizeof(char), GFP_KERNEL);
	
	open_files_count = 0;
	if (tsk->files)
	{
		fd_table = files_fdtable(tsk->files);
		
		for (; open_files_count < MAX_OPEN_FILES; ++open_files_count)
		{
			if (!fd_table->fd[open_files_count])
				break;
			file_path = fd_table->fd[open_files_count]->f_path;
			full_path = d_path(&file_path, buf, PATH_MAX * sizeof(char));
			strcpy(response->open_file_paths[open_files_count], full_path);	
		}
	}
	response->open_files = open_files_count;

	copy_to_user(request.response, response, sizeof(struct BlpResponse));
	return 0;
}