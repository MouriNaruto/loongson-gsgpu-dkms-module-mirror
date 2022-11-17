/*
 * Copyright 2008 Advanced Micro Devices, Inc.
 * Copyright 2008 Red Hat Inc.
 * Copyright 2009 Jerome Glisse.
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

/*
 * Debugfs
 */
struct gsgpu_debugfs {
	const struct drm_info_list	*files;
	unsigned		num_files;
};

int gsgpu_debugfs_regs_init(struct gsgpu_device *adev);
void gsgpu_debugfs_regs_cleanup(struct gsgpu_device *adev);
int gsgpu_debugfs_init(struct gsgpu_device *adev);
int gsgpu_debugfs_add_files(struct gsgpu_device *adev,
			     const struct drm_info_list *files,
			     unsigned nfiles);
int gsgpu_debugfs_fence_init(struct gsgpu_device *adev);
int gsgpu_debugfs_firmware_init(struct gsgpu_device *adev);
int gsgpu_debugfs_gem_init(struct gsgpu_device *adev);
int gsgpu_debugfs_sema_init(struct gsgpu_device *adev);
