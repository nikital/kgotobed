#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/timekeeping.h>

MODULE_AUTHOR("Nikita Leshenko <nikita@leshenko.net>");
MODULE_LICENSE("GPL");

static struct task_struct *kgotobed_thread;
static ktime_t bed_time;
static bool armed;

static int kgotobed(void *unused)
{
    while (!kthread_should_stop()) {
        if (armed && ktime_after(ktime_get_boottime(), bed_time)) {
            pr_err("kgotobed: Time to go bed!\n");
        }
        ssleep(1);
    }
    return 0;
}

static ssize_t seconds_to_bed_show(struct kobject *kobj,
                                   struct kobj_attribute *attr,
                                   char *buf)
{
    ktime_t time_to_bed;

    if (!armed) {
        buf[0] = '\0';
        return 1;
    }

    time_to_bed = ktime_sub(bed_time, ktime_get_boottime());
	return sprintf(buf, "%lld\n", ktime_to_ns(time_to_bed) / NSEC_PER_SEC);
}

static ssize_t seconds_to_bed_store(struct kobject *kobj,
                                    struct kobj_attribute *attr,
                                    const char *buf, size_t count)
{
    s32 seconds_to_bed;
    if (sscanf(buf, "%d", &seconds_to_bed) == 1) {
        ktime_t new_bed_time = ktime_add(ktime_get_boottime(), ktime_set(seconds_to_bed, 0));
        if (!armed || ktime_before(new_bed_time, bed_time)) {
            bed_time = new_bed_time;
            armed = true;
        }
    }

    return count;
}
static struct kobj_attribute seconds_to_bed_attr =
    __ATTR(seconds_to_bed, 0644,
           seconds_to_bed_show, seconds_to_bed_store);

static int __init kgotobed_init_module(void)
{
    int ret;

    ret = sysfs_create_file(kernel_kobj, &seconds_to_bed_attr.attr);
    if (ret < 0)
        goto fail_sysfs;

    kgotobed_thread = kthread_run(kgotobed, NULL, "kgotobed");
    if (IS_ERR(kgotobed_thread)) {
        ret = PTR_ERR(kgotobed_thread);
        goto fail_thread;
    }

    return 0;

fail_thread:
    sysfs_remove_file(kernel_kobj, &seconds_to_bed_attr.attr);
fail_sysfs:
    return ret;
}

module_init(kgotobed_init_module);

static void __exit kgotobed_exit_module(void)
{
    sysfs_remove_file(kernel_kobj, &seconds_to_bed_attr.attr);
    kthread_stop(kgotobed_thread);
}

module_exit(kgotobed_exit_module);
