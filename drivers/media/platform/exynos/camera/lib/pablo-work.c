/*
 * Samsung EXYNOS IS (Imaging Subsystem) driver
 *
 *
 * Copyright (c) 2019 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/errno.h>

#include "pablo-work.h"
#include "pablo-debug.h"
#include "is-config.h"

static int __get_free_work_irq(struct is_work_list *this,
	struct is_work **work)
{
	int ret = 0;

	if (work) {
		spin_lock(&this->slock_free);

		if (this->work_free_cnt) {
			*work = container_of(this->work_free_head.next,
					struct is_work, list);
			list_del(&(*work)->list);
			this->work_free_cnt--;
		} else
			*work = NULL;

		spin_unlock(&this->slock_free);
	} else {
		ret = -EFAULT;
		err_hw("item is null ptr");
	}

	return ret;
}

static int __set_req_work_irq(struct is_work_list *this,
	struct is_work *work)
{
	int ret = 0;

	if (work) {
		spin_lock(&this->slock_request);
		list_add_tail(&work->list, &this->work_request_head);
		this->work_request_cnt++;
#ifdef TRACE_WORK
		print_req_work_list(this);
#endif

		spin_unlock(&this->slock_request);
	} else {
		ret = -EFAULT;
		err_hw("item is null ptr");
	}

	return ret;
}

static int __get_free_work(struct is_work_list *this,
	struct is_work **work)
{
	int ret = 0;
	ulong flags = 0;

	if (work) {
		spin_lock_irqsave(&this->slock_free, flags);

		if (this->work_free_cnt) {
			*work = container_of(this->work_free_head.next,
					struct is_work, list);
			list_del(&(*work)->list);
			this->work_free_cnt--;
		} else
			*work = NULL;

		spin_unlock_irqrestore(&this->slock_free, flags);
	} else {
		ret = -EFAULT;
		err_hw("item is null ptr");
	}

	return ret;
}

static int __set_req_work(struct is_work_list *this,
	struct is_work *work)
{
	int ret = 0;
	ulong flags = 0;

	if (work) {
		spin_lock_irqsave(&this->slock_request, flags);
		list_add_tail(&work->list, &this->work_request_head);
		this->work_request_cnt++;
#ifdef TRACE_WORK
		print_req_work_list(this);
#endif

		spin_unlock_irqrestore(&this->slock_request, flags);
	} else {
		ret = -EFAULT;
		err_hw("item is null ptr");
	}

	return ret;
}

void init_work_list(struct is_work_list *this, u32 id, u32 count)
{
	u32 i;

	this->id = id;
	this->work_free_cnt	= 0;
	this->work_request_cnt	= 0;
	INIT_LIST_HEAD(&this->work_free_head);
	INIT_LIST_HEAD(&this->work_request_head);
	spin_lock_init(&this->slock_free);
	spin_lock_init(&this->slock_request);
	for (i = 0; i < count; ++i)
		set_free_work(this, &this->work[i]);

	init_waitqueue_head(&this->wait_queue);
}
EXPORT_SYMBOL_GPL(init_work_list);

int set_free_work(struct is_work_list *this, struct is_work *work)
{
	int ret = 0;
	ulong flags;

	if (work) {
		spin_lock_irqsave(&this->slock_free, flags);

		list_add_tail(&work->list, &this->work_free_head);
		this->work_free_cnt++;
#ifdef TRACE_WORK
		print_fre_work_list(this);
#endif

		spin_unlock_irqrestore(&this->slock_free, flags);
	} else {
		ret = -EFAULT;
		err("item is null ptr\n");
	}

	return ret;
}
EXPORT_SYMBOL_GPL(set_free_work);

int get_free_work(struct is_work_list *this, struct is_work **work)
{
	if (in_irq())
		return __get_free_work_irq(this, work);
	else
		return __get_free_work(this, work);
}
EXPORT_SYMBOL_GPL(get_free_work);

int set_req_work(struct is_work_list *this, struct is_work *work)
{
	if (in_irq())
		return __set_req_work_irq(this, work);
	else
		return __set_req_work(this, work);
}
EXPORT_SYMBOL_GPL(set_req_work);

int get_req_work(struct is_work_list *this, struct is_work **work)
{
	int ret = 0;
	ulong flags;

	if (work) {
		spin_lock_irqsave(&this->slock_request, flags);

		if (this->work_request_cnt) {
			*work = container_of(this->work_request_head.next,
					struct is_work, list);
			list_del(&(*work)->list);
			this->work_request_cnt--;
		} else
			*work = NULL;

		spin_unlock_irqrestore(&this->slock_request, flags);
	} else {
		ret = -EFAULT;
		err("item is null ptr\n");
	}

	return ret;
}
EXPORT_SYMBOL_GPL(get_req_work);

int print_fre_work_list(struct is_work_list *this)
{
	struct is_work *work, *temp;

	if (this->id >= WORK_MAX_MAP)
		return 0;

	err("[INF] fre(%02X, %02d): ", this->id, this->work_free_cnt);

	list_for_each_entry_safe(work, temp, &this->work_free_head, list) {
		is_cont("%X(F%d)->", work->msg.command, work->fcount);
	}

	is_cont("X\n");

	return 0;
}
EXPORT_SYMBOL_GPL(print_fre_work_list);

int print_req_work_list(struct is_work_list *this)
{
	struct is_work *work, *temp;

	if (this->id >= WORK_MAX_MAP)
		return 0;

	err("[INF] req(%02X, %02d): ", this->id, this->work_request_cnt);

	list_for_each_entry_safe(work, temp, &this->work_request_head, list) {
		is_cont("%X([%d][G%llx][F%d])->", work->msg.command,
				work->msg.instance, work->msg.group, work->fcount);
	}

	is_cont("X\n");

	return 0;
}
EXPORT_SYMBOL_GPL(print_req_work_list);
