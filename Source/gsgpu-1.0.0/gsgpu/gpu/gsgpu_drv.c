/*
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <drm/drmP.h>
#include <drm/gsgpu_drm.h>
#include <drm/drm_gem.h>
#include "gsgpu_drv.h"

#include <drm/drm_pciids.h>
#include <linux/console.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/vga_switcheroo.h>
#include <drm/drm_crtc_helper.h>

#include "gsgpu.h"
#include "gsgpu_irq.h"


/*
 * KMS wrapper.
 * - 3.0.0 - initial driver
 * - 3.1.0 - allow reading more status registers (GRBM, SRBM, XDMA, CP)
 * - 3.2.0 - GFX8: Uses EOP_TC_WB_ACTION_EN, so UMDs don't have to do the same
 *           at the end of IBs.
 * - 3.3.0 - Add VM support for UVD on supported hardware.
 * - 3.4.0 - Add GSGPU_INFO_NUM_EVICTIONS.
 * - 3.5.0 - Add support for new UVD_NO_OP register.
 * - 3.6.0 - kmd involves use CONTEXT_CONTROL in ring buffer.
 * - 3.7.0 - Add support for VCE clock list packet
 * - 3.8.0 - Add support raster config init in the kernel
 * - 3.9.0 - Add support for memory query info about VRAM and GTT.
 * - 3.10.0 - Add support for new fences ioctl, new gem ioctl flags
 * - 3.11.0 - Add support for sensor query info (clocks, temp, etc).
 * - 3.12.0 - Add query for double offchip LDS buffers
 * - 3.13.0 - Add PRT support
 * - 3.14.0 - Fix race in gsgpu_ctx_get_fence() and note new functionality
 * - 3.15.0 - Export more gpu info for gfx9
 * - 3.16.0 - Add reserved vmid support
 * - 3.17.0 - Add GSGPU_NUM_VRAM_CPU_PAGE_FAULTS.
 * - 3.18.0 - Export gpu always on cu bitmap
 * - 3.19.0 - Add support for UVD MJPEG decode
 * - 3.20.0 - Add support for local BOs
 * - 3.21.0 - Add DRM_GSGPU_FENCE_TO_HANDLE ioctl
 * - 3.22.0 - Add DRM_GSGPU_SCHED ioctl
 * - 3.23.0 - Add query for VRAM lost counter
 * - 3.24.0 - Add high priority compute support for gfx9
 * - 3.25.0 - Add support for sensor query info (stable pstate sclk/mclk).
 * - 3.26.0 - GFX9: Process GSGPU_IB_FLAG_TC_WB_NOT_INVALIDATE.
 * - 3.27.0 - Add new chunk to to GSGPU_CS to enable BO_LIST creation.
 */
#define KMS_DRIVER_MAJOR	0
#define KMS_DRIVER_MINOR	1
#define KMS_DRIVER_PATCHLEVEL	0

int gsgpu_vram_limit = 0;
int gsgpu_vis_vram_limit = 0;
int gsgpu_gart_size = -1; /* auto */
int gsgpu_gtt_size = -1; /* auto */
int gsgpu_moverate = -1; /* auto */
int gsgpu_benchmarking = 0;
int gsgpu_testing = 0;
int gsgpu_disp_priority = 0;
int gsgpu_msi = -1;
int gsgpu_lockup_timeout = 10000;
int gsgpu_runtime_pm = -1;
int gsgpu_vm_size = -1;
int gsgpu_vm_block_size = -1;
int gsgpu_vm_fault_stop = 0;
int gsgpu_vm_debug = 0;
int gsgpu_vram_page_split = 2048;
int gsgpu_vm_update_mode = -1;
int gsgpu_exp_hw_support = 0;
int gsgpu_sched_jobs = 32;
int gsgpu_sched_hw_submission = 2;
int gsgpu_job_hang_limit = 0;
int gsgpu_gpu_recovery = 1; /* auto */
int gsgpu_using_ram = 0; /* using system  memory for gpu*/

int gsgpu_lg100_support = 1;
MODULE_PARM_DESC(LG100_support, "LG100 support (1 = enabled (default), 0 = disabled");
module_param_named(LG100_support, gsgpu_lg100_support, int, 0444);

/**
 * DOC: vramlimit (int)
 * Restrict the total amount of VRAM in MiB for testing.  The default is 0 (Use full VRAM).
 */
MODULE_PARM_DESC(vramlimit, "Restrict VRAM for testing, in megabytes");
module_param_named(vramlimit, gsgpu_vram_limit, int, 0600);

/**
 * DOC: vis_vramlimit (int)
 * Restrict the amount of CPU visible VRAM in MiB for testing.  The default is 0 (Use full CPU visible VRAM).
 */
MODULE_PARM_DESC(vis_vramlimit, "Restrict visible VRAM for testing, in megabytes");
module_param_named(vis_vramlimit, gsgpu_vis_vram_limit, int, 0444);

/**
 * DOC: gartsize (uint)
 * Restrict the size of GART in Mib (32, 64, etc.) for testing. The default is -1 (The size depends on asic).
 */
MODULE_PARM_DESC(gartsize, "Size of GART to setup in megabytes (32, 64, etc., -1=auto)");
module_param_named(gartsize, gsgpu_gart_size, uint, 0600);

/**
 * DOC: gttsize (int)
 * Restrict the size of GTT domain in MiB for testing. The default is -1 (It's VRAM size if 3GB < VRAM < 3/4 RAM,
 * otherwise 3/4 RAM size).
 */
MODULE_PARM_DESC(gttsize, "Size of the GTT domain in megabytes (-1 = auto)");
module_param_named(gttsize, gsgpu_gtt_size, int, 0600);

/**
 * DOC: moverate (int)
 * Set maximum buffer migration rate in MB/s. The default is -1 (8 MB/s).
 */
MODULE_PARM_DESC(moverate, "Maximum buffer migration rate in MB/s. (32, 64, etc., -1=auto, 0=1=disabled)");
module_param_named(moverate, gsgpu_moverate, int, 0600);

/**
 * DOC: benchmark (int)
 * Run benchmarks. The default is 0 (Skip benchmarks).
 */
MODULE_PARM_DESC(benchmark, "Run benchmark");
module_param_named(benchmark, gsgpu_benchmarking, int, 0444);

/**
 * DOC: test (int)
 * Test BO GTT->VRAM and VRAM->GTT GPU copies. The default is 0 (Skip test, only set 1 to run test).
 */
MODULE_PARM_DESC(test, "Run tests");
module_param_named(test, gsgpu_testing, int, 0444);

/**
 * DOC: disp_priority (int)
 * Set display Priority (1 = normal, 2 = high). Only affects non-DC display handling. The default is 0 (auto).
 */
MODULE_PARM_DESC(disp_priority, "Display Priority (0 = auto, 1 = normal, 2 = high)");
module_param_named(disp_priority, gsgpu_disp_priority, int, 0444);

/**
 * DOC: msi (int)
 * To disable Message Signaled Interrupts (MSI) functionality (1 = enable, 0 = disable). The default is -1 (auto, enabled).
 */
MODULE_PARM_DESC(msi, "MSI support (1 = enable, 0 = disable, -1 = auto)");
module_param_named(msi, gsgpu_msi, int, 0444);

/**
 * DOC: lockup_timeout (int)
 * Set GPU scheduler timeout value in ms. Value 0 is invalidated, will be adjusted to 10000.
 * Negative values mean 'infinite timeout' (MAX_JIFFY_OFFSET). The default is 10000.
 */
MODULE_PARM_DESC(lockup_timeout, "GPU lockup timeout in ms > 0 (default 10000)");
module_param_named(lockup_timeout, gsgpu_lockup_timeout, int, 0444);

/**
 * DOC: runpm (int)
 * Override for runtime power management control for dGPUs in PX/HG laptops. The gsgpu driver can dynamically power down
 * the dGPU on PX/HG laptops when it is idle. The default is -1 (auto enable). Setting the value to 0 disables this functionality.
 */
MODULE_PARM_DESC(runpm, "PX runtime pm (1 = force enable, 0 = disable, -1 = PX only default)");
module_param_named(runpm, gsgpu_runtime_pm, int, 0444);

/**
 * DOC: vm_size (int)
 * Override the size of the GPU's per client virtual address space in GiB.  The default is -1 (automatic for each asic).
 */
MODULE_PARM_DESC(vm_size, "VM address space size in gigabytes (default 64GB)");
module_param_named(vm_size, gsgpu_vm_size, int, 0444);

/**
 * DOC: vm_block_size (int)
 * Override VM page table size in bits (default depending on vm_size and hw setup). The default is -1 (automatic for each asic).
 */
MODULE_PARM_DESC(vm_block_size, "VM page table size in bits (default depending on vm_size)");
module_param_named(vm_block_size, gsgpu_vm_block_size, int, 0444);

/**
 * DOC: vm_fault_stop (int)
 * Stop on VM fault for debugging (0 = never, 1 = print first, 2 = always). The default is 0 (No stop).
 */
MODULE_PARM_DESC(vm_fault_stop, "Stop on VM fault (0 = never (default), 1 = print first, 2 = always)");
module_param_named(vm_fault_stop, gsgpu_vm_fault_stop, int, 0444);

/**
 * DOC: vm_debug (int)
 * Debug VM handling (0 = disabled, 1 = enabled). The default is 0 (Disabled).
 */
MODULE_PARM_DESC(vm_debug, "Debug VM handling (0 = disabled (default), 1 = enabled)");
module_param_named(vm_debug, gsgpu_vm_debug, int, 0644);

/**
 * DOC: vm_update_mode (int)
 * Override VM update mode. VM updated by using CPU (0 = never, 1 = Graphics only, 2 = Compute only, 3 = Both). The default
 * is -1 (Only in large BAR(LB) systems Compute VM tables will be updated by CPU, otherwise 0, never).
 */
MODULE_PARM_DESC(vm_update_mode, "VM update using CPU (0 = never (default except for large BAR(LB)), 1 = Graphics only, 2 = Compute only (default for LB), 3 = Both");
module_param_named(vm_update_mode, gsgpu_vm_update_mode, int, 0444);

/**
 * DOC: vram_page_split (int)
 * Override the number of pages after we split VRAM allocations (default 1024, -1 = disable). The default is 1024.
 */
MODULE_PARM_DESC(vram_page_split, "Number of pages after we split VRAM allocations (default 1024, -1 = disable)");
module_param_named(vram_page_split, gsgpu_vram_page_split, int, 0444);

/**
 * DOC: exp_hw_support (int)
 * Enable experimental hw support (1 = enable). The default is 0 (disabled).
 */
MODULE_PARM_DESC(exp_hw_support, "experimental hw support (1 = enable, 0 = disable (default))");
module_param_named(exp_hw_support, gsgpu_exp_hw_support, int, 0444);

/**
 * DOC: sched_jobs (int)
 * Override the max number of jobs supported in the sw queue. The default is 32.
 */
MODULE_PARM_DESC(sched_jobs, "the max number of jobs supported in the sw queue (default 32)");
module_param_named(sched_jobs, gsgpu_sched_jobs, int, 0444);

/**
 * DOC: sched_hw_submission (int)
 * Override the max number of HW submissions. The default is 2.
 */
MODULE_PARM_DESC(sched_hw_submission, "the max number of HW submissions (default 2)");
module_param_named(sched_hw_submission, gsgpu_sched_hw_submission, int, 0444);

/**
 * DOC: job_hang_limit (int)
 * Set how much time allow a job hang and not drop it. The default is 0.
 */
MODULE_PARM_DESC(job_hang_limit, "how much time allow a job hang and not drop it (default 0)");
module_param_named(job_hang_limit, gsgpu_job_hang_limit, int ,0444);

/**
 * DOC: gpu_recovery (int)
 * Set to enable GPU recovery mechanism (1 = enable, 0 = disable). The default is -1 (auto, disabled except SRIOV).
 */
MODULE_PARM_DESC(gpu_recovery, "Enable GPU recovery mechanism, (1 = enable, 0 = disable, -1 = auto)");
module_param_named(gpu_recovery, gsgpu_gpu_recovery, int, 0444);

MODULE_PARM_DESC(gsgpu_using_ram,"Gpu uses memory instead vram"
		 "0: using vram for gpu 1:use system for gpu");
module_param_named(gsgpu_using_ram, gsgpu_using_ram, uint, 0444);


static const struct pci_device_id pciidlist[] = {
	{0x0014, 0x7A25, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CHIP_LG100}, //GSGPU
	{0, 0, 0}
};

MODULE_DEVICE_TABLE(pci, pciidlist);

static struct drm_driver kms_driver;

static int gsgpu_kick_out_firmware_fb(struct pci_dev *pdev)
{
	struct apertures_struct *ap;
	bool primary = false;

	ap = alloc_apertures(1);
	if (!ap)
		return -ENOMEM;

	ap->ranges[0].base = pci_resource_start(pdev, 0);
	ap->ranges[0].size = pci_resource_len(pdev, 0);

#ifdef CONFIG_X86
	primary = pdev->resource[PCI_ROM_RESOURCE].flags & IORESOURCE_ROM_SHADOW;
#endif
	drm_fb_helper_remove_conflicting_framebuffers(ap, "gsgpudrmfb", primary);
	kfree(ap);

	return 0;
}

static int gsgpu_pci_probe(struct pci_dev *pdev,
			    const struct pci_device_id *ent)
{
	struct drm_device *dev;
	unsigned long flags = ent->driver_data;
	int ret, retry = 0;

	if ((flags & GSGPU_EXP_HW_SUPPORT) && !gsgpu_exp_hw_support) {
		DRM_INFO("This hardware requires experimental hardware support.\n"
			 "See modparam exp_hw_support\n");
		return -ENODEV;
	}

	if (!gsgpu_lg100_support)
		return -ENODEV;

	/* Get rid of things like offb */
	ret = gsgpu_kick_out_firmware_fb(pdev);
	if (ret)
		return ret;

	dev = drm_dev_alloc(&kms_driver, &pdev->dev);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	ret = pci_enable_device(pdev);
	if (ret)
		goto err_free;

	dev->pdev = pdev;

	pci_set_drvdata(pdev, dev);

retry_init:
	ret = drm_dev_register(dev, ent->driver_data);
	if (ret == -EAGAIN && ++retry <= 3) {
		DRM_INFO("retry init %d\n", retry);
		/* Don't request EX mode too frequently which is attacking */
		msleep(5000);
		goto retry_init;
	} else if (ret)
		goto err_pci;

	return 0;

err_pci:
	pci_disable_device(pdev);
err_free:
	drm_dev_put(dev);
	return ret;
}

static void
gsgpu_pci_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

	drm_dev_unregister(dev);
	drm_dev_put(dev);
	pci_disable_device(pdev);
	pci_set_drvdata(pdev, NULL);
}

static void
gsgpu_pci_shutdown(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct gsgpu_device *adev = dev->dev_private;

	/* if we are running in a VM, make sure the device
	 * torn down properly on reboot/shutdown.
	 * unfortunately we can't detect certain
	 * hypervisors so just do this all the time.
	 */
	gsgpu_device_ip_suspend(adev);
}

static int gsgpu_pmops_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	return gsgpu_device_suspend(drm_dev, true, true);
}

static int gsgpu_pmops_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return gsgpu_device_resume(drm_dev, true, true);
}

static int gsgpu_pmops_freeze(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	return gsgpu_device_suspend(drm_dev, false, true);
}

static int gsgpu_pmops_thaw(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	return gsgpu_device_resume(drm_dev, false, true);
}

static int gsgpu_pmops_poweroff(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	return gsgpu_device_suspend(drm_dev, true, true);
}

static int gsgpu_pmops_restore(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	return gsgpu_device_resume(drm_dev, false, true);
}

static int gsgpu_pmops_runtime_suspend(struct device *dev)
{
	pm_runtime_forbid(dev);
	return -EBUSY;
}

static int gsgpu_pmops_runtime_resume(struct device *dev)
{
	return -EINVAL;
}

static int gsgpu_pmops_runtime_idle(struct device *dev)
{
	pm_runtime_forbid(dev);
	return -EBUSY;
}

long gsgpu_drm_ioctl(struct file *filp,
		      unsigned int cmd, unsigned long arg)
{
	struct drm_file *file_priv = filp->private_data;
	struct drm_device *dev;
	long ret;
	dev = file_priv->minor->dev;
	ret = pm_runtime_get_sync(dev->dev);
	if (ret < 0)
		return ret;

	ret = drm_ioctl(filp, cmd, arg);

	pm_runtime_mark_last_busy(dev->dev);
	pm_runtime_put_autosuspend(dev->dev);
	return ret;
}

/**
 * loongson_vga_pci_devices  -- pci device id info
 *
 * __u32 vendor, device            Vendor and device ID or PCI_ANY_ID
 * __u32 subvendor, subdevice     Subsystem ID's or PCI_ANY_ID
 * __u32 class, class_mask        (class,subclass,prog-if) triplet
 * kernel_ulong_t driver_data     Data private to the driver
 */
static struct pci_device_id loongson_vga_pci_devices[] = {
	{PCI_DEVICE(PCI_VENDOR_ID_LOONGSON, 0x7a36)},
	{0, 0, 0, 0, 0, 0, 0},
};

struct pci_dev *loongson_dc_pdev;
EXPORT_SYMBOL(loongson_dc_pdev);

/**
 * loongson_vga_pci_register -- add pci device
 *
 * @pdev PCI device
 * @ent pci device id
 */
static int loongson_vga_pci_register(struct pci_dev *pdev,
				 const struct pci_device_id *ent)

{
	int ret;

	ret = pci_enable_device(pdev);
	loongson_dc_pdev = pdev;

	return ret;
}

/**
 * loongson_vga_pci_unregister -- release drm device
 *
 * @pdev PCI device
 */
static void loongson_vga_pci_unregister(struct pci_dev *pdev)
{
	pci_disable_device(pdev);
}

static const struct dev_pm_ops gsgpu_pm_ops = {
	.suspend = gsgpu_pmops_suspend,
	.resume = gsgpu_pmops_resume,
	.freeze = gsgpu_pmops_freeze,
	.thaw = gsgpu_pmops_thaw,
	.poweroff = gsgpu_pmops_poweroff,
	.restore = gsgpu_pmops_restore,
	.runtime_suspend = gsgpu_pmops_runtime_suspend,
	.runtime_resume = gsgpu_pmops_runtime_resume,
	.runtime_idle = gsgpu_pmops_runtime_idle,
};

static int gsgpu_flush(struct file *f, fl_owner_t id)
{
	struct drm_file *file_priv = f->private_data;
	struct gsgpu_fpriv *fpriv = file_priv->driver_priv;

	gsgpu_ctx_mgr_entity_flush(&fpriv->ctx_mgr);

	return 0;
}

static const struct file_operations gsgpu_driver_kms_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.flush = gsgpu_flush,
	.release = drm_release,
	.unlocked_ioctl = gsgpu_drm_ioctl,
	.mmap = gsgpu_mmap,
	.poll = drm_poll,
	.read = drm_read,
#ifdef CONFIG_COMPAT
	.compat_ioctl = gsgpu_kms_compat_ioctl,
#endif
};

static bool
gsgpu_get_crtc_scanout_position(struct drm_device *dev, unsigned int pipe,
				 bool in_vblank_irq, int *vpos, int *hpos,
				 ktime_t *stime, ktime_t *etime,
				 const struct drm_display_mode *mode)
{
	return gsgpu_display_get_crtc_scanoutpos(dev, pipe, 0, vpos, hpos,
						  stime, etime, mode);
}

static struct drm_driver kms_driver = {
	.driver_features = DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED | DRIVER_GEM |
		DRIVER_PRIME |  DRIVER_MODESET | DRIVER_SYNCOBJ
		|DRIVER_RENDER | DRIVER_ATOMIC,
	.load = gsgpu_driver_load_kms,
	.open = gsgpu_driver_open_kms,
	.postclose = gsgpu_driver_postclose_kms,
	.lastclose = gsgpu_driver_lastclose_kms,
	.unload = gsgpu_driver_unload_kms,
	.get_vblank_counter = gsgpu_get_vblank_counter_kms,
	.get_vblank_timestamp = drm_calc_vbltimestamp_from_scanoutpos,
	.get_scanout_position = gsgpu_get_crtc_scanout_position,
	.irq_handler = gsgpu_irq_handler,
	.ioctls = gsgpu_ioctls_kms,
	.gem_free_object_unlocked = gsgpu_gem_object_free,
	.gem_open_object = gsgpu_gem_object_open,
	.gem_close_object = gsgpu_gem_object_close,
	.dumb_create = gsgpu_mode_dumb_create,
	.dumb_map_offset = gsgpu_mode_dumb_mmap,
	.fops = &gsgpu_driver_kms_fops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_export = gsgpu_gem_prime_export,
	.gem_prime_import = gsgpu_gem_prime_import,
	.gem_prime_res_obj = gsgpu_gem_prime_res_obj,
	.gem_prime_get_sg_table = gsgpu_gem_prime_get_sg_table,
	.gem_prime_import_sg_table = gsgpu_gem_prime_import_sg_table,
	.gem_prime_vmap = gsgpu_gem_prime_vmap,
	.gem_prime_vunmap = gsgpu_gem_prime_vunmap,
	.gem_prime_mmap = gsgpu_gem_prime_mmap,

	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = KMS_DRIVER_MAJOR,
	.minor = KMS_DRIVER_MINOR,
	.patchlevel = KMS_DRIVER_PATCHLEVEL,
};

static struct drm_driver *driver;
static struct pci_driver *pdriver;
static struct pci_driver *loongson_dc_pdriver;

static struct pci_driver gsgpu_kms_pci_driver = {
	.name = DRIVER_NAME,
	.id_table = pciidlist,
	.probe = gsgpu_pci_probe,
	.remove = gsgpu_pci_remove,
	.shutdown = gsgpu_pci_shutdown,
	.driver.pm = &gsgpu_pm_ops,
};

/**
 * loongson_vga_pci_driver -- pci driver structure
 *
 * .id_table : must be non-NULL for probe to be called
 * .probe: New device inserted
 * .remove: Device removed
 * .resume: Device suspended
 * .suspend: Device woken up
 */
static struct pci_driver loongson_vga_pci_driver = {
	.name = "gsgpu-dc",
	.id_table = loongson_vga_pci_devices,
	.probe = loongson_vga_pci_register,
	.remove = loongson_vga_pci_unregister,
};

static int __init gsgpu_init(void)
{
	struct pci_dev *pdev = NULL;
	int r;

	if (vgacon_text_force()) {
		DRM_ERROR("VGACON disables gsgpu kernel modesetting.\n");
		return -EINVAL;
	}

	/* Prefer discrete card if present */
	while ((pdev = pci_get_class(PCI_CLASS_DISPLAY_VGA << 8, pdev))) {
		if (pdev->vendor != PCI_VENDOR_ID_LOONGSON)
			return 0;
	}

	r = gsgpu_sync_init();
	if (r)
		goto error_sync;

	r = gsgpu_fence_slab_init();
	if (r)
		goto error_fence;

	DRM_INFO("gsgpu kernel modesetting enabled.\n");
	driver = &kms_driver;
	pdriver = &gsgpu_kms_pci_driver;
	loongson_dc_pdriver = &loongson_vga_pci_driver;
	driver->num_ioctls = gsgpu_max_kms_ioctl;

	r = pci_register_driver(loongson_dc_pdriver);
	if (r) {
		goto error_sync;
	}

	r = pci_register_driver(pdriver);
	if (r) {
		goto error_sync;
	}

	return 0;

error_fence:
	gsgpu_sync_fini();

error_sync:
	return r;
}

static void __exit gsgpu_exit(void)
{
	pci_unregister_driver(pdriver);
	pci_unregister_driver(loongson_dc_pdriver);
	gsgpu_sync_fini();
	gsgpu_fence_slab_fini();
}

module_init(gsgpu_init);
module_exit(gsgpu_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL and additional rights");
