/*
 * Copyright 2015 Advanced Micro Devices, Inc.
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
 */

#ifndef __AMD_SHARED_H__
#define __AMD_SHARED_H__

#include <drm/gsgpu_family_type.h>


#define AMD_MAX_USEC_TIMEOUT		200000  /* 200 ms */

/*
 * Chip flags
 */
enum amd_chip_flags {
	AMD_ASIC_MASK = 0x0000ffffUL,
	AMD_FLAGS_MASK  = 0xffff0000UL,
	AMD_IS_MOBILITY = 0x00010000UL,
	AMD_IS_APU      = 0x00020000UL,
	AMD_IS_PX       = 0x00040000UL,
	AMD_EXP_HW_SUPPORT = 0x00080000UL,
};

enum amd_ip_block_type {
	AMD_IP_BLOCK_TYPE_COMMON,
	AMD_IP_BLOCK_TYPE_GMC,
	AMD_IP_BLOCK_TYPE_ZIP,
	AMD_IP_BLOCK_TYPE_IH,
	AMD_IP_BLOCK_TYPE_SMC,
	AMD_IP_BLOCK_TYPE_PSP,
	AMD_IP_BLOCK_TYPE_DCE,
	AMD_IP_BLOCK_TYPE_GFX,
	AMD_IP_BLOCK_TYPE_SDMA,
	AMD_IP_BLOCK_TYPE_UVD,
	AMD_IP_BLOCK_TYPE_VCE,
	AMD_IP_BLOCK_TYPE_ACP,
	AMD_IP_BLOCK_TYPE_VCN
};

/**
 * struct amd_ip_funcs - general hooks for managing gsgpu IP Blocks
 */
struct amd_ip_funcs {
	/** @name: Name of IP block */
	char *name;
	/**
	 * @early_init:
	 *
	 * sets up early driver state (pre sw_init),
	 * does not configure hw - Optional
	 */
	int (*early_init)(void *handle);
	/** @late_init: sets up late driver/hw state (post hw_init) - Optional */
	int (*late_init)(void *handle);
	/** @sw_init: sets up driver state, does not configure hw */
	int (*sw_init)(void *handle);
	/** @sw_fini: tears down driver state, does not configure hw */
	int (*sw_fini)(void *handle);
	/** @hw_init: sets up the hw state */
	int (*hw_init)(void *handle);
	/** @hw_fini: tears down the hw state */
	int (*hw_fini)(void *handle);
	/** @late_fini: final cleanup */
	void (*late_fini)(void *handle);
	/** @suspend: handles IP specific hw/sw changes for suspend */
	int (*suspend)(void *handle);
	/** @resume: handles IP specific hw/sw changes for resume */
	int (*resume)(void *handle);
	/** @is_idle: returns current IP block idle status */
	bool (*is_idle)(void *handle);
	/** @wait_for_idle: poll for idle */
	int (*wait_for_idle)(void *handle);
	/** @check_soft_reset: check soft reset the IP block */
	bool (*check_soft_reset)(void *handle);
	/** @pre_soft_reset: pre soft reset the IP block */
	int (*pre_soft_reset)(void *handle);
	/** @soft_reset: soft reset the IP block */
	int (*soft_reset)(void *handle);
	/** @post_soft_reset: post soft reset the IP block */
	int (*post_soft_reset)(void *handle);
};


#endif /* __AMD_SHARED_H__ */
