#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcus");
MODULE_DESCRIPTION("Keyboard interrupt handler module");

static int __init kb_intrpt_init(void)
{
    printk(KERN_INFO "Load kb_intrpt.\n");
    return 0;
}

static void __exit kb_intrpt_cleanup(void)
{
    printk(KERN_INFO "Unload kb_intrpt.\n");
}

module_init(kb_intrpt_init);
module_exit(kb_intrpt_cleanup);
