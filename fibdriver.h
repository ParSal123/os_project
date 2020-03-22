#ifndef FIB_DRIVER_H_
#define FIB_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>

const char* DEVICE_NAME = "fibdriver";
static int device_file_major_number = 0;
static int fib_init(void);
static void fib_exit(void);
static long fib_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

static struct file_operations fib_fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = fib_ioctl
};

module_init(fib_init);
module_exit(fib_exit);

#endif