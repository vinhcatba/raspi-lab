#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int mod_init(void)
{
    printk(KERN_ALERT "Hello world\n");
    return 0;
}
static void mod_exit(void)
{
    printk(KERN_ALERT "Goodbye\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("thanhvinhkma");
MODULE_DESCRIPTION("Custom Linux kernel module");

module_init(mod_init);
module_exit(mod_exit);
