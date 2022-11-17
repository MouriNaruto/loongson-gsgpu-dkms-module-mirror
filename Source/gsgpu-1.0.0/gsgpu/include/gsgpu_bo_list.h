/*
 * Copyright 2018 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef __GSGPU_BO_LIST_H__
#define __GSGPU_BO_LIST_H__

#include <drm/ttm/ttm_execbuf_util.h>
#include <drm/gsgpu_drm.h>

struct gsgpu_device;
struct gsgpu_bo;
struct gsgpu_bo_va;
struct gsgpu_fpriv;

struct gsgpu_bo_list_entry {
	struct gsgpu_bo		*robj;
	struct ttm_validate_buffer	tv;
	struct gsgpu_bo_va		*bo_va;
	uint32_t			priority;
	struct page			**user_pages;
	int				user_invalidated;
};

struct gsgpu_bo_list {
	struct rcu_head rhead;
	struct kref refcount;
	unsigned first_userptr;
	unsigned num_entries;
};

int gsgpu_bo_list_get(struct gsgpu_fpriv *fpriv, int id,
		       struct gsgpu_bo_list **result);
void gsgpu_bo_list_get_list(struct gsgpu_bo_list *list,
			     struct list_head *validated);
void gsgpu_bo_list_put(struct gsgpu_bo_list *list);
int gsgpu_bo_create_list_entry_array(struct drm_gsgpu_bo_list_in *in,
				      struct drm_gsgpu_bo_list_entry **info_param);

int gsgpu_bo_list_create(struct gsgpu_device *adev,
				 struct drm_file *filp,
				 struct drm_gsgpu_bo_list_entry *info,
				 unsigned num_entries,
				 struct gsgpu_bo_list **list);

static inline struct gsgpu_bo_list_entry *
gsgpu_bo_list_array_entry(struct gsgpu_bo_list *list, unsigned index)
{
	struct gsgpu_bo_list_entry *array = (void *)&list[1];

	return &array[index];
}

#define gsgpu_bo_list_for_each_entry(e, list) \
	for (e = gsgpu_bo_list_array_entry(list, 0); \
	     e != gsgpu_bo_list_array_entry(list, (list)->num_entries); \
	     ++e)

#define gsgpu_bo_list_for_each_userptr_entry(e, list) \
	for (e = gsgpu_bo_list_array_entry(list, (list)->first_userptr); \
	     e != gsgpu_bo_list_array_entry(list, (list)->num_entries); \
	     ++e)

#endif
