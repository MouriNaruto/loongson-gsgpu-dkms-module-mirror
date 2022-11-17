#
# Copyright 2017 Advanced Micro Devices, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
#
# Makefile for the drm device driver.  This driver provides support for the
# Direct Rendering Infrastructure (DRI) in XFree86 4.1.0 and higher.

FULL_LS_PATH=$(src)/..

ccflags-y := -Iinclude/drm -I$(FULL_LS_PATH)/include/asic_reg \
	-I$(FULL_LS_PATH)/include \

CFLAGS_gsgpu_fence.o += -O
gsgpu-y := gsgpu_drv.o

# add KMS driver
gsgpu-y += gsgpu_device.o gsgpu_kms.o \
	gsgpu_fence.o gsgpu_ttm.o gsgpu_object.o gsgpu_gart.o \
	gsgpu_display.o gsgpu_cp.o \
	gsgpu_fb.o gsgpu_gem.o gsgpu_ring.o \
	gsgpu_cs.o gsgpu_benchmark.o gsgpu_test.o \
	gsgpu_trace_points.o \
	gsgpu_sa.o gsgpu_zip_meta.o \
	gsgpu_prime.o gsgpu_vm.o gsgpu_vm_it.o gsgpu_ib.o \
	gsgpu_bo_list.o gsgpu_ctx.o gsgpu_sync.o \
	gsgpu_gtt_mgr.o gsgpu_vram_mgr.o \
	gsgpu_queue_mgr.o gsgpu_sched.o gsgpu_debugfs.o \
	gsgpu_ids.o gsgpu_hw_sema.o gsgpu_pm.o

gsgpu-y += gsgpu_common.o

# add MMU block
gsgpu-y +=  gsgpu_mmu.o

# add ZIP block
gsgpu-y += gsgpu_zip.o

# add IH block
gsgpu-y += \
	gsgpu_irq.o \
	gsgpu_ih.o \

# add GFX block
gsgpu-y += gsgpu_gfx.o

# add async DMA block
gsgpu-y += gsgpu_xdma.o 

# GPU scheduler
gsgpu-y += gsgpu_job.o

gsgpu-$(CONFIG_COMPAT) += gsgpu_ioc32.o
gsgpu-$(CONFIG_MMU_NOTIFIER) += gsgpu_mn.o

# add DC block
gsgpu-y += gsgpu_dc_drv.o gsgpu_dc_crtc.o gsgpu_dc_hdmi.o \
	  gsgpu_dc_irq.o gsgpu_dc_plane.o gsgpu_dc_connector.o \
	  gsgpu_dc_encoder.o gsgpu_dc_i2c.o  gsgpu_dc_cursor.o \
	  gsgpu_dc_vbios.o

include $(FULL_LS_PATH)/backport/Makefile

obj-$(CONFIG_DRM_GSGPU)+= gsgpu.o
