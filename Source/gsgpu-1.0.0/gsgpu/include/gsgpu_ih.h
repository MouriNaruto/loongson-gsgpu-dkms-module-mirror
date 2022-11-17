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

#ifndef __GSGPU_IH_H__
#define __GSGPU_IH_H__

#include <linux/chash.h>

struct gsgpu_device;

#define GSGPU_IH_CLIENTID_LEGACY 0
#define GSGPU_IH_CLIENTID_MAX SOC15_IH_CLIENTID_MAX

extern const struct gsgpu_ip_block_version gsgpu_ih_ip_block;

#define GSGPU_PAGEFAULT_HASH_BITS 8
struct gsgpu_retryfault_hashtable {
	DECLARE_CHASH_TABLE(hash, GSGPU_PAGEFAULT_HASH_BITS, 8, 0);
	spinlock_t	lock;
	int		count;
};

enum soc15_ih_clientid {
	SOC15_IH_CLIENTID_IH		= 0x00,
	SOC15_IH_CLIENTID_ACP		= 0x01,
	SOC15_IH_CLIENTID_ATHUB		= 0x02,
	SOC15_IH_CLIENTID_BIF		= 0x03,
	SOC15_IH_CLIENTID_DCE		= 0x04,
	SOC15_IH_CLIENTID_ISP		= 0x05,
	SOC15_IH_CLIENTID_PCIE0		= 0x06,
	SOC15_IH_CLIENTID_RLC		= 0x07,
	SOC15_IH_CLIENTID_SDMA0		= 0x08,
	SOC15_IH_CLIENTID_SDMA1		= 0x09,
	SOC15_IH_CLIENTID_SE0SH		= 0x0a,
	SOC15_IH_CLIENTID_SE1SH		= 0x0b,
	SOC15_IH_CLIENTID_SE2SH		= 0x0c,
	SOC15_IH_CLIENTID_SE3SH		= 0x0d,
	SOC15_IH_CLIENTID_SYSHUB	= 0x0e,
	SOC15_IH_CLIENTID_UVD1		= 0x0e,
	SOC15_IH_CLIENTID_THM		= 0x0f,
	SOC15_IH_CLIENTID_UVD		= 0x10,
	SOC15_IH_CLIENTID_VCE0		= 0x11,
	SOC15_IH_CLIENTID_VMC		= 0x12,
	SOC15_IH_CLIENTID_XDMA		= 0x13,
	SOC15_IH_CLIENTID_GRBM_CP	= 0x14,
	SOC15_IH_CLIENTID_ATS		= 0x15,
	SOC15_IH_CLIENTID_ROM_SMUIO	= 0x16,
	SOC15_IH_CLIENTID_DF		= 0x17,
	SOC15_IH_CLIENTID_VCE1		= 0x18,
	SOC15_IH_CLIENTID_PWR		= 0x19,
	SOC15_IH_CLIENTID_UTCL2		= 0x1b,
	SOC15_IH_CLIENTID_EA		= 0x1c,
	SOC15_IH_CLIENTID_UTCL2LOG	= 0x1d,
	SOC15_IH_CLIENTID_MP0		= 0x1e,
	SOC15_IH_CLIENTID_MP1		= 0x1f,

	SOC15_IH_CLIENTID_MAX,

	SOC15_IH_CLIENTID_VCN		= SOC15_IH_CLIENTID_UVD
};

/*
 * R6xx+ IH ring
 */
struct gsgpu_ih_ring {
	struct gsgpu_bo	*ring_obj;
	volatile uint32_t	*ring;
	unsigned		rptr;
	unsigned		ring_size;
	uint64_t		gpu_addr;
	uint32_t		ptr_mask;
	atomic_t		lock;
	bool                    enabled;
	unsigned		wptr_offs;
	unsigned		rptr_offs;
	bool			use_bus_addr;
	dma_addr_t		rb_dma_addr; /* only used when use_bus_addr = true */
	struct gsgpu_retryfault_hashtable *faults;
};

#define GSGPU_IH_SRC_DATA_MAX_SIZE_DW 4

struct gsgpu_iv_entry {
	unsigned client_id;
	unsigned src_id;
	unsigned ring_id;
	unsigned vmid;
	unsigned vmid_src;
	uint64_t timestamp;
	unsigned timestamp_src;
	unsigned pasid;
	unsigned pasid_src;
	unsigned src_data[GSGPU_IH_SRC_DATA_MAX_SIZE_DW];
	const uint32_t *iv_entry;
};

int gsgpu_ih_ring_init(struct gsgpu_device *adev, unsigned ring_size,
			bool use_bus_addr);
void gsgpu_ih_ring_fini(struct gsgpu_device *adev);
int gsgpu_ih_process(struct gsgpu_device *adev);
int gsgpu_ih_add_fault(struct gsgpu_device *adev, u64 key);
void gsgpu_ih_clear_fault(struct gsgpu_device *adev, u64 key);

#endif
