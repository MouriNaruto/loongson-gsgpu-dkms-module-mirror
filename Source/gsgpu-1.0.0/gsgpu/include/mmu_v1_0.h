/*
 * Copyright 2020 Loongson Technology, Inc.
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

#ifndef __MMU_V1_0_H__
#define __MMU_V1_0_H__

#define GSGPU_MMU_GEN_CTRL_OFFSET       0x10000
#define GSGPU_MMU_EXC_CTRL_OFFSET       0x10004
#define GSGPU_MMU_EXADDR_LO_OFFSET      0x10008
#define GSGPU_MMU_EXADDR_HI_OFFSET      0x1000c
#define GSGPU_MMU_SAFE_LO_OFFSET        0x10010
#define GSGPU_MMU_SAFE_HI_OFFSET        0x10014
#define GSGPU_MMU_FLUSH_CTRL_OFFSET     0x10018
#define GSGPU_MMU_MISC_CTRL_OFFSET      0x1001c
#define GSGPU_MMU_PGD_LO_OFFSET         0x10020
#define GSGPU_MMU_PGD_HI_OFFSET         0x10024
#define GSGPU_MMU_DIR_CTRL_OFFSET       0x100a0


#define MMU_ENABLE 						0x01
#define MMU_SET_PGD						0x02
#define MMU_SET_SAFE 					0x03
#define MMU_SET_DIR						0x04
#define MMU_SET_EXC						0x05
#define MMU_FLUSH						0x06


#define GSGPU_MMU_PTE_SIZE 				8
#define GSGPU_MMU_PGD_REG_SIZE    		8
#define GSGPU_MMU_VMID_OF_PGD(vmid)   \
			     (GSGPU_MMU_PGD_LO_OFFSET + ((vmid) * GSGPU_MMU_PGD_REG_SIZE))

#define GSGPU_MMU_DIR_CTRL_256M_1LVL ((14 << 26 | 0 << 20) | \
			              (14 << 16 | 0 << 10) | \
			              (14 <<  6 | 14 <<  0))

#define GSGPU_MMU_DIR_CTRL_1T_3LVL   (( 4 << 26 | 36 << 20) | \
			              (11 << 16 | 25 << 10) | \
			              (11 <<  6 | 14 <<  0))

#define GSGPU_MMU_FLUSH_DOMAIN_SHIFT 	8
#define GSGPU_MMU_FLUSH_EN 				BIT(0)
#define GSGPU_MMU_FLUSH_ALL 			BIT(1)
#define GSGPU_MMU_FLUSH_VMID 			0

#define GSGPU_MMU_FLUSH_PKT(vmid, all) \
			     ((vmid << GSGPU_MMU_FLUSH_DOMAIN_SHIFT) | (all) | GSGPU_MMU_FLUSH_EN )


extern const struct gsgpu_ip_block_version mmu_v1_0_ip_block;

#endif
