#include <linux/module.h>       // included for all kernel modules
#include <linux/kernel.h>       // KERN_INFO
#include <linux/init.h>         // __init, __exit macros
#include <linux/fs.h>           // file_operations
#include <linux/semaphore.h>    // semaphores, synchronization behaviors
#include <linux/uaccess.h>        // copy_to_user, copy_from_user
#include <linux/cdev.h>         // cdev_init, cdev_all, handles character devices
#include <linux/device.h>       // class_create, class_destroy, 
                                // device_create, device_destrony
#include "ocrw.h"

// Global variables are declared as static, so are global within the file.
static int major;               // Major number assigned to our device driver
static int ret;                 // Used for return values as the kernel stack is small
static struct ocrw_device virtual_device;
static dev_t dev;               // Major number given by kernel
static struct cdev c_dev;       // cdev object
static struct class *c1;
static struct device *dev_ret;

int __init init_mod(void)
{
        // Dynamic allocation to assign our device
        if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_COUNT, DEVICE_NAME)) < 0)
                return ret;

        major = MAJOR(dev);
        printk(KERN_INFO "ocrw: Loaded, major number: %d.\n", major);

        cdev_init(&c_dev, &fops);

        // Add cdev to the kernel
        if ((ret = cdev_add(&c_dev, dev, MINOR_COUNT)) < 0)
                return ret;

        // Create a class struct
        if (IS_ERR(c1 = class_create(THIS_MODULE, "char"))) {
                cdev_del(&c_dev);
                unregister_chrdev_region(dev, MINOR_COUNT);
                return PTR_ERR(c1);
        }

        // Creates a device and registers it with sysfs, A "dev" file will be created.
        if (IS_ERR(dev_ret = device_create(c1, NULL, dev, NULL, DEVICE_NAME))) {
                class_destroy(c1);
                cdev_del(&c_dev);
                unregister_chrdev_region(dev, MINOR_COUNT);
                return PTR_ERR(dev_ret);
        }

        sema_init(&virtual_device.sem, 1);

        return 0;
}

void __exit cleanup_mod(void)
{
        printk(KERN_INFO "ocrw: Cleaning up device.\n");
        device_destroy(c1, dev);
        class_destroy(c1);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_COUNT);
}

static int device_open(struct inode *inode, struct file *file)
{
        if (down_interruptible(&virtual_device.sem) != 0) {
                printk(KERN_ALERT "ocrw: Already in use.");
                return -EBUSY;
        }
        printk(KERN_INFO "ocrw: Opened device.");
        return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
        // Release semaphore
        up(&virtual_device.sem);
        printk(KERN_INFO "ocrw: Close device.\n");
        return 0;
}

static ssize_t device_read(
                struct file *filp,
                char *buffer,
                size_t length,
                loff_t *offset)
{
        // Nothing left to read
        if (*offset >= sizeof(virtual_device.data))
        		return 0;

        // Read more than possible
		if (*offset + length > sizeof(virtual_device.data))
        		length = sizeof(virtual_device.data) - *offset;

        ret = copy_to_user(buffer, virtual_device.data + *offset, length);
        printk(KERN_INFO "ocrw: Reading from device.");
        return ret;
}

static ssize_t device_write(
                struct file *filp,
                const char *buffer,
                size_t length,
                loff_t *offset)
{
        // Nothing left to write
        if (*offset >= sizeof(virtual_device.data))
        		return 0;

        // Write more than possible
		if(*offset + length > sizeof(virtual_device.data))
        		length = sizeof(virtual_device.data) - *offset;

        ret = copy_from_user(virtual_device.data, buffer, length);
        printk(KERN_INFO "ocrw: Writing to device.\n");
        return ret;
}

module_init(init_mod);
module_exit(cleanup_mod);
