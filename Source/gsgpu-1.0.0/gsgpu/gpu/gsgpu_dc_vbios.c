// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2020-2022 Loongson Technology Corporation Limited
 */

#include "gsgpu.h"
#include "gsgpu_dc_vbios.h"
#include "gsgpu_dc_resource.h"

#define VBIOS_START 0x1000
#define VBIOS_SIZE 0x400000
#define VBIOS_DESC_OFFSET 0x6000
#define VBIOS_DESC_TOTAL 0xA00

static bool read_bios_from_sysconf(struct gsgpu_dc *dc)
{
	if (!loongson_sysconf.vgabios_addr)
		return false;

	dc->vbios->vbios_ptr = kmalloc(VBIOS_SIZE, GFP_KERNEL);
	if (!dc->vbios->vbios_ptr)
		return false;

	memcpy(dc->vbios->vbios_ptr, (void *)loongson_sysconf.vgabios_addr, VBIOS_SIZE);
	DRM_INFO("GSGPU get vbios from sysconf Sucess \n");

	return true;
}

#ifdef CONFIG_ACPI
static bool read_bios_from_acpi(struct gsgpu_dc *dc)
{
	struct acpi_table_header *hdr;
	struct acpi_viat_table *viat;
	void *vaddr;
	acpi_size tbl_size;

	if (!ACPI_SUCCESS(acpi_get_table("VIAT", 1, &hdr)))
		return false;

	tbl_size = hdr->length;
	if (tbl_size != sizeof(struct acpi_viat_table)) {
		DRM_WARN("ACPI viat table present but broken (length error #1)\n");
		return false;
	}

	viat = (struct acpi_viat_table *)hdr;
	dc->vbios->vbios_ptr = kmalloc(VBIOS_SIZE, GFP_KERNEL);
	if (!dc->vbios->vbios_ptr)
		return false;

	vaddr = phys_to_virt(viat->vbios_addr);
	memcpy(dc->vbios->vbios_ptr, vaddr, VBIOS_SIZE);
	DRM_INFO("Get vbios from ACPI success!\n");
	return true;
}
#else
static bool read_bios_from_acpi(struct gsgpu_device *adev)
{
	return false;
}
#endif

static bool get_vbios_data(struct gsgpu_dc *dc)
{
	if (read_bios_from_acpi(dc))
                goto success;

	if (read_bios_from_sysconf(dc))
		goto success;

	DRM_ERROR("Unable to locate a BIOS ROM\n");
	return false;

success:
	return true;
}

static bool parse_vbios_header(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_header_resource)
		ret = vbios->funcs->create_header_resource(vbios, data, vb_desc->size);

	return ret;
}

static bool parse_vbios_crtc(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_crtc_resource)
		ret = vbios->funcs->create_crtc_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_connector(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_connecor_resource)
		ret = vbios->funcs->create_connecor_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_encoder(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_encoder_resource)
		ret = vbios->funcs->create_encoder_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_i2c(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_i2c_resource)
		ret = vbios->funcs->create_i2c_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_pwm(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_pwm_resource)
		ret = vbios->funcs->create_pwm_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_gpu(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	bool ret = false;
	u8 *data;

	if (IS_ERR_OR_NULL(vb_desc) || IS_ERR_OR_NULL(vbios))
		return ret;

	data = (u8*)vbios->vbios_ptr + vb_desc->offset;

	if (vbios->funcs && vbios->funcs->create_gpu_resource)
		ret = vbios->funcs->create_gpu_resource(vbios, data, vb_desc->link, vb_desc->size);

	return ret;
}

static bool parse_vbios_default(struct vbios_desc *vb_desc, struct gsgpu_vbios *vbios)
{
	DRM_ERROR("Current descriptor[T-%d][V-%d] cannot be interprete.\n",
		  vb_desc->type, vb_desc->ver);
	return false;
}

#define FUNC(t, v, f)                                                         \
	{                                                                     \
		.type = t, .ver = v, .func = f,                               \
	}

static struct desc_func tables[] = {
	FUNC(desc_header, ver_v1, parse_vbios_header),
	FUNC(desc_crtc, ver_v1, parse_vbios_crtc),
	FUNC(desc_encoder, ver_v1, parse_vbios_encoder),
	FUNC(desc_connector, ver_v1, parse_vbios_connector),
	FUNC(desc_i2c, ver_v1, parse_vbios_i2c),
	FUNC(desc_pwm, ver_v1, parse_vbios_pwm),
	FUNC(desc_gpu, ver_v1, parse_vbios_gpu),
};

static inline parse_func *get_parse_func(struct vbios_desc *desc)
{
	parse_func *func = parse_vbios_default;
	u32 tt_num = ARRAY_SIZE(tables);
	u32 type = desc->type;
	u32 ver = desc->ver;
	int i;

	for (i = 0; i < tt_num; i++) {
		if ((tables[i].ver == ver) && (tables[i].type == type)) {
			func = tables[i].func;
			break;
		}
	}

	return func;
}

static inline void parse_vbios_info(struct gsgpu_vbios *vbios)
{
	struct vbios_info *vb_info;
	struct header_resource *header;

	if (IS_ERR_OR_NULL(vbios))
		return;

	header = vbios->funcs->get_header_resource(vbios);;
	if (IS_ERR_OR_NULL(header))
		return;

	vb_info = (struct vbios_info*)vbios->vbios_ptr;
	header->links = vb_info->link_num;
	header->ver_majro = vb_info->version_major;
	header->ver_minor = vb_info->version_minor;
	memcpy(header->name, vb_info->title, 16);
}

static bool dc_vbios_parse(struct gsgpu_vbios *vbios)
{
	struct vbios_header *vb_header;
	struct vbios_desc *start;
	struct vbios_desc *desc;
	enum desc_type type;
	parse_func *func;
	u8 *vbios_ptr;
	bool ret;

	if (IS_ERR_OR_NULL(vbios))
		return false;

	vbios_ptr = (u8 *)vbios->vbios_ptr;
	if (IS_ERR_OR_NULL(vbios_ptr))
		return false;

	/* get header for global information of vbios */
	desc = (struct vbios_desc *)(vbios_ptr + VBIOS_DESC_OFFSET);
	if (desc->type != desc_header) {
		pr_err("vbios first desc not header type\n");
		return false;
	}

	func = get_parse_func(desc);
	if (IS_ERR_OR_NULL(func)) {
		pr_err("vbios get header parser funcs err %pf \n",func);
		return false;
	}

	ret = (*func)(desc, vbios);
	if (!ret) {
		pr_err("get vbios header info error \n");
		return false;
	}

	vb_header = (struct vbios_header *)(vbios_ptr + desc->offset);
	DRM_DEBUG("oem-vendor %s oem-product %s\n", vb_header->oem_vendor,
		 vb_header->oem_product);

	/* start parsing vbios components */
	start = desc = (struct vbios_desc *)(vbios_ptr + vb_header->desc_offset);
	while (1) {
		type = desc->type;
		if (type == desc_header) {
			desc++;
			continue;
		}

		if (type == desc_max || ((desc - start) > vb_header->desc_size) ||
		    ((desc - start) > VBIOS_DESC_TOTAL))
			break;

		func = get_parse_func(desc);
		if (IS_ERR_OR_NULL(func))
			continue;

		ret = (*func)(desc, vbios);
		if (!ret)
			pr_err("Parse T-%d V-%d failed[%d]\n", desc->ver, desc->type, ret);

		desc++;
	}

	/* append legacy information to header resource */
	parse_vbios_info(vbios);

	return true;
}

static bool vbios_resource_pool_create(struct gsgpu_vbios *vbios)
{
	if (IS_ERR_OR_NULL(vbios))
		return false;

	return dc_vbios_parse(vbios);
}

static bool vbios_resource_pool_destory(struct gsgpu_vbios *vbios)
{
	struct resource_object *entry, *tmp;

	if (IS_ERR_OR_NULL(vbios))
		return false;

	if (list_empty(&vbios->resource_list))
		return true;

	list_for_each_entry_safe (entry, tmp, &vbios->resource_list, node) {
		list_del(&entry->node);
		kvfree(entry);
		entry = NULL;
	}

	return true;
}

static bool vbios_create_header_resource(struct gsgpu_vbios *vbios, void *data, u32 size)
{
	struct vbios_header vb_header;
	struct header_resource *header;
	u32 header_size = sizeof(struct vbios_header);

	header = kvmalloc(sizeof(*header), GFP_KERNEL);
	if (IS_ERR_OR_NULL(header))
		return false;

	memset(&vb_header, VBIOS_DATA_INVAL, header_size);
	memcpy(&vb_header, data, min(size, header_size));

	memcpy(header->oem_product, vb_header.oem_product, 32);
	memcpy(header->oem_vendor, vb_header.oem_vendor, 32);
	header->base.type = GSGPU_RESOURCE_HEADER;

	list_add_tail(&header->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_crtc_resource(struct gsgpu_vbios *vbios,
				       void *data, u32 link, u32 size)
{
	struct vbios_crtc vb_crtc;
	struct crtc_resource *crtc;
	u32 crtc_size = sizeof(struct vbios_crtc);

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	crtc = kvmalloc(sizeof(*crtc), GFP_KERNEL);
	if (IS_ERR_OR_NULL(crtc))
		return false;

	memset(&vb_crtc, VBIOS_DATA_INVAL, crtc_size);
	memcpy(&vb_crtc, data, min(size, crtc_size));

	crtc->base.link = link;
	crtc->base.type = GSGPU_RESOURCE_CRTC;
	crtc->feature = vb_crtc.feature;
	crtc->crtc_id = vb_crtc.crtc_id;
	crtc->encoder_id = vb_crtc.encoder_id;
	crtc->max_freq = vb_crtc.max_freq;
	crtc->max_width = vb_crtc.max_width;
	crtc->max_height = vb_crtc.max_height;
	crtc->is_vb_timing = vb_crtc.is_vb_timing;

	list_add_tail(&crtc->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_encoder_resource(struct gsgpu_vbios *vbios, void *data, u32 link, u32 size)
{
	struct vbios_encoder vb_encoder;
	struct encoder_resource *encoder;
	u32 encoder_size;

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	encoder = kvmalloc(sizeof(*encoder), GFP_KERNEL);
	if (IS_ERR_OR_NULL(encoder))
		return false;

	encoder_size = sizeof(struct vbios_encoder);
	memset(&vb_encoder, VBIOS_DATA_INVAL, encoder_size);
	memcpy(&vb_encoder, data, min(size, encoder_size));

	encoder->base.link = link;
	encoder->base.type = GSGPU_RESOURCE_ENCODER;
	encoder->feature = vb_encoder.feature;
	encoder->i2c_id = vb_encoder.i2c_id;
	encoder->connector_id = vb_encoder.connector_id;
	encoder->type = vb_encoder.type;
	encoder->config_type = vb_encoder.config_type;
	encoder->chip_addr = vb_encoder.chip_addr;

	list_add_tail(&encoder->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_connector_resource(struct gsgpu_vbios *vbios, void *data, u32 link, u32 size)
{
	struct vbios_connector vb_connector;
	struct connector_resource *connector;
	u32 connector_size;

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	connector = kvmalloc(sizeof(*connector), GFP_KERNEL);
	if (IS_ERR_OR_NULL(connector))
		return false;

	connector_size = sizeof(struct vbios_connector);
	memset(&vb_connector, VBIOS_DATA_INVAL, connector_size);
	memcpy(&vb_connector, data, min(size, connector_size));

	connector->base.link = link;
	connector->base.type = GSGPU_RESOURCE_CONNECTOR;

	connector->feature = vb_connector.feature;
	connector->i2c_id = vb_connector.i2c_id;

	connector->type = vb_connector.type;
	connector->hotplug = vb_connector.hotplug;
	connector->edid_method = vb_connector.edid_method;
	connector->irq_gpio = vb_connector.irq_gpio;
	connector->gpio_placement = vb_connector.gpio_placement;

	list_add_tail(&connector->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_i2c_resource(struct gsgpu_vbios *vbios, void *data, u32 link, u32 size)
{
	struct i2c_resource *i2c_resource;
	struct vbios_i2c vb_i2c;
	u32 i2c_size;

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	i2c_resource = kvmalloc(sizeof(*i2c_resource), GFP_KERNEL);
	if (IS_ERR_OR_NULL(i2c_resource))
		return false;

	i2c_size = sizeof(struct vbios_i2c);
	memset(&vb_i2c, VBIOS_DATA_INVAL, i2c_size);
	memcpy(&vb_i2c, data, min(size, i2c_size));

	i2c_resource->speed = vb_i2c.speed == VBIOS_DATA_INVAL ? 10 : vb_i2c.speed;
	i2c_resource->base.link = link;
	i2c_resource->base.type = GSGPU_RESOURCE_I2C;

	i2c_resource->feature = vb_i2c.feature;
	i2c_resource->id = vb_i2c.id;

	list_add_tail(&i2c_resource->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_pwm_resource(struct gsgpu_vbios *vbios, void *data, u32 link, u32 size)
{
	struct pwm_resource *pwm_resource;
	struct vbios_pwm vb_pwm;
	u32 pwm_size;

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	pwm_resource = kvmalloc(sizeof(*pwm_resource), GFP_KERNEL);
	if (IS_ERR_OR_NULL(pwm_resource))
		return false;

	pwm_size = sizeof(struct vbios_pwm);
	memset(&vb_pwm, VBIOS_DATA_INVAL, pwm_size);
	memcpy(&vb_pwm, data, min(size, pwm_size));

	pwm_resource->base.link = link;
	pwm_resource->base.type = GSGPU_RESOURCE_PWM;

	pwm_resource->feature = vb_pwm.feature;
	pwm_resource->pwm = vb_pwm.pwm;
	pwm_resource->polarity = vb_pwm.polarity;
	pwm_resource->peroid = vb_pwm.peroid;

	list_add_tail(&pwm_resource->base.node, &vbios->resource_list);

	return true;
}

static bool vbios_create_gpu_resource(struct gsgpu_vbios *vbios, void *data, u32 link, u32 size)
{
	struct gpu_resource *gpu_resource;
	struct vbios_gpu vb_gpu;
	u32 gpu_size;

	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(data))
		return false;

	gpu_resource = kvmalloc(sizeof(*gpu_resource), GFP_KERNEL);
	if (IS_ERR_OR_NULL(gpu_resource))
		return false;

	gpu_size = sizeof(struct vbios_gpu);
	memset(&vb_gpu, VBIOS_DATA_INVAL, gpu_size);
	memcpy(&vb_gpu, data, min(size, gpu_size));

	gpu_resource->base.link = 0;
	gpu_resource->base.type = GSGPU_RESOURCE_GPU;

	gpu_resource->vram_type = vb_gpu.type;
	gpu_resource->bit_width = vb_gpu.bit_width;
	gpu_resource->cap = vb_gpu.cap;
	gpu_resource->count_freq = vb_gpu.count_freq;
	gpu_resource->freq = vb_gpu.freq;
	gpu_resource->shaders_num = vb_gpu.shaders_num;
	gpu_resource->shaders_freq = vb_gpu.shaders_freq;

	list_add_tail(&gpu_resource->base.node, &vbios->resource_list);

	return true;
}

static struct header_resource *vbios_get_header_resource(struct gsgpu_vbios *vbios)
{
	struct resource_object *entry;
	struct header_resource *header;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if (entry->type == GSGPU_RESOURCE_HEADER) {
			header = container_of(entry, struct header_resource, base);
			return header;
		}
	}

	return NULL;
}

static struct crtc_resource *vbios_get_crtc_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct crtc_resource *crtc;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && entry->type == GSGPU_RESOURCE_CRTC) {
			crtc = container_of(entry, struct crtc_resource, base);
			return crtc;
		}
	}

	return NULL;
}

static struct encoder_resource *vbios_get_encoder_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct encoder_resource *encoder;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && entry->type == GSGPU_RESOURCE_ENCODER) {
			encoder = container_of(entry, struct encoder_resource, base);
			return encoder;
		}
	}

	return NULL;
}

static struct connector_resource *vbios_get_connector_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct connector_resource *connector;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && entry->type == GSGPU_RESOURCE_CONNECTOR) {
			connector = container_of(entry, struct connector_resource, base);
			return connector;
		}
	}

	return NULL;
}

static struct i2c_resource *vbios_get_i2c_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct i2c_resource *i2c_resource;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && (entry->type == GSGPU_RESOURCE_I2C)) {
			i2c_resource = container_of(entry, struct i2c_resource, base);
			return i2c_resource;
		}
	}

	return NULL;
}

static struct pwm_resource *vbios_get_pwm_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct pwm_resource *pwm_resource;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && (entry->type == GSGPU_RESOURCE_PWM)) {
			pwm_resource = container_of(entry, struct pwm_resource, base);
			return pwm_resource;
		}
	}

	return NULL;
}

static struct gpu_resource *vbios_get_gpu_resource(struct gsgpu_vbios *vbios, u32 link)
{
	struct resource_object *entry;
	struct gpu_resource *gpu_resource;

	if (IS_ERR_OR_NULL(vbios))
		return NULL;

	if (list_empty(&vbios->resource_list))
		return NULL;

	list_for_each_entry (entry, &vbios->resource_list, node) {
		if ((entry->link == link) && (entry->type == GSGPU_RESOURCE_GPU)) {
			gpu_resource = container_of(entry, struct gpu_resource, base);
			return gpu_resource;
		}
	}

	return NULL;
}

static struct vbios_funcs vbios_funcs = {
	.resource_pool_create = vbios_resource_pool_create,
	.resource_pool_destory = vbios_resource_pool_destory,

	.create_header_resource = vbios_create_header_resource,
	.create_crtc_resource = vbios_create_crtc_resource,
	.create_encoder_resource = vbios_create_encoder_resource,
	.create_connecor_resource = vbios_create_connector_resource,
	.create_i2c_resource =  vbios_create_i2c_resource,
//TODO	.create_gpio_resource = vbios_create_gpio_resource,
	.create_pwm_resource = vbios_create_pwm_resource,
	.create_gpu_resource = vbios_create_gpu_resource,

	.get_header_resource = vbios_get_header_resource,
	.get_crtc_resource = vbios_get_crtc_resource,
	.get_encoder_resource = vbios_get_encoder_resource,
	.get_connector_resource = vbios_get_connector_resource,
	.get_i2c_resource = vbios_get_i2c_resource,
//	.get_gpio_resource = vbios_get_gpio_resource,
	.get_pwm_resource = vbios_get_pwm_resource,
	.get_gpu_resource = vbios_get_gpu_resource,
};

static bool dc_vbios_create(struct gsgpu_vbios *vbios)
{
	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(vbios->funcs))
		return false;

	if (vbios->funcs->resource_pool_create)
		return vbios->funcs->resource_pool_create(vbios);

	return false;
}

void *dc_get_vbios_resource(struct gsgpu_vbios *vbios, u32 link,
			    enum resource_type type)
{
	if (IS_ERR_OR_NULL(vbios) || IS_ERR_OR_NULL(vbios->funcs)) {
		DRM_ERROR("GSGPU get vbios resource%d failed\n", type);
		return NULL;
	}

	switch (type) {
	case GSGPU_RESOURCE_HEADER:
		if (vbios->funcs->get_header_resource)
			return (void *)vbios->funcs->get_header_resource(vbios);
		break;
	case GSGPU_RESOURCE_CRTC:
		if (vbios->funcs->get_crtc_resource)
			return (void *)vbios->funcs->get_crtc_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_ENCODER:
		if (vbios->funcs->get_encoder_resource)
			return (void *)vbios->funcs->get_encoder_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_CONNECTOR:
		if (vbios->funcs->get_connector_resource)
			return (void *)vbios->funcs->get_connector_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_GPIO:
		if (vbios->funcs->get_gpio_resource)
			return (void *)vbios->funcs->get_gpio_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_I2C:
		if (vbios->funcs->get_i2c_resource)
			return (void *)vbios->funcs->get_i2c_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_PWM:
		if (vbios->funcs->get_pwm_resource)
			return (void *)vbios->funcs->get_pwm_resource(vbios, link);
		break;
	case GSGPU_RESOURCE_GPU:
		if (vbios->funcs->get_gpu_resource)
			return (void *)vbios->funcs->get_gpu_resource(vbios, 0);
		break;
	default:
		return NULL;
		break;
	}

	return NULL;
}

void dc_vbios_show(struct gsgpu_vbios *vbios)
{
	struct header_resource *header_res = NULL;
	struct crtc_resource *crtc_res;
	int i;

	header_res = dc_get_vbios_resource(vbios, 0, GSGPU_RESOURCE_HEADER);
	if (header_res == NULL)
		return;

	DRM_INFO("GSGPU vbios header info:\n");
	DRM_INFO("ver:%d.%d links%d max_planes%d name %s\n",
		 header_res->ver_majro, header_res->ver_minor, 
		 header_res->links, header_res->max_planes, header_res->name);
	DRM_INFO("oem-vendor %s oem-product %s\n", header_res->oem_vendor,
		 header_res->oem_product);

	for (i = 0; i < header_res->links; i++) {
		crtc_res = dc_get_vbios_resource(vbios, i, GSGPU_RESOURCE_CRTC);
		DRM_INFO("GSGPU vbios crtc%d max frep%d width%d height%d\n",
			 i, crtc_res->max_freq,
			 crtc_res->max_width, crtc_res->max_height);
	}

}

bool dc_vbios_init(struct gsgpu_dc *dc)
{
	struct vbios_info *header;
	bool status;
	bool ret;

	if (IS_ERR_OR_NULL(dc))
		return false;

	dc->vbios = kmalloc(sizeof(*dc->vbios), GFP_KERNEL);
	if (IS_ERR_OR_NULL(dc->vbios))
		return false;

	dc->vbios->funcs = &vbios_funcs;
	INIT_LIST_HEAD(&dc->vbios->resource_list);

	status = get_vbios_data(dc);
	if (!status) {
		DRM_ERROR("GSGPU Can not get vbios from sysconf!!!\n");
	} else {
		header = dc->vbios->vbios_ptr;;
	}

//TODO: Add vbios default when read bios failed.
	ret = dc_vbios_create(dc->vbios);
	if (ret == false) {
		pr_err("%s %d failed \n", __func__, __LINE__);
		kvfree(dc->vbios);
		dc->vbios = NULL;
	}

	dc_vbios_show(dc->vbios);

	return true;
}

void dc_vbios_exit(struct gsgpu_vbios *vbios)
{
	if (IS_ERR_OR_NULL(vbios))
		return;

	if (!IS_ERR_OR_NULL(vbios->vbios_ptr)) {
		kvfree(vbios->vbios_ptr);
		vbios->vbios_ptr = NULL;
	}

	if (!IS_ERR_OR_NULL(vbios->funcs) &&
	    (vbios->funcs->resource_pool_destory))
		vbios->funcs->resource_pool_destory(vbios);

	kvfree(vbios);
	vbios = NULL;
}
