#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

static int input_param;
module_param(input_param, int, 0644);

static int __init hello_init(void) {
    printk(KERN_INFO "Hello world, Parameter has been passed: %d\n", input_param);
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye world\n");
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Passing Argument from Command Line");
MODULE_AUTHOR("David Lee");

module_init(hello_init);
module_exit(hello_exit);
