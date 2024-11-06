
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name f the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

	if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		printk(KERN_WARNING "Warning: repeat is either 0 or between 5 and 10\n");
	} else if (repeat > 10) {
		printk(KERN_ERR "Error: repeat is greater than 10\n");
		return -EINVAL;
	}

	for (i = 0; i < repeat; i++) {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			printk(KERN_ERR "Memory allocation failed\n");
			return -ENOMEM;
		}

		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_time_list);
		printk(KERN_INFO "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &hello_time_list, list) {
		printk(KERN_INFO "Time: %lld ns\n", ktime_to_ns(entry->time));
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello_init);
module_exit(hello_exit);


