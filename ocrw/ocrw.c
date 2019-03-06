#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // KERN_INFO
#include <linux/init.h>      // __init and __exit macros
#include <linux/fs.h>        // file_operations
#include "ocrw.h"

int __init init_mod(void)
{
        major = register_chrdev(0, DEVICE_NAME, &fops);
        if (major < 0) {
                printk (KERN_INFO "ocrw failed: %d\n", major);
                return major;
        }
        printk(KERN_INFO "Load ocrw, major number: %d.\n", major);
        printk("Run: 'mknod /dev/ocrw c %d 0'.\n", major);

        return 0;
}

void __exit cleanup_mod(void)
{
        printk(KERN_INFO "Cleaning up ocrw.\n");
        unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
        printk (KERN_INFO "Open device ocrw.\n");
        if (device_in_use) return -EBUSY;
        device_in_use++; 

        return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
        printk (KERN_INFO "Close device ocrw.\n");
        device_in_use--;

        return 0;
}

static ssize_t device_read(struct file *filp,
                char *buffer,
                size_t length,
                loff_t *offset)
{
        printk (KERN_INFO "Reading from ocrw.\n");
        return -EINVAL;
        //return bytes_read;
}

static ssize_t device_write(struct file *filp,
                const char *buff,
                size_t len,
                loff_t *off)
{
        printk (KERN_INFO "Writing to ocrw.\n");
        return -EINVAL;
}

module_init(init_mod);
module_exit(cleanup_mod);
