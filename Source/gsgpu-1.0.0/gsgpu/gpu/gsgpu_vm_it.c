/*
 * Copyright (c) 2018 Loongson Technology Co., Ltd.
 * Authors:
 *	Zhang Zhaoge <zhangzhaoge@loongson.cn>
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/interval_tree_generic.h>
#include "gsgpu_vm_it.h"

#define START(node) ((node)->start)
#define LAST(node) ((node)->last)

INTERVAL_TREE_DEFINE(struct gsgpu_bo_va_mapping, rb, uint64_t, __subtree_last,
		     START, LAST,, gsgpu_vm_it)

