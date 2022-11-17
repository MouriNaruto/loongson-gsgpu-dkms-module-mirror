/*
 * Copyright 2017 Advanced Micro Devices, Inc.
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
 * Authors: Christian König
 */
#ifndef __GSGPU_MN_H__
#define __GSGPU_MN_H__

/*
 * MMU Notifier
 */
struct gsgpu_mn;

enum gsgpu_mn_type {
	GSGPU_MN_TYPE_GFX,
	GSGPU_MN_TYPE_HSA,
};

#if defined(CONFIG_MMU_NOTIFIER)
void gsgpu_mn_lock(struct gsgpu_mn *mn);
void gsgpu_mn_unlock(struct gsgpu_mn *mn);
struct gsgpu_mn *gsgpu_mn_get(struct gsgpu_device *adev,
				enum gsgpu_mn_type type);
int gsgpu_mn_register(struct gsgpu_bo *bo, unsigned long addr);
void gsgpu_mn_unregister(struct gsgpu_bo *bo);
#else
static inline void gsgpu_mn_lock(struct gsgpu_mn *mn) {}
static inline void gsgpu_mn_unlock(struct gsgpu_mn *mn) {}
static inline struct gsgpu_mn *gsgpu_mn_get(struct gsgpu_device *adev,
					      enum gsgpu_mn_type type)
{
	return NULL;
}
static inline int gsgpu_mn_register(struct gsgpu_bo *bo, unsigned long addr)
{
	return -ENODEV;
}
static inline void gsgpu_mn_unregister(struct gsgpu_bo *bo) {}
#endif

#endif
