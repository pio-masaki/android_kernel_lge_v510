/*
 * f_sourcesink.c - USB peripheral source/sink configuration driver
 *
 * Copyright (C) 2003-2008 David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*                       */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>

#include "g_zero.h"
#include "gadget_chips.h"


/*
                                                                        
                      
  
                                                                         
                                                                           
                                                          
  
                                                                         
                                                                         
                                                                        
                            
  
                                                                        
                                                                        
                                                                          
                                                                           
                                                                           
                                                     
  
  
                                                                       
                                                                        
                                                         
 */
struct f_sourcesink {
	struct usb_function	function;

	struct usb_ep		*in_ep;
	struct usb_ep		*out_ep;
};

static inline struct f_sourcesink *func_to_ss(struct usb_function *f)
{
	return container_of(f, struct f_sourcesink, function);
}

static unsigned pattern;
module_param(pattern, uint, 0);
MODULE_PARM_DESC(pattern, "0 = all zeroes, 1 = mod63 ");

/*                                                                         */

static struct usb_interface_descriptor source_sink_intf = {
	.bLength =		sizeof source_sink_intf,
	.bDescriptorType =	USB_DT_INTERFACE,

	.bNumEndpoints =	2,
	.bInterfaceClass =	USB_CLASS_VENDOR_SPEC,
	/*                       */
};

/*                     */

static struct usb_endpoint_descriptor fs_source_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fs_sink_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header *fs_source_sink_descs[] = {
	(struct usb_descriptor_header *) &source_sink_intf,
	(struct usb_descriptor_header *) &fs_sink_desc,
	(struct usb_descriptor_header *) &fs_source_desc,
	NULL,
};

/*                     */

static struct usb_endpoint_descriptor hs_source_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_sink_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_descriptor_header *hs_source_sink_descs[] = {
	(struct usb_descriptor_header *) &source_sink_intf,
	(struct usb_descriptor_header *) &hs_source_desc,
	(struct usb_descriptor_header *) &hs_sink_desc,
	NULL,
};

/*                      */

static struct usb_endpoint_descriptor ss_source_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

struct usb_ss_ep_comp_descriptor ss_source_comp_desc = {
	.bLength =		USB_DT_SS_EP_COMP_SIZE,
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,
	.bMaxBurst =		0,
	.bmAttributes =		0,
	.wBytesPerInterval =	0,
};

static struct usb_endpoint_descriptor ss_sink_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

struct usb_ss_ep_comp_descriptor ss_sink_comp_desc = {
	.bLength =		USB_DT_SS_EP_COMP_SIZE,
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,
	.bMaxBurst =		0,
	.bmAttributes =		0,
	.wBytesPerInterval =	0,
};

static struct usb_descriptor_header *ss_source_sink_descs[] = {
	(struct usb_descriptor_header *) &source_sink_intf,
	(struct usb_descriptor_header *) &ss_source_desc,
	(struct usb_descriptor_header *) &ss_source_comp_desc,
	(struct usb_descriptor_header *) &ss_sink_desc,
	(struct usb_descriptor_header *) &ss_sink_comp_desc,
	NULL,
};

/*                            */

static struct usb_string strings_sourcesink[] = {
	[0].s = "source and sink data",
	{  }			/*             */
};

static struct usb_gadget_strings stringtab_sourcesink = {
	.language	= 0x0409,	/*       */
	.strings	= strings_sourcesink,
};

static struct usb_gadget_strings *sourcesink_strings[] = {
	&stringtab_sourcesink,
	NULL,
};

/*                                                                         */

static int __init
sourcesink_bind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_composite_dev *cdev = c->cdev;
	struct f_sourcesink	*ss = func_to_ss(f);
	int	id;

	/*                          */
	id = usb_interface_id(c, f);
	if (id < 0)
		return id;
	source_sink_intf.bInterfaceNumber = id;

	/*                    */
	ss->in_ep = usb_ep_autoconfig(cdev->gadget, &fs_source_desc);
	if (!ss->in_ep) {
autoconf_fail:
		ERROR(cdev, "%s: can't autoconfigure on %s\n",
			f->name, cdev->gadget->name);
		return -ENODEV;
	}
	ss->in_ep->driver_data = cdev;	/*       */

	ss->out_ep = usb_ep_autoconfig(cdev->gadget, &fs_sink_desc);
	if (!ss->out_ep)
		goto autoconf_fail;
	ss->out_ep->driver_data = cdev;	/*       */

	/*                             */
	if (gadget_is_dualspeed(c->cdev->gadget)) {
		hs_source_desc.bEndpointAddress =
				fs_source_desc.bEndpointAddress;
		hs_sink_desc.bEndpointAddress =
				fs_sink_desc.bEndpointAddress;
		f->hs_descriptors = hs_source_sink_descs;
	}

	/*                              */
	if (gadget_is_superspeed(c->cdev->gadget)) {
		ss_source_desc.bEndpointAddress =
				fs_source_desc.bEndpointAddress;
		ss_sink_desc.bEndpointAddress =
				fs_sink_desc.bEndpointAddress;
		f->ss_descriptors = ss_source_sink_descs;
	}

	DBG(cdev, "%s speed %s: IN/%s, OUT/%s\n",
	    (gadget_is_superspeed(c->cdev->gadget) ? "super" :
	     (gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full")),
			f->name, ss->in_ep->name, ss->out_ep->name);
	return 0;
}

static void
sourcesink_unbind(struct usb_configuration *c, struct usb_function *f)
{
	kfree(func_to_ss(f));
}

/*                                                        */
static int check_read_data(struct f_sourcesink *ss, struct usb_request *req)
{
	unsigned		i;
	u8			*buf = req->buf;
	struct usb_composite_dev *cdev = ss->function.config->cdev;

	for (i = 0; i < req->actual; i++, buf++) {
		switch (pattern) {

		/*                                          */
		case 0:
			if (*buf == 0)
				continue;
			break;

		/*                                                       
                                                         
                                                         
                                                        
                                                            
                                                            
   */
		case 1:
			if (*buf == (u8)(i % 63))
				continue;
			break;
		}
		ERROR(cdev, "bad OUT byte, buf[%d] = %d\n", i, *buf);
		usb_ep_set_halt(ss->out_ep);
		return -EINVAL;
	}
	return 0;
}

static void reinit_write_data(struct usb_ep *ep, struct usb_request *req)
{
	unsigned	i;
	u8		*buf = req->buf;

	switch (pattern) {
	case 0:
		memset(req->buf, 0, req->length);
		break;
	case 1:
		for  (i = 0; i < req->length; i++)
			*buf++ = (u8) (i % 63);
		break;
	}
}

static void source_sink_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_sourcesink	*ss = ep->driver_data;
	struct usb_composite_dev *cdev = ss->function.config->cdev;
	int			status = req->status;

	switch (status) {

	case 0:				/*                    */
		if (ep == ss->out_ep) {
			check_read_data(ss, req);
			memset(req->buf, 0x55, req->length);
		} else
			reinit_write_data(ep, req);
		break;

	/*                                                         */
	case -ECONNABORTED:		/*                          */
	case -ECONNRESET:		/*                  */
	case -ESHUTDOWN:		/*                      */
		VDBG(cdev, "%s gone (%d), %d/%d\n", ep->name, status,
				req->actual, req->length);
		if (ep == ss->out_ep)
			check_read_data(ss, req);
		free_ep_req(ep, req);
		return;

	case -EOVERFLOW:		/*                                  
                                      
               
      */
	default:
#if 1
		DBG(cdev, "%s complete --> %d, %d/%d\n", ep->name,
				status, req->actual, req->length);
#endif
	case -EREMOTEIO:		/*            */
		break;
	}

	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		ERROR(cdev, "kill %s:  resubmit %d bytes --> %d\n",
				ep->name, req->length, status);
		usb_ep_set_halt(ep);
		/*                                 */
	}
}

static int source_sink_start_ep(struct f_sourcesink *ss, bool is_in)
{
	struct usb_ep		*ep;
	struct usb_request	*req;
	int			status;

	ep = is_in ? ss->in_ep : ss->out_ep;
	req = alloc_ep_req(ep);
	if (!req)
		return -ENOMEM;

	req->complete = source_sink_complete;
	if (is_in)
		reinit_write_data(ep, req);
	else
		memset(req->buf, 0x55, req->length);

	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		struct usb_composite_dev	*cdev;

		cdev = ss->function.config->cdev;
		ERROR(cdev, "start %s %s --> %d\n",
				is_in ? "IN" : "OUT",
				ep->name, status);
		free_ep_req(ep, req);
	}

	return status;
}

static void disable_source_sink(struct f_sourcesink *ss)
{
	struct usb_composite_dev	*cdev;

	cdev = ss->function.config->cdev;
	disable_endpoints(cdev, ss->in_ep, ss->out_ep);
	VDBG(cdev, "%s disabled\n", ss->function.name);
}

static int
enable_source_sink(struct usb_composite_dev *cdev, struct f_sourcesink *ss)
{
	int					result = 0;
	struct usb_ep				*ep;

	/*                                                       */
	ep = ss->in_ep;
	result = config_ep_by_speed(cdev->gadget, &(ss->function), ep);
	if (result)
		return result;
	result = usb_ep_enable(ep);
	if (result < 0)
		return result;
	ep->driver_data = ss;

	result = source_sink_start_ep(ss, true);
	if (result < 0) {
fail:
		ep = ss->in_ep;
		usb_ep_disable(ep);
		ep->driver_data = NULL;
		return result;
	}

	/*                                                         */
	ep = ss->out_ep;
	result = config_ep_by_speed(cdev->gadget, &(ss->function), ep);
	if (result)
		goto fail;
	result = usb_ep_enable(ep);
	if (result < 0)
		goto fail;
	ep->driver_data = ss;

	result = source_sink_start_ep(ss, false);
	if (result < 0) {
		usb_ep_disable(ep);
		ep->driver_data = NULL;
		goto fail;
	}

	DBG(cdev, "%s enabled\n", ss->function.name);
	return result;
}

static int sourcesink_set_alt(struct usb_function *f,
		unsigned intf, unsigned alt)
{
	struct f_sourcesink	*ss = func_to_ss(f);
	struct usb_composite_dev *cdev = f->config->cdev;

	/*                     */
	if (ss->in_ep->driver_data)
		disable_source_sink(ss);
	return enable_source_sink(cdev, ss);
}

static void sourcesink_disable(struct usb_function *f)
{
	struct f_sourcesink	*ss = func_to_ss(f);

	disable_source_sink(ss);
}

/*                                                                         */

static int __init sourcesink_bind_config(struct usb_configuration *c)
{
	struct f_sourcesink	*ss;
	int			status;

	ss = kzalloc(sizeof *ss, GFP_KERNEL);
	if (!ss)
		return -ENOMEM;

	ss->function.name = "source/sink";
	ss->function.descriptors = fs_source_sink_descs;
	ss->function.bind = sourcesink_bind;
	ss->function.unbind = sourcesink_unbind;
	ss->function.set_alt = sourcesink_set_alt;
	ss->function.disable = sourcesink_disable;

	status = usb_add_function(c, &ss->function);
	if (status)
		kfree(ss);
	return status;
}

static int sourcesink_setup(struct usb_configuration *c,
		const struct usb_ctrlrequest *ctrl)
{
	struct usb_request	*req = c->cdev->req;
	int			value = -EOPNOTSUPP;
	u16			w_index = le16_to_cpu(ctrl->wIndex);
	u16			w_value = le16_to_cpu(ctrl->wValue);
	u16			w_length = le16_to_cpu(ctrl->wLength);

	req->length = USB_BUFSIZ;

	/*                                                          
                                  
  */
	switch (ctrl->bRequest) {

	/*
                                                            
                                                            
                                                     
   
                                                            
                                                            
                                  
  */
	case 0x5b:	/*                                       */
		if (ctrl->bRequestType != (USB_DIR_OUT|USB_TYPE_VENDOR))
			goto unknown;
		if (w_value || w_index)
			break;
		/*                                           */
		if (w_length > req->length)
			break;
		value = w_length;
		break;
	case 0x5c:	/*                                        */
		if (ctrl->bRequestType != (USB_DIR_IN|USB_TYPE_VENDOR))
			goto unknown;
		if (w_value || w_index)
			break;
		/*                                                       */
		if (w_length > req->length)
			break;
		value = w_length;
		break;

	default:
unknown:
		VDBG(c->cdev,
			"unknown control req%02x.%02x v%04x i%04x l%d\n",
			ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, w_length);
	}

	/*                                             */
	if (value >= 0) {
		VDBG(c->cdev, "source/sink req%02x.%02x v%04x i%04x l%d\n",
			ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, w_length);
		req->zero = 0;
		req->length = value;
		value = usb_ep_queue(c->cdev->gadget->ep0, req, GFP_ATOMIC);
		if (value < 0)
			ERROR(c->cdev, "source/sinkc response, err %d\n",
					value);
	}

	/*                                                     */
	return value;
}

static struct usb_configuration sourcesink_driver = {
	.label		= "source/sink",
	.strings	= sourcesink_strings,
	.setup		= sourcesink_setup,
	.bConfigurationValue = 3,
	.bmAttributes	= USB_CONFIG_ATT_SELFPOWER,
	/*                           */
};

/* 
                                                                       
                                                 
 */
int __init sourcesink_add(struct usb_composite_dev *cdev, bool autoresume)
{
	int id;

	/*                       */
	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_sourcesink[0].id = id;

	source_sink_intf.iInterface = id;
	sourcesink_driver.iConfiguration = id;

	/*                                              */
	if (autoresume)
		sourcesink_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;

	/*                     */
	if (gadget_is_otg(cdev->gadget)) {
		sourcesink_driver.descriptors = otg_desc;
		sourcesink_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

	return usb_add_config(cdev, &sourcesink_driver, sourcesink_bind_config);
}
