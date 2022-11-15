#include <linux/module.h>

static int __init init_base(void)
{
    printk("----Hello. World----\n");
    return 0;
}

static void __exit exit_base(void)
{
    printk("----Bye----\n");
}

module_init(init_base);
module_exit(exit_base);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR("Zhao Lei");
MODULE_DESCRIPTION("For test");