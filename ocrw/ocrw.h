#ifndef OCRW_DEVICE
#define OCRW_DEVICE

int __init init_mod(void);
void __exit cleanup_mod(void);

// File operations
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "ocrw"      // Name as it appears in /proc/devices

// Global variables are declared as static, so are global within the file.
static int major;               // Major number assigned to our device driver
static int device_in_use = 0;   // Prevent multiple access to the device

struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcus");
MODULE_DESCRIPTION("Example of open, close, read write");

#endif // OCRW_DEVICE
