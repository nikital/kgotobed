#include <linux/module.h>

MODULE_AUTHOR("Nikita Leshenko <nikita@leshenko.net>");
MODULE_LICENSE("GPL");

static int __init kgotobed_init_module(void)
{
	pr_info("kgotobed loaded\n");
	return 0;
}

module_init(kgotobed_init_module);

static void __exit kgotobed_exit_module(void)
{
}

module_exit(kgotobed_exit_module);
