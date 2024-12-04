// SPDX-License-Identifier: Dual BSD/GPL

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/errno.h>

MODULE_AUTHOR("Andriy Plaksenkov <andriy.plaksenkov@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat = 1;
module_param(repeat, uint, 0660);
MODULE_PARM_DESC(repeat, "Number of times to print 'Hello, world!'");

struct hello_time {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_time_list);

static int __init hello_init(void)
{
	struct hello_time *entry;
	int i;

	if (repeat == 0 || (repeat >= 5 && repeat <= 10))
		pr_warn("Warning: hello_count is %u\n", repeat);
	else if (repeat > 10)
		BUG_ON(1);

	for (i = 0; i < repeat; i++) {
		if (i == 5)
			entry = NULL;
		else
			entry = kmalloc(sizeof(*entry), GFP_KERNEL);

		BUG_ON(!entry);

		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_time_list);
		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &hello_time_list, list) {
		pr_info("Time: %lld ns\n", ktime_to_ns(entry->time));
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello_init);
module_exit(hello_exit);

