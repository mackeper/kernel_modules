#include <linux/module.h>       // included for all kernel modules
#include <linux/kernel.h>       // KERN_INFO
#include <linux/init.h>         // __init, __exit macros
#include <linux/fs.h>           // file_operations
#include <linux/timer.h>        // timer_list, init_timer, add_timer...


#include "timer.h"

static struct timer_list tmr;
static long timer_interval = 1000;

static void timeout_fn(struct timer_list *t)
{
        printk(KERN_INFO "timer: Times up!\n");
        mod_timer(&tmr, jiffies + msecs_to_jiffies(timer_interval));
}

static int __init mod_init(void)
{
        timer_setup(&tmr, timeout_fn, 0);
        mod_timer(&tmr, jiffies + msecs_to_jiffies(timer_interval));
        printk(KERN_INFO "timer: loaded.\n");
        return 0;
}

static void __exit mod_exit(void)
{
        del_timer_sync(&tmr);
        printk(KERN_INFO "timer: clean up.\n");
}

module_init(mod_init);
module_exit(mod_exit);
