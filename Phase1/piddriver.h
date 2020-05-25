#ifndef PID_DRIVER_H_
#define PID_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fdtable.h>
#include <linux/dcache.h>

#include "PidRequest.h"
#include "PidResponse.h"

const char* DEVICE_NAME = "piddriver";
static int device_file_major_number = 0;
static int pid_init(void);
static void pid_exit(void);
static long pid_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

static struct file_operations pid_fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = pid_ioctl
};

module_init(pid_init);
module_exit(pid_exit);
MODULE_LICENSE("GPL");

#endif