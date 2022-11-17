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
#ifndef __GSGPU_JOB_H__
#define __GSGPU_JOB_H__

/* bit set means command submit involves a preamble IB */
#define GSGPU_PREAMBLE_IB_PRESENT          (1 << 0)
/* bit set means preamble IB is first presented in belonging context */
#define GSGPU_PREAMBLE_IB_PRESENT_FIRST    (1 << 1)
/* bit set means context switch occured */
#define GSGPU_HAVE_CTX_SWITCH              (1 << 2)

#define to_gsgpu_job(sched_job)		\
		container_of((sched_job), struct gsgpu_job, base)

struct gsgpu_fence;

struct gsgpu_job {
	struct drm_sched_job    base;
	struct gsgpu_vm	*vm;
	struct gsgpu_sync	sync;
	struct gsgpu_sync	sched_sync;
	struct gsgpu_ib	*ibs;
	struct dma_fence	*fence; /* the hw fence */
	uint32_t		preamble_status;
	uint32_t		num_ibs;
	void			*owner;
	bool                    vm_needs_flush;
	uint64_t		vm_pd_addr;
	unsigned		vmid;
	unsigned		pasid;
	uint32_t		vram_lost_counter;

	/* user fence handling */
	uint64_t		uf_addr;
	uint64_t		uf_sequence;

};

int gsgpu_job_alloc(struct gsgpu_device *adev, unsigned num_ibs,
		     struct gsgpu_job **job, struct gsgpu_vm *vm);
int gsgpu_job_alloc_with_ib(struct gsgpu_device *adev, unsigned size,
			     struct gsgpu_job **job);

void gsgpu_job_free_resources(struct gsgpu_job *job);
void gsgpu_job_free(struct gsgpu_job *job);
int gsgpu_job_submit(struct gsgpu_job *job, struct drm_sched_entity *entity,
		      void *owner, struct dma_fence **f);
int gsgpu_job_submit_direct(struct gsgpu_job *job, struct gsgpu_ring *ring,
			     struct dma_fence **fence);
#endif
