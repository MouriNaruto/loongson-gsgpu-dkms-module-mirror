
DKMS_INCLUDE_PREFIX = \
	-I$(src)/include \
	-I$(src)/include/drm \
	-I$(src)/include/uapi \

export DKMS_INCLUDE_PREFIX

export CONFIG_DRM_TTM=m
export CONFIG_DRM_GSGPU=m
export CONFIG_CHASH=m
export CONFIG_DRM_SCHED=m
#export BUILD_AS_DKMS=y
export CONFIG_DRM_GSGPU_USERPTR=y

subdir-ccflags-y += -DBUILD_AS_DKMS
subdir-ccflags-y += -DCONFIG_DRM_GSGPU_USERPTR

obj-m += gsgpu/lib/ scheduler/ gsgpu/gpu/ ttm/ 
