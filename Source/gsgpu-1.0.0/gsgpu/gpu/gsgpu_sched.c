/*
 * Copyright 2017 Valve Corporation
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
 * Authors: Andres Rodriguez <andresx7@gmail.com>
 */

#include <linux/fdtable.h>
#include <linux/pid.h>
#include <drm/gsgpu_drm.h>
#include "gsgpu.h"

#include "gsgpu_vm.h"

enum drm_sched_priority gsgpu_to_sched_priority(int gsgpu_priority)
{
	switch (gsgpu_priority) {
	case GSGPU_CTX_PRIORITY_VERY_HIGH:
		return DRM_SCHED_PRIORITY_HIGH_HW;
	case GSGPU_CTX_PRIORITY_HIGH:
		return DRM_SCHED_PRIORITY_HIGH_SW;
	case GSGPU_CTX_PRIORITY_NORMAL:
		return DRM_SCHED_PRIORITY_NORMAL;
	case GSGPU_CTX_PRIORITY_LOW:
	case GSGPU_CTX_PRIORITY_VERY_LOW:
		return DRM_SCHED_PRIORITY_LOW;
	case GSGPU_CTX_PRIORITY_UNSET:
		return DRM_SCHED_PRIORITY_UNSET;
	default:
		WARN(1, "Invalid context priority %d\n", gsgpu_priority);
		return DRM_SCHED_PRIORITY_INVALID;
	}
}

static int gsgpu_sched_process_priority_override(struct gsgpu_device *adev,
						  int fd,
						  enum drm_sched_priority priority)
{
	struct file *filp = fget(fd);
	struct drm_file *file;
	struct gsgpu_fpriv *fpriv;
	struct gsgpu_ctx *ctx;
	uint32_t id;

	if (!filp)
		return -EINVAL;

	file = filp->private_data;
	fpriv = file->driver_priv;
	idr_for_each_entry(&fpriv->ctx_mgr.ctx_handles, ctx, id)
		gsgpu_ctx_priority_override(ctx, priority);

	fput(filp);

	return 0;
}

int gsgpu_sched_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *filp)
{
	union drm_gsgpu_sched *args = data;
	struct gsgpu_device *adev = dev->dev_private;
	enum drm_sched_priority priority;
	int r;

	priority = gsgpu_to_sched_priority(args->in.priority);
	if (args->in.flags || priority == DRM_SCHED_PRIORITY_INVALID)
		return -EINVAL;

	switch (args->in.op) {
	case GSGPU_SCHED_OP_PROCESS_PRIORITY_OVERRIDE:
		r = gsgpu_sched_process_priority_override(adev,
							   args->in.fd,
							   priority);
		break;
	default:
		DRM_ERROR("Invalid sched op specified: %d\n", args->in.op);
		r = -EINVAL;
		break;
	}

	return r;
}
