/*
 * Copyright 2014 Advanced Micro Devices, Inc.
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

#ifndef __GSGPU_IRQ_H__
#define __GSGPU_IRQ_H__

#include <linux/irqdomain.h>
#include "gsgpu_ih.h"
#include "gsgpu_dc_i2c.h"

#define GSGPU_MAX_IRQ_SRC_ID	0x100
#define GSGPU_MAX_IRQ_CLIENT_ID	0x100

struct gsgpu_device;
struct gsgpu_iv_entry;

enum gsgpu_interrupt_state {
	GSGPU_IRQ_STATE_DISABLE,
	GSGPU_IRQ_STATE_ENABLE,
};

struct gsgpu_irq_src {
	unsigned				num_types;
	atomic_t				*enabled_types;
	const struct gsgpu_irq_src_funcs	*funcs;
	void *data;
};

struct gsgpu_irq_client {
	struct gsgpu_irq_src **sources;
};

/* provided by interrupt generating IP blocks */
struct gsgpu_irq_src_funcs {
	int (*set)(struct gsgpu_device *adev, struct gsgpu_irq_src *source,
		   unsigned type, enum gsgpu_interrupt_state state);

	int (*process)(struct gsgpu_device *adev,
		       struct gsgpu_irq_src *source,
		       struct gsgpu_iv_entry *entry);
};

struct gsgpu_irq {
	bool				installed;
	spinlock_t			lock;
	/* interrupt sources */
	struct gsgpu_irq_client	client[GSGPU_IH_CLIENTID_MAX];

	/* status, etc. */
	bool				msi_enabled; /* msi enabled */

	/* interrupt ring */
	struct gsgpu_ih_ring		ih;
	const struct gsgpu_ih_funcs	*ih_funcs;
};

void gsgpu_irq_disable_all(struct gsgpu_device *adev);
irqreturn_t gsgpu_irq_handler(int irq, void *arg);

int gsgpu_irq_init(struct gsgpu_device *adev);
void gsgpu_irq_fini(struct gsgpu_device *adev);
int gsgpu_irq_add_id(struct gsgpu_device *adev,
		      unsigned client_id, unsigned src_id,
		      struct gsgpu_irq_src *source);
void gsgpu_irq_dispatch(struct gsgpu_device *adev,
			 struct gsgpu_iv_entry *entry);
int gsgpu_irq_update(struct gsgpu_device *adev, struct gsgpu_irq_src *src,
		      unsigned type);
int gsgpu_irq_get(struct gsgpu_device *adev, struct gsgpu_irq_src *src,
		   unsigned type);
int gsgpu_irq_put(struct gsgpu_device *adev, struct gsgpu_irq_src *src,
		   unsigned type);
bool gsgpu_irq_enabled(struct gsgpu_device *adev, struct gsgpu_irq_src *src,
			unsigned type);
void gsgpu_irq_gpu_reset_resume_helper(struct gsgpu_device *adev);

#endif
