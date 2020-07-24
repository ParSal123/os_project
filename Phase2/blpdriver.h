#ifndef BLP_DRIVER_H_
#define BLP_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fdtable.h>
#include <linux/dcache.h>
#include <linux/semaphore.h>
#include "BlpRequest.h"
#include "BlpResponse.h"

const char* DEVICE_NAME = "blpdriver";
static int device_file_major_number = 0;
static int blp_init(void);
static void blp_exit(void);
static long blp_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
#define CR0_WP 0x00010000
asmlinkage int (*original_open)(const char*, int, int);
asmlinkage int blp_open(const char* filename, int flags, int mode);

void **syscall_table = (void *)0xffffffffa4800260;

static struct file_operations blp_fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = blp_ioctl
};

module_init(blp_init);
module_exit(blp_exit);
MODULE_LICENSE("GPL");

#endif