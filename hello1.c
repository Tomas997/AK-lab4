#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Plaksenkov Andriy <andriy.plaksenkov@gmail.com>");
MODULE_DESCRIPTION("hello1 module providing print_hello()");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_item {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_item *item;

	item = kmalloc(sizeof(*item), GFP_KERNEL);
	if (!item) {
		pr_err("Memory allocation failed\n");
		return;
	}

	item->start_time = ktime_get();
	pr_info("Hello, world!\n");
	item->end_time = ktime_get();

	list_add(&item->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
	struct hello_item *item, *tmp;

	list_for_each_entry_safe(item, tmp, &hello_list, list) {
		pr_info("Print duration: %lld ns\n",
			ktime_to_ns(item->end_time) - ktime_to_ns(item->start_time));
		list_del(&item->list);
		kfree(item);
	}
}

module_exit(hello1_exit);

