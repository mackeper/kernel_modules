#ifndef OCRW_DEVICE
#define OCRW_DEVICE

#define SUCCESS 0
#define DEVICE_NAME "ocrw"      // Name as it appears in /proc/devices
#define FIRST_MINOR 0
#define MINOR_COUNT 1

int __init init_mod(void);
void __exit cleanup_mod(void);

// File operations
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};

struct ocrw_device {
        char data[100];
        struct semaphore sem;
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcus");
MODULE_DESCRIPTION("Example of open, close, read write");

#endif // OCRW_DEVICE
