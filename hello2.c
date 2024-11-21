#include <linux/module.h>
#include <linux/kernel.h>
#include "hello1.h"

MODULE_AUTHOR("Plaksenkov Andriy <andriy.plaksenkov@gmail.com>");
MODULE_DESCRIPTION("hello2 module using hello1's print_hello()");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat = 1;
module_param(repeat, uint, 0660);
MODULE_PARM_DESC(repeat, "Number of times to call print_hello()");

extern void print_hello(void);

static int __init hello2_init(void)
{
	int i;

	pr_info("hello2 module loaded\n");

	if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		pr_warn("Warning: repeat is either 0 or between 5 and 10\n");
	} else if (repeat > 10) {
		pr_err("Error: repeat is greater than 10\n");
		return -EINVAL;
	}

	for (i = 0; i < repeat; i++)
	print_hello();
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

