#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kthread.h>

MODULE_AUTHOR("Nikita Leshenko <nikita@leshenko.net>");
MODULE_LICENSE("GPL");

static struct task_struct *kgotobed_thread;

static int kgotobed(void *unused)
{
    while (!kthread_should_stop()) {
        pr_info("kgotobed thread\n");
        ssleep(1);
    }
    pr_info("kgotobed done\n");
    return 0;
}

static int __init kgotobed_init_module(void)
{
    kgotobed_thread = kthread_run(kgotobed, NULL, "kgotobed");
    return PTR_ERR_OR_ZERO(kgotobed_thread);
}

module_init(kgotobed_init_module);

static void __exit kgotobed_exit_module(void)
{
    kthread_stop(kgotobed_thread);
}

module_exit(kgotobed_exit_module);
