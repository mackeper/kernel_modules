#ifndef KTHREAD_EXAMPLE
#define KTHREAD_EXAMPLE

#define SUCCESS 0
#define DEVICE_NAME "kthread_example"      // Name as it appears in /proc/devices

struct kthread_data {
        int     id;
        int     interval;
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcus");
MODULE_DESCRIPTION("Example of kthreads");

#endif // KTHREAD_EXAMPLE
