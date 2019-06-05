#include <linux/module.h>       // included for all kernel modules
#include <linux/kernel.h>       // KERN_INFO
#include <linux/init.h>         // __init, __exit macros
#include <linux/kthread.h>      // kthread_run, kthread_oneop... 
#include <linux/delay.h>        // ssleep(secs) blocks a kthread
#include <linux/sched/signal.h> // signal_pending
#include <linux/slab.h>         // kmalloc, kfree
#include <linux/gfp.h>          // GFP_KERNEL flag

#include "kthread_ex.h"

static int counter;
static struct task_struct *thread_one;
static struct task_struct *thread_two;
static struct kthread_data *data_one;
static struct kthread_data *data_two;
static int ret;

static int counter_fn(void* data)
{
        /** Allow the SIGKILL signal */
        /** allow_signal(SIGKILL); */

        while (!kthread_should_stop()) {
                ssleep(((struct kthread_data*)data)->interval);
                counter += 1;
                printk(KERN_INFO "kthread_ex: %d add to counter (value: %d)\n",
                        ((struct kthread_data*)data)->id, counter);
                
                /** Check if the signal is pending */
                /** if (signal_pending(thread_one)) */
                /**         printk(KERN_INFO "kthread_ex: You can't stop me!\n"); */
        }
        printk(KERN_INFO "kthread_ex: Counter (%d) has been stopped\n",
                        ((struct kthread_data*)data)->id);
        return counter;
}

static int __init mod_init(void)
{
        counter = 0;

        data_one = (struct kthread_data*)kmalloc(sizeof(struct kthread_data), 
                GFP_KERNEL);
        data_two = (struct kthread_data*)kmalloc(sizeof(struct kthread_data), 
                GFP_KERNEL);
        data_one->interval = 2;
        data_two->interval = 1;
        data_one->id = 0;
        
        data_two->id = 1;
        thread_one = kthread_run(counter_fn, data_one, "kthread_counter1");
        thread_two = kthread_run(counter_fn, data_two, "kthread_counter2");
        
        if (IS_ERR(thread_one)) {
                printk(KERN_INFO "kthread_ex: ERROR! Could not run kthread_run\n");
                return PTR_ERR(thread_one);
        }
        if (IS_ERR(thread_two)) {
                printk(KERN_INFO "kthread_ex: ERROR! Could not run kthread_run\n");
                return PTR_ERR(thread_two);
        }

        printk(KERN_INFO "kthread_ex: Loaded (thread id: %d).\n", thread_one->pid);
        printk(KERN_INFO "kthread_ex: Loaded (thread id: %d).\n", thread_two->pid);
        return 0;
}

static void __exit mod_exit(void)
{
        ret = kthread_stop(thread_one);
        if (ret != -EINTR)
                printk(KERN_INFO "kthread_ex: Counter thread has stopped\n");
        ret = kthread_stop(thread_two);
        if (ret != -EINTR)
                printk(KERN_INFO "kthread_ex: Counter thread has stopped\n");

        kfree(data_one);
        printk(KERN_INFO "kthread_ex: Clean up.\n");
}

module_init(mod_init);
module_exit(mod_exit);
