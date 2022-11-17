#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2d393864, "module_layout" },
	{ 0x42595e58, "vgacon_text_force" },
	{ 0x8487a2b6, "flush_work" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x94495a75, "drm_gem_prime_export" },
	{ 0x8c2226d7, "pci_resize_resource" },
	{ 0x87c27ff7, "kmem_cache_destroy" },
	{ 0x7d4e2561, "drm_release" },
	{ 0xd4553211, "drm_prime_gem_destroy" },
	{ 0xe5b1b880, "sync_file_create" },
	{ 0x5b6dd389, "drm_fb_helper_set_par" },
	{ 0x5cc15b30, "drm_fb_helper_ioctl" },
	{ 0xe119e6eb, "kmalloc_caches" },
	{ 0x5a02f909, "ttm_page_alloc_debugfs" },
	{ 0xcf3f86a9, "ttm_bo_eviction_valuable" },
	{ 0xa19670df, "pci_write_config_dword" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x8b97ae6d, "loongson_dma_ops" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x757f89e2, "ttm_fbdev_mmap" },
	{ 0xf9a482f9, "msleep" },
	{ 0x9b53e14, "interval_tree_remove" },
	{ 0xd5801ddc, "drm_syncobj_create" },
	{ 0x463b5151, "drm_dev_register" },
	{ 0x9f71e336, "drm_atomic_helper_suspend" },
	{ 0xceddeecd, "dma_release_from_dev_coherent" },
	{ 0xa9e362d0, "up_read" },
	{ 0x2c422fe4, "trace_handle_return" },
	{ 0xc4076d0e, "drm_get_edid" },
	{ 0x410f4965, "drm_atomic_helper_update_plane" },
	{ 0xdea0d5de, "drm_atomic_helper_crtc_duplicate_state" },
	{ 0x4ff6e8e0, "pci_write_config_word" },
	{ 0xafa4c93e, "drm_fb_helper_cfb_fillrect" },
	{ 0x14af37d0, "drm_calc_vbltimestamp_from_scanoutpos" },
	{ 0xc8aa0043, "ttm_bo_move_memcpy" },
	{ 0x3a04401c, "drm_dev_alloc" },
	{ 0x1b918fa3, "drm_fb_helper_initial_config" },
	{ 0x7aa1756e, "kvfree" },
	{ 0xacd0a24f, "param_ops_int" },
	{ 0x74b9187b, "ttm_bo_put" },
	{ 0x98cf60b3, "strlen" },
	{ 0x37da7abb, "pci_read_config_byte" },
	{ 0xe123f3d9, "dma_fence_release" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0x29896055, "drm_atomic_helper_resume" },
	{ 0x7a16c41b, "drm_gem_dmabuf_release" },
	{ 0x5a2bda1, "drm_mode_config_cleanup" },
	{ 0x4d8512d9, "reservation_object_wait_timeout_rcu" },
	{ 0x9ad2a0f3, "__pm_runtime_suspend" },
	{ 0x495a206c, "drm_mode_config_reset" },
	{ 0xad73041f, "autoremove_wake_function" },
	{ 0xd88d1c3d, "drm_atomic_helper_commit_hw_done" },
	{ 0xf6b7ba04, "drm_gem_fb_create_handle" },
	{ 0x94388751, "ttm_bo_move_to_lru_tail" },
	{ 0xfdea4690, "drm_syncobj_get_handle" },
	{ 0xd9b2ad73, "drm_set_preferred_mode" },
	{ 0x9f224177, "drm_atomic_helper_page_flip" },
	{ 0xa34c5449, "ttm_sg_tt_init" },
	{ 0xdab5a1eb, "interval_tree_insert" },
	{ 0x4e68e9be, "rb_next_postorder" },
	{ 0x3477f884, "pci_disable_device" },
	{ 0x7bd4fb03, "ttm_bo_validate" },
	{ 0xffae8e8b, "nsecs_to_jiffies" },
	{ 0xcf92acae, "drm_fb_helper_debug_leave" },
	{ 0xca3c7ba8, "__chash_table_copy_out" },
	{ 0x990d625a, "drm_sched_dependency_optimized" },
	{ 0x3f767388, "ttm_dma_unpopulate" },
	{ 0xd7204ff6, "drm_gem_map_dma_buf" },
	{ 0xeb48bf7b, "drm_gem_dmabuf_kmap" },
	{ 0x20000329, "simple_strtoul" },
	{ 0xd9b0f418, "pci_bus_resource_n" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xbfb7739b, "drm_syncobj_get_fd" },
	{ 0x89852ba8, "drm_helper_probe_single_connector_modes" },
	{ 0xd55b79c7, "seq_printf" },
	{ 0xad27f361, "__warn_printk" },
	{ 0xfd9e885c, "drm_crtc_vblank_off" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0x5504f82d, "__mmu_notifier_register" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x602b2c9d, "drm_atomic_helper_wait_for_flip_done" },
	{ 0x6112a75a, "radix_tree_tagged" },
	{ 0x5d7318dc, "drm_global_item_ref" },
	{ 0xee91879b, "rb_first_postorder" },
	{ 0xe29f4156, "pci_get_class" },
	{ 0xfc04c3fe, "seq_write" },
	{ 0x448eac3e, "kmemdup" },
	{ 0x5b2f27fb, "do_wait_intr" },
	{ 0xa843805a, "get_unused_fd_flags" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xe7b46cef, "ttm_bo_unlock_delayed_workqueue" },
	{ 0xdc147c42, "drm_kms_helper_hotplug_event" },
	{ 0xf6c1256d, "cancel_delayed_work_sync" },
	{ 0xcb4c64fd, "down_write_killable" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x85df9b6c, "strsep" },
	{ 0xe44853f4, "__pm_runtime_resume" },
	{ 0x2cbf2a69, "trace_event_buffer_reserve" },
	{ 0x999e8297, "vfree" },
	{ 0xad5b58aa, "drm_printf" },
	{ 0xdd727c36, "drm_vblank_init" },
	{ 0xd3d00be0, "drm_encoder_init" },
	{ 0x563796cb, "ttm_bo_lock_delayed_workqueue" },
	{ 0x80f6a1d9, "debugfs_create_file" },
	{ 0x36591b7e, "pm_runtime_forbid" },
	{ 0x16cdc340, "acpi_get_table" },
	{ 0x67225bc9, "__drm_atomic_helper_connector_duplicate_state" },
	{ 0x672fff92, "idr_for_each" },
	{ 0x91715312, "sprintf" },
	{ 0x4d63a88d, "bpf_trace_run3" },
	{ 0xa8976730, "drm_sched_hw_job_reset" },
	{ 0x40c7247c, "si_meminfo" },
	{ 0xcfd31a16, "drm_atomic_helper_crtc_reset" },
	{ 0x65d358f4, "drm_gem_unmap_dma_buf" },
	{ 0xaf09b8ce, "drm_universal_plane_init" },
	{ 0x21a38557, "set_page_dirty" },
	{ 0xab402648, "dma_fence_wait_any_timeout" },
	{ 0xce90062e, "refcount_inc_not_zero_checked" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xe4607073, "drm_connector_cleanup" },
	{ 0x156cbb27, "i2c_add_adapter" },
	{ 0xb3b42a38, "mutex_trylock" },
	{ 0x90ddd8f7, "down_read" },
	{ 0x259d9619, "drm_fb_helper_unregister_fbi" },
	{ 0x87595f9b, "reservation_object_add_shared_fence" },
	{ 0x404b92d0, "ttm_bo_init_reserved" },
	{ 0x35afae24, "__drm_puts_seq_file" },
	{ 0xc392746b, "dma_alloc_from_dev_coherent" },
	{ 0xdf3a9ff3, "to_drm_sched_fence" },
	{ 0xe526f136, "__copy_user" },
	{ 0x214b0f92, "ttm_dma_populate" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x6263ce11, "drm_fb_helper_pan_display" },
	{ 0x39c147db, "drm_atomic_helper_update_legacy_modeset_state" },
	{ 0xf4bd2b60, "ttm_mem_global_init" },
	{ 0xf96f051b, "drm_atomic_helper_set_config" },
	{ 0x1fcc0e39, "drm_handle_vblank" },
	{ 0x6e6b7e68, "__get_task_comm" },
	{ 0x4d4e7cee, "pci_set_master" },
	{ 0xe02b98e4, "drm_connector_update_edid_property" },
	{ 0x1a411479, "drm_syncobj_free" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x3b8e576f, "drm_dev_put" },
	{ 0x6fab8912, "trace_define_field" },
	{ 0x7b982195, "drm_mm_init" },
	{ 0x5924cdcd, "_dev_warn" },
	{ 0xfb578fc5, "memset" },
	{ 0xf2bb84fe, "ttm_mem_global_release" },
	{ 0x1fa99e93, "drm_irq_install" },
	{ 0xb9056bb6, "remove_conflicting_framebuffers" },
	{ 0x73a3028d, "drm_fb_helper_cfb_copyarea" },
	{ 0xef198359, "ttm_dma_page_alloc_debugfs" },
	{ 0xa9ed5689, "default_llseek" },
	{ 0x3d85d98, "idr_destroy" },
	{ 0xa45bf54e, "bpf_trace_run1" },
	{ 0x226b8e9f, "pci_restore_state" },
	{ 0xa0ce4a07, "drm_dev_unregister" },
	{ 0x76ab0037, "drm_send_event_locked" },
	{ 0x6c96cd0a, "drm_prime_pages_to_sg" },
	{ 0xbda11858, "ttm_bo_add_to_lru" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0x4a928de6, "mutex_lock_interruptible" },
	{ 0xd2e4cdc4, "drm_gem_map_attach" },
	{ 0xc5e9aa1f, "dma_fence_init" },
	{ 0x935982fe, "drm_crtc_vblank_put" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x45e1ba9, "drm_helper_mode_fill_fb_struct" },
	{ 0x6b2ab817, "drm_sched_entity_fini" },
	{ 0x7c32d0f0, "printk" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x7ef1602, "drm_helper_probe_detect" },
	{ 0x191bfe9a, "drm_crtc_init_with_planes" },
	{ 0x470eafed, "pci_assign_unassigned_bus_resources" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x7bcf5f59, "__cpu_online_mask" },
	{ 0x9ba7ea4f, "drm_atomic_helper_plane_reset" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0x54fbd8c8, "ttm_unmap_and_unpopulate_pages" },
	{ 0xda07145, "drm_vma_node_allow" },
	{ 0x9725e48c, "drm_gem_handle_create" },
	{ 0xf8510198, "drm_gem_object_put_unlocked" },
	{ 0x479c3c86, "find_next_zero_bit" },
	{ 0x8a1a3d33, "drm_crtc_vblank_on" },
	{ 0xadf4146a, "ida_free" },
	{ 0x320cbd70, "ttm_bo_kmap" },
	{ 0x662d7948, "drm_fb_helper_fill_var" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xe6aea7b8, "drm_sched_fini" },
	{ 0x2455c156, "__clear_user" },
	{ 0x67549fed, "trace_event_reg" },
	{ 0xf5123fef, "ttm_bo_evict_mm" },
	{ 0x2054f941, "ww_mutex_lock" },
	{ 0x830485a6, "drm_sched_init" },
	{ 0xb145eb4a, "pci_read_config_word" },
	{ 0x1a3d1e9d, "debugfs_remove" },
	{ 0x41c55d21, "kmem_cache_free" },
	{ 0x5e51cd74, "swiotlb_nr_tbl" },
	{ 0xbb1c979c, "ttm_eu_fence_buffer_objects" },
	{ 0xa1716baf, "__rb_insert_augmented" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xe73827d3, "vga_client_register" },
	{ 0x2bb92958, "mem_section" },
	{ 0xf3285f13, "drm_gem_object_release" },
	{ 0x68cae13, "bpf_trace_run5" },
	{ 0x57e38b5c, "ttm_tt_bind" },
	{ 0xdecd5317, "drm_read" },
	{ 0x5c1ef44d, "ttm_bo_dma_acc_size" },
	{ 0xd96babb4, "interval_tree_iter_next" },
	{ 0xb0fa0d15, "dma_fence_signal" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x17cff04f, "__drm_atomic_helper_connector_destroy_state" },
	{ 0x6558ef13, "drm_add_edid_modes" },
	{ 0xf47b3ff1, "__drm_atomic_helper_connector_reset" },
	{ 0xedc06d37, "refcount_dec_and_test_checked" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x2773c485, "__wake_up_locked" },
	{ 0xbf4d5e82, "pid_task" },
	{ 0x6b5c234, "ttm_bo_init_mm" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x7d79ea60, "idr_alloc" },
	{ 0xf09517f7, "ttm_bo_device_init" },
	{ 0x14605535, "dma_fence_context_alloc" },
	{ 0x59d2a508, "up_write" },
	{ 0xed536c64, "hdmi_avi_infoframe_pack" },
	{ 0xe07a332c, "_dev_err" },
	{ 0xfce12ed9, "down_write" },
	{ 0x6fb23ca9, "fput" },
	{ 0xb6e53f07, "drm_debugfs_create_files" },
	{ 0x8659e7ee, "perf_trace_run_bpf_submit" },
	{ 0xf5e32949, "pci_enable_msi" },
	{ 0x65e9e0b1, "release_pages" },
	{ 0xa2f8b6fb, "perf_trace_buf_alloc" },
	{ 0x1dd71f7a, "drm_err" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x3362d4b, "drm_connector_init" },
	{ 0xccb27a33, "drm_kms_helper_poll_enable" },
	{ 0x19f0f26c, "drm_fb_helper_prepare" },
	{ 0xa3aaeff8, "mark_page_accessed" },
	{ 0xfe6e59fa, "idr_remove" },
	{ 0x3949b32, "device_create_file" },
	{ 0x1efbed29, "ttm_bo_mem_space" },
	{ 0xc83311fc, "iommu_get_domain_for_dev" },
	{ 0xc5d1dd00, "drm_sched_entity_destroy" },
	{ 0x90efc314, "ttm_eu_backoff_reservation" },
	{ 0xdbfb4b97, "kthread_unpark" },
	{ 0xc5bc25de, "kvmalloc_node" },
	{ 0x6843be5a, "drm_gem_prime_fd_to_handle" },
	{ 0xd1712c48, "find_vma" },
	{ 0x2ef424cc, "drm_gem_dmabuf_vunmap" },
	{ 0x28514aa6, "drm_fb_helper_set_suspend_unlocked" },
	{ 0xc6cbbc89, "capable" },
	{ 0xd53899e9, "drm_plane_cleanup" },
	{ 0x3a32e9dd, "drm_ioctl" },
	{ 0x56da07e8, "drm_fb_helper_output_poll_changed" },
	{ 0x8f2d19b4, "drm_gem_prime_import" },
	{ 0x5bd937bf, "drm_property_create_enum" },
	{ 0x97a27130, "_dev_info" },
	{ 0x40a9b349, "vzalloc" },
	{ 0x1b4dbc87, "drm_gem_map_detach" },
	{ 0x996aae40, "kmem_cache_alloc" },
	{ 0xbab7b6f2, "drm_atomic_helper_plane_duplicate_state" },
	{ 0x19ae9bd2, "ttm_bo_mem_put" },
	{ 0x9df03b9d, "idr_get_next" },
	{ 0x6b581f7a, "drm_syncobj_replace_fence" },
	{ 0x9d27a60, "mmu_notifier_unregister_no_release" },
	{ 0x1c8837a2, "pci_release_resource" },
	{ 0x12a38747, "usleep_range" },
	{ 0xe7ce72ab, "drm_atomic_helper_connector_destroy_state" },
	{ 0x173a4d20, "drm_mode_create_scaling_mode_property" },
	{ 0x90227e2d, "dma_fence_array_create" },
	{ 0x3bfec4d, "trace_event_ignore_this_pid" },
	{ 0xaed5dc3f, "drm_atomic_helper_check" },
	{ 0xdebc716e, "drm_framebuffer_unregister_private" },
	{ 0x2a50d617, "ttm_bo_del_sub_from_lru" },
	{ 0x9b78c7e6, "drm_atomic_helper_commit" },
	{ 0x164e7c53, "queue_delayed_work_on" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x3b26f51, "get_user_pages" },
	{ 0xb3740669, "drm_fb_helper_check_var" },
	{ 0x4e2ca257, "drm_gem_private_object_init" },
	{ 0x44f1ade, "drm_fb_helper_blank" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xa202a8e5, "kmalloc_order_trace" },
	{ 0xaf0ef43f, "ttm_bo_kunmap" },
	{ 0x7098a334, "drm_irq_uninstall" },
	{ 0xf850ee7c, "ttm_bo_pipeline_move" },
	{ 0xecebac16, "flush_delayed_work" },
	{ 0xf21d40b, "drm_crtc_accurate_vblank_count" },
	{ 0x3b840edb, "drm_connector_attach_encoder" },
	{ 0x515c5fc, "drm_crtc_send_vblank_event" },
	{ 0x6439cb6d, "drm_gem_object_lookup" },
	{ 0x385e7789, "drm_crtc_vblank_get" },
	{ 0x6e0bbabe, "drm_mm_print" },
	{ 0x8187325d, "trace_event_buffer_commit" },
	{ 0x7f24de73, "jiffies_to_usecs" },
	{ 0xd6c0a99c, "get_user_pages_remote" },
	{ 0x3f4547a7, "put_unused_fd" },
	{ 0x5c9ebf54, "drm_add_modes_noedid" },
	{ 0xd14f4ea7, "drm_format_plane_cpp" },
	{ 0xe4ef3ef2, "dma_fence_wait_timeout" },
	{ 0x364257dd, "ttm_eu_reserve_buffers" },
	{ 0xd4d36571, "pci_unregister_driver" },
	{ 0xa44a1307, "interval_tree_iter_first" },
	{ 0x484715ff, "kmem_cache_alloc_trace" },
	{ 0xf3faa55a, "drm_fb_helper_alloc_fbi" },
	{ 0xf74333d2, "__chash_table_copy_in" },
	{ 0x7219601a, "reservation_object_add_excl_fence" },
	{ 0x45a83260, "__dynamic_dev_dbg" },
	{ 0x147da8a5, "kthread_park" },
	{ 0xffeabc5, "drm_gem_prime_handle_to_fd" },
	{ 0x333a16cf, "kmem_cache_create" },
	{ 0xd7e979a6, "drm_sched_job_init" },
	{ 0x354fc611, "drm_gem_dmabuf_vmap" },
	{ 0x15d8aa41, "__drm_printfn_seq_file" },
	{ 0x8ee32c83, "dma_fence_add_callback" },
	{ 0xeaf00b3e, "event_triggers_call" },
	{ 0xa68f184a, "drm_connector_unregister" },
	{ 0x1c6bda19, "drm_vma_node_revoke" },
	{ 0x8442cc72, "drm_encoder_cleanup" },
	{ 0x6e36097e, "pci_set_power_state" },
	{ 0xdb9c937, "bpf_trace_run2" },
	{ 0xd362a882, "ttm_bo_device_release" },
	{ 0x15969d07, "drm_gem_dmabuf_kunmap" },
	{ 0x732e396, "drm_crtc_cleanup" },
	{ 0x60178777, "i2c_bit_add_numbered_bus" },
	{ 0xa7437aea, "drm_sched_entity_flush" },
	{ 0xa3a507d5, "drm_atomic_helper_crtc_destroy_state" },
	{ 0x4d39998b, "drm_get_max_iomem" },
	{ 0x8ec2f174, "drm_mode_config_init" },
	{ 0x37a0cba, "kfree" },
	{ 0xb7ed6cfd, "ttm_dma_tt_fini" },
	{ 0x8033eda1, "drm_gem_dmabuf_mmap" },
	{ 0x5443b0b6, "drm_hdmi_avi_infoframe_from_display_mode" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x74507799, "trace_event_raw_init" },
	{ 0x643e0ce5, "call_rcu_sched" },
	{ 0xa026d55f, "fd_install" },
	{ 0xd94b5a6d, "pci_disable_msi" },
	{ 0x6128b5fc, "__printk_ratelimit" },
	{ 0xe7043306, "iounmap" },
	{ 0x59966cb6, "ww_mutex_unlock" },
	{ 0xeab1e19, "ttm_bo_move_ttm" },
	{ 0x66c2e4f7, "__put_task_struct" },
	{ 0xea46cda3, "fget" },
	{ 0x6f66ffa6, "drm_fb_helper_cfb_imageblit" },
	{ 0xbeada68, "__pci_register_driver" },
	{ 0x6078326d, "drm_mm_insert_node_in_range" },
	{ 0xa89857fa, "idr_replace" },
	{ 0x74d2e2c8, "drm_atomic_helper_cleanup_planes" },
	{ 0x6ce6be2c, "request_firmware" },
	{ 0x9d9932f8, "sg_alloc_table_from_pages" },
	{ 0x530a2f61, "drm_atomic_helper_shutdown" },
	{ 0xb068ee49, "drm_fb_helper_fini" },
	{ 0xc307a10c, "drm_mm_takedown" },
	{ 0x1b67631e, "iommu_iova_to_phys" },
	{ 0xca714c91, "drm_crtc_vblank_count" },
	{ 0xca9360b5, "rb_next" },
	{ 0xe6b05d8f, "loongson_sysconf" },
	{ 0x2109a78c, "drm_dbg" },
	{ 0x13817285, "sg_free_table" },
	{ 0x92559ec2, "drm_atomic_helper_disable_plane" },
	{ 0xa172406d, "ttm_populate_and_map_pages" },
	{ 0xd6d65562, "drm_prime_sg_to_page_addr_arrays" },
	{ 0x564e1aee, "ttm_bo_global_init" },
	{ 0xb79d7564, "trace_raw_output_prep" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x29361773, "complete" },
	{ 0x28318305, "snprintf" },
	{ 0xfe917f94, "drm_syncobj_find_fence" },
	{ 0xc6e06ce3, "trace_seq_printf" },
	{ 0x6380cbd5, "reservation_object_get_fences_rcu" },
	{ 0xde487289, "pci_iomap" },
	{ 0x6929920f, "drm_fb_helper_setcmap" },
	{ 0x1ee8d6d4, "refcount_inc_checked" },
	{ 0x62b9963e, "reservation_object_reserve_shared" },
	{ 0x9b81a01, "drm_fb_helper_lastclose" },
	{ 0x5f82a40c, "drm_framebuffer_init" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x2c287154, "drm_mm_remove_node" },
	{ 0x4ae6a1ab, "drm_vma_node_is_allowed" },
	{ 0x492f4e20, "drm_sched_entity_init" },
	{ 0x27d0414a, "pci_enable_device" },
	{ 0x4d1ff60a, "wait_for_completion_timeout" },
	{ 0xe0dcfe7, "drm_fb_helper_fill_fix" },
	{ 0xf802bf2a, "param_ops_uint" },
	{ 0x5282cede, "drm_connector_register" },
	{ 0x9ad03057, "idr_find" },
	{ 0x29e74266, "drm_sched_entity_set_priority" },
	{ 0x29f078d1, "drm_mode_legacy_fb_format" },
	{ 0x8cea3220, "drm_syncobj_find" },
	{ 0x75aef43a, "i2c_new_device" },
	{ 0x7a0cb8d, "ttm_bo_clean_mm" },
	{ 0x551bd071, "__rb_erase_color" },
	{ 0xbda00400, "drm_kms_helper_poll_disable" },
	{ 0x4e8dd264, "ttm_bo_global_release" },
	{ 0xcb891374, "drm_mode_object_find" },
	{ 0x7989e9a9, "release_firmware" },
	{ 0x407f12, "__init_rwsem" },
	{ 0x4d2f5ede, "ww_mutex_lock_interruptible" },
	{ 0x592778ad, "drm_fb_helper_debug_enter" },
	{ 0x6ee9c1cb, "drm_poll" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xbdb514c2, "drm_sched_entity_push_job" },
	{ 0xb3c766d8, "__put_page" },
	{ 0xf5f13262, "drm_fb_helper_init" },
	{ 0xe9ca5ad7, "drm_fb_helper_single_add_all_connectors" },
	{ 0xa14ab2f3, "ida_alloc_range" },
	{ 0xeadf4aee, "drm_is_current_master" },
	{ 0xfbe90108, "ttm_tt_set_placement_caching" },
	{ 0xb4cea61f, "drm_open" },
	{ 0xf3dcabd1, "pci_save_state" },
	{ 0xe914e41e, "strcpy" },
	{ 0xb5540008, "drm_global_item_unref" },
	{ 0xf5849dcd, "drm_atomic_helper_plane_destroy_state" },
	{ 0xe57059ee, "drm_gem_fb_destroy" },
	{ 0x9816272d, "drm_framebuffer_cleanup" },
	{ 0xf4bfb3ff, "ttm_bo_mmap" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=chash,gpu-sched";

MODULE_ALIAS("pci:v00000014d00007A25sv*sd*bc*sc*i*");
