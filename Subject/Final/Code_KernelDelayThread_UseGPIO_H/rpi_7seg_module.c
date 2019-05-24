#include <linux/init.h>    // not sure if it is necessary
#include <linux/module.h>  // module_init, module_exit, kobject
#include <linux/kthread.h> // kthread_run, kthread_stop
#include "seven_seg.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David D.W.Lee");
MODULE_DESCRIPTION("Raspberry Pi GPIO control for 7-segment display Using Delay running in a kernel Thread");
MODULE_VERSION("1.0");

/* GPIO */

void rpi_7seg_gpio_init(void)
{
    printk(KERN_INFO "RPi7Seg: starting gpio...");
    init_7seg_gpio();
    printk(KERN_INFO "RPi7Seg: starting gpio done.");
}

void rpi_7seg_gpio_exit(void)
{
    printk(KERN_INFO "RPi7Seg: stopping gpio...");
    free_7seg_gpio();
    printk(KERN_INFO "RPi7Seg: stopping gpio done.");
}

/* Pseudo File System */

#define PSEUDO_FILENAME display

static struct kobject *g_kobject;

static ssize_t set_7seg(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
    int number;
    sscanf(buff, "%d", &number);
    printk(KERN_INFO "RPi7Seg: received number: %d\n", number);
    setNumber(number);

    return count;
}

static struct kobj_attribute rpi_7seg_attribute = __ATTR(PSEUDO_FILENAME, (S_IWUSR | S_IRUGO), NULL, set_7seg);

void rpi_7seg_sysfs_init(void)
{
    printk(KERN_INFO "RPi7Seg: starting sysfs...");
    g_kobject = kobject_create_and_add("rpi_7seg", NULL);
    if (sysfs_create_file(g_kobject, &rpi_7seg_attribute.attr))
    {
        pr_debug("failed to create rpi_7seg sysfs!\n");
    }
    printk(KERN_INFO "RPi7Seg: starting sysfs done.");
}

void rpi_7seg_sysfs_exit(void)
{
    printk(KERN_INFO "RPi7Seg: stopping sysfs...");
    kobject_put(g_kobject);
    printk(KERN_INFO "RPi7Seg: stopping sysfs done.");
}

/* Kernel Thread */

#define THREAD_PRIORITY 45
#define THREAD_NAME "rpi_7seg"

struct task_struct *g_task;

struct sched_param
{
    int sched_priority;
};

int rpi_7seg_thread(void *data)
{
    struct task_struct *TSK;
    struct sched_param PARAM = {.sched_priority = MAX_RT_PRIO - 50};
    TSK = current;

    PARAM.sched_priority = THREAD_PRIORITY;
    sched_setscheduler(TSK, SCHED_FIFO, &PARAM);

    while (1)
    {
        // Display the minimal time in case of the number change
        showAllDigits(DISP_DELAY_US * DigitUsed);
        if (kthread_should_stop())
            break;
    }
    return 0;
}

void rpi_7seg_thread_init(void)
{
    printk(KERN_INFO "RPi7Seg: starting thread...");
    g_task = kthread_run(rpi_7seg_thread, NULL, THREAD_NAME);
    printk(KERN_INFO "RPi7Seg: starting thread done.");
}

void rpi_7seg_thread_exit(void)
{
    printk(KERN_INFO "RPi7Seg: stopping thread...");
    kthread_stop(g_task);
    printk(KERN_INFO "RPi7Seg: stopping thread done.");
}

/* Module */

static int __init rpi_7seg_init(void)
{
    printk(KERN_INFO "RPi7Seg: staring...");
    rpi_7seg_gpio_init();
    rpi_7seg_thread_init();
    rpi_7seg_sysfs_init();
    printk(KERN_INFO "RPi7Seg: staring done.");
    return 0;
}

static void __exit rpi_7seg_exit(void)
{
    printk(KERN_INFO "RPi7Seg: stopping...");
    rpi_7seg_sysfs_exit();
    rpi_7seg_thread_exit();
    rpi_7seg_gpio_exit();
    printk(KERN_INFO "RPi7Seg: stopping done.");
}

module_init(rpi_7seg_init);
module_exit(rpi_7seg_exit);
