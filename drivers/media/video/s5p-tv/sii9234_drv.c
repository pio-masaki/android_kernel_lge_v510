/*
 * Samsung MHL interface driver
 *
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Author: Tomasz Stanislawski <t.stanislaws@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/freezer.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/machine.h>
#include <linux/slab.h>

#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#include <media/sii9234.h>
#include <media/v4l2-subdev.h>

MODULE_AUTHOR("Tomasz Stanislawski <t.stanislaws@samsung.com>");
MODULE_DESCRIPTION("Samsung MHL interface driver");
MODULE_LICENSE("GPL");

struct sii9234_context {
	struct i2c_client *client;
	struct regulator *power;
	int gpio_n_reset;
	struct v4l2_subdev sd;
};

static inline struct sii9234_context *sd_to_context(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sii9234_context, sd);
}

static inline int sii9234_readb(struct i2c_client *client, int addr)
{
	return i2c_smbus_read_byte_data(client, addr);
}

static inline int sii9234_writeb(struct i2c_client *client, int addr, int value)
{
	return i2c_smbus_write_byte_data(client, addr, value);
}

static inline int sii9234_writeb_mask(struct i2c_client *client, int addr,
	int value, int mask)
{
	int ret;

	ret = i2c_smbus_read_byte_data(client, addr);
	if (ret < 0)
		return ret;
	ret = (ret & ~mask) | (value & mask);
	return i2c_smbus_write_byte_data(client, addr, ret);
}

static inline int sii9234_readb_idx(struct i2c_client *client, int addr)
{
	int ret;
	ret = i2c_smbus_write_byte_data(client, 0xbc, addr >> 8);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_byte_data(client, 0xbd, addr & 0xff);
	if (ret < 0)
		return ret;
	return i2c_smbus_read_byte_data(client, 0xbe);
}

static inline int sii9234_writeb_idx(struct i2c_client *client, int addr,
	int value)
{
	int ret;
	ret = i2c_smbus_write_byte_data(client, 0xbc, addr >> 8);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_byte_data(client, 0xbd, addr & 0xff);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_byte_data(client, 0xbe, value);
	return ret;
}

static inline int sii9234_writeb_idx_mask(struct i2c_client *client, int addr,
	int value, int mask)
{
	int ret;

	ret = sii9234_readb_idx(client, addr);
	if (ret < 0)
		return ret;
	ret = (ret & ~mask) | (value & mask);
	return sii9234_writeb_idx(client, addr, ret);
}

static int sii9234_reset(struct sii9234_context *ctx)
{
	struct i2c_client *client = ctx->client;
	struct device *dev = &client->dev;
	int ret, tries;

	gpio_direction_output(ctx->gpio_n_reset, 1);
	mdelay(1);
	gpio_direction_output(ctx->gpio_n_reset, 0);
	mdelay(1);
	gpio_direction_output(ctx->gpio_n_reset, 1);
	mdelay(1);

	/*                    */
	ret = sii9234_writeb(client, 0xc7, 0);
	if (ret < 0) {
		dev_err(dev, "failed to set TTPI mode\n");
		return ret;
	}
	for (tries = 0; tries < 100 ; ++tries) {
		ret = sii9234_readb(client, 0x1b);
		if (ret > 0)
			break;
		if (ret < 0) {
			dev_err(dev, "failed to reset device\n");
			return -EIO;
		}
		mdelay(1);
	}
	if (tries == 100) {
		dev_err(dev, "maximal number of tries reached\n");
		return -EIO;
	}

	return 0;
}

static int sii9234_verify_version(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	int family, rev, tpi_rev, dev_id, sub_id, hdcp, id;

	family = sii9234_readb(client, 0x1b);
	rev = sii9234_readb(client, 0x1c) & 0x0f;
	tpi_rev = sii9234_readb(client, 0x1d) & 0x7f;
	dev_id = sii9234_readb_idx(client, 0x0103);
	sub_id = sii9234_readb_idx(client, 0x0102);
	hdcp = sii9234_readb(client, 0x30);

	if (family < 0 || rev < 0 || tpi_rev < 0 || dev_id < 0 ||
		sub_id < 0 || hdcp < 0) {
		dev_err(dev, "failed to read chip's version\n");
		return -EIO;
	}

	id = (dev_id << 8) | sub_id;

	dev_info(dev, "chip: SiL%02x family: %02x, rev: %02x\n",
		id, family, rev);
	dev_info(dev, "tpi_rev:%02x, hdcp: %02x\n", tpi_rev, hdcp);
	if (id != 0x9234) {
		dev_err(dev, "not supported chip\n");
		return -ENODEV;
	}

	return 0;
}

static u8 data[][3] = {
/*                                            */
	{ 0x01, 0x05, 0x04 }, /*                                    */
	{ 0x01, 0x08, 0x35 }, /*                       */
	{ 0x01, 0x0d, 0x1c }, /*                            */
	{ 0x01, 0x2b, 0x01 }, /*                                   */
	{ 0x01, 0x79, 0x40 }, /*                       */
	{ 0x01, 0x80, 0x34 }, /*                           */
	{ 0x01, 0x90, 0x27 }, /*                       */
	{ 0x01, 0x91, 0xe5 }, /*                     */
	{ 0x01, 0x92, 0x46 }, /*                */
	{ 0x01, 0x93, 0xdc }, /*                                              */
	{ 0x01, 0x94, 0x66 }, /*                               */
	{ 0x01, 0x95, 0x31 }, /*                                       */
	{ 0x01, 0x96, 0x22 }, /*                       */
	{ 0x01, 0xa0, 0x10 }, /*                 */
	{ 0x01, 0xa1, 0xfc }, /*                                   */
	{ 0x01, 0xa3, 0xfa }, /*                                             */
	{ 0x01, 0xa5, 0x80 }, /*                                          */
	{ 0x01, 0xa6, 0x0c }, /*                    */
	{ 0x02, 0x3d, 0x3f }, /*                         */
	{ 0x03, 0x00, 0x00 }, /*                             */
	{ 0x03, 0x11, 0x01 }, /*                    */
	{ 0x03, 0x12, 0x15 }, /*                                  */
	{ 0x03, 0x13, 0x60 }, /*                             */
	{ 0x03, 0x14, 0xf0 }, /*                         */
	{ 0x03, 0x17, 0x07 }, /*                     */
	{ 0x03, 0x1a, 0x20 }, /*         */
	{ 0x03, 0x22, 0xe0 }, /*               */
	{ 0x03, 0x23, 0xc0 }, /*               */
	{ 0x03, 0x24, 0xa0 }, /*               */
	{ 0x03, 0x25, 0x80 }, /*               */
	{ 0x03, 0x26, 0x60 }, /*               */
	{ 0x03, 0x27, 0x40 }, /*               */
	{ 0x03, 0x28, 0x20 }, /*               */
	{ 0x03, 0x29, 0x00 }, /*               */
	{ 0x03, 0x31, 0x0b }, /*                                          */
	{ 0x03, 0x45, 0x06 }, /*                 */
	{ 0x03, 0x4b, 0x06 }, /*                             */
	{ 0x03, 0x4c, 0xa0 }, /*                     */
	{ 0x03, 0x4d, 0x02 }, /*                                           */
};

static int sii9234_set_internal(struct sii9234_context *ctx)
{
	struct i2c_client *client = ctx->client;
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(data); ++i) {
		int addr = (data[i][0] << 8) | data[i][1];
		ret = sii9234_writeb_idx(client, addr, data[i][2]);
		if (ret < 0)
			return ret;
	}
	return 0;
}

static int sii9234_runtime_suspend(struct device *dev)
{
	struct v4l2_subdev *sd = dev_get_drvdata(dev);
	struct sii9234_context *ctx = sd_to_context(sd);
	struct i2c_client *client = ctx->client;

	dev_info(dev, "suspend start\n");

	sii9234_writeb_mask(client, 0x1e, 3, 3);
	regulator_disable(ctx->power);

	return 0;
}

static int sii9234_runtime_resume(struct device *dev)
{
	struct v4l2_subdev *sd = dev_get_drvdata(dev);
	struct sii9234_context *ctx = sd_to_context(sd);
	struct i2c_client *client = ctx->client;
	int ret;

	dev_info(dev, "resume start\n");
	regulator_enable(ctx->power);

	ret = sii9234_reset(ctx);
	if (ret)
		goto fail;

	/*            */
	ret = sii9234_writeb_mask(client, 0x1e, 1, 0);
	if (ret < 0)
		goto fail;
	ret = sii9234_set_internal(ctx);
	if (ret < 0)
		goto fail;

	return 0;

fail:
	dev_err(dev, "failed to resume\n");
	regulator_disable(ctx->power);

	return ret;
}

static const struct dev_pm_ops sii9234_pm_ops = {
	.runtime_suspend = sii9234_runtime_suspend,
	.runtime_resume	 = sii9234_runtime_resume,
};

static int sii9234_s_power(struct v4l2_subdev *sd, int on)
{
	struct sii9234_context *ctx = sd_to_context(sd);
	int ret;

	if (on)
		ret = pm_runtime_get_sync(&ctx->client->dev);
	else
		ret = pm_runtime_put(&ctx->client->dev);
	/*                                 */
	return IS_ERR_VALUE(ret) ? ret : 0;
}

static int sii9234_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct sii9234_context *ctx = sd_to_context(sd);

	/*                          */
	sii9234_writeb_mask(ctx->client, 0x1a, enable ? 0 : ~0 , 1 << 4);
	return 0;
}

static const struct v4l2_subdev_core_ops sii9234_core_ops = {
	.s_power =  sii9234_s_power,
};

static const struct v4l2_subdev_video_ops sii9234_video_ops = {
	.s_stream =  sii9234_s_stream,
};

static const struct v4l2_subdev_ops sii9234_ops = {
	.core = &sii9234_core_ops,
	.video = &sii9234_video_ops,
};

static int __devinit sii9234_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct sii9234_platform_data *pdata = dev->platform_data;
	struct sii9234_context *ctx;
	int ret;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		dev_err(dev, "out of memory\n");
		ret = -ENOMEM;
		goto fail;
	}
	ctx->client = client;

	ctx->power = regulator_get(dev, "hdmi-en");
	if (IS_ERR(ctx->power)) {
		dev_err(dev, "failed to acquire regulator hdmi-en\n");
		ret = PTR_ERR(ctx->power);
		goto fail_ctx;
	}

	ctx->gpio_n_reset = pdata->gpio_n_reset;
	ret = gpio_request(ctx->gpio_n_reset, "MHL_RST");
	if (ret) {
		dev_err(dev, "failed to acquire MHL_RST gpio\n");
		goto fail_power;
	}

	v4l2_i2c_subdev_init(&ctx->sd, client, &sii9234_ops);

	pm_runtime_enable(dev);

	/*               */
	ret = pm_runtime_get_sync(dev);
	if (ret)
		goto fail_pm;

	/*                     */
	ret = sii9234_verify_version(client);
	if (ret)
		goto fail_pm_get;

	/*                 */
	pm_runtime_put(dev);

	dev_info(dev, "probe successful\n");

	return 0;

fail_pm_get:
	pm_runtime_put_sync(dev);

fail_pm:
	pm_runtime_disable(dev);
	gpio_free(ctx->gpio_n_reset);

fail_power:
	regulator_put(ctx->power);

fail_ctx:
	kfree(ctx);

fail:
	dev_err(dev, "probe failed\n");

	return ret;
}

static int __devexit sii9234_remove(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sii9234_context *ctx = sd_to_context(sd);

	pm_runtime_disable(dev);
	gpio_free(ctx->gpio_n_reset);
	regulator_put(ctx->power);
	kfree(ctx);

	dev_info(dev, "remove successful\n");

	return 0;
}


static const struct i2c_device_id sii9234_id[] = {
	{ "SII9234", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, sii9234_id);
static struct i2c_driver sii9234_driver = {
	.driver = {
		.name	= "sii9234",
		.owner	= THIS_MODULE,
		.pm = &sii9234_pm_ops,
	},
	.probe		= sii9234_probe,
	.remove		= __devexit_p(sii9234_remove),
	.id_table = sii9234_id,
};

static int __init sii9234_init(void)
{
	return i2c_add_driver(&sii9234_driver);
}
module_init(sii9234_init);

static void __exit sii9234_exit(void)
{
	i2c_del_driver(&sii9234_driver);
}
module_exit(sii9234_exit);
