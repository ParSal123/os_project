#ifndef FIB_DRIVER_H_
#define FIB_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>

static int __init fib_init(void);
static void __exit fib_exit(void);

struct file_operations simple_driver_fops = 
{
    .owner = THIS_MODULE,
};

module_init(fib_init);
module_exit(fib_exit);

#endif