/* linux/arch/arm/mach-exynos4/include/mach/regs-hdmi.h
 *
 * Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
 * http://www.samsung.com/
 *
 * HDMI register header file for Samsung TVOUT driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef SAMSUNG_REGS_HDMI_H
#define SAMSUNG_REGS_HDMI_H

/*
                
*/

#define HDMI_CTRL_BASE(x)		((x) + 0x00000000)
#define HDMI_CORE_BASE(x)		((x) + 0x00010000)
#define HDMI_TG_BASE(x)			((x) + 0x00050000)

/*                   */
#define HDMI_INTC_CON			HDMI_CTRL_BASE(0x0000)
#define HDMI_INTC_FLAG			HDMI_CTRL_BASE(0x0004)
#define HDMI_HPD_STATUS			HDMI_CTRL_BASE(0x000C)
#define HDMI_PHY_RSTOUT			HDMI_CTRL_BASE(0x0014)
#define HDMI_PHY_VPLL			HDMI_CTRL_BASE(0x0018)
#define HDMI_PHY_CMU			HDMI_CTRL_BASE(0x001C)
#define HDMI_CORE_RSTOUT		HDMI_CTRL_BASE(0x0020)

/*                */
#define HDMI_CON_0			HDMI_CORE_BASE(0x0000)
#define HDMI_CON_1			HDMI_CORE_BASE(0x0004)
#define HDMI_CON_2			HDMI_CORE_BASE(0x0008)
#define HDMI_SYS_STATUS			HDMI_CORE_BASE(0x0010)
#define HDMI_PHY_STATUS			HDMI_CORE_BASE(0x0014)
#define HDMI_STATUS_EN			HDMI_CORE_BASE(0x0020)
#define HDMI_HPD			HDMI_CORE_BASE(0x0030)
#define HDMI_MODE_SEL			HDMI_CORE_BASE(0x0040)
#define HDMI_BLUE_SCREEN_0		HDMI_CORE_BASE(0x0050)
#define HDMI_BLUE_SCREEN_1		HDMI_CORE_BASE(0x0054)
#define HDMI_BLUE_SCREEN_2		HDMI_CORE_BASE(0x0058)
#define HDMI_H_BLANK_0			HDMI_CORE_BASE(0x00A0)
#define HDMI_H_BLANK_1			HDMI_CORE_BASE(0x00A4)
#define HDMI_V_BLANK_0			HDMI_CORE_BASE(0x00B0)
#define HDMI_V_BLANK_1			HDMI_CORE_BASE(0x00B4)
#define HDMI_V_BLANK_2			HDMI_CORE_BASE(0x00B8)
#define HDMI_H_V_LINE_0			HDMI_CORE_BASE(0x00C0)
#define HDMI_H_V_LINE_1			HDMI_CORE_BASE(0x00C4)
#define HDMI_H_V_LINE_2			HDMI_CORE_BASE(0x00C8)
#define HDMI_VSYNC_POL			HDMI_CORE_BASE(0x00E4)
#define HDMI_INT_PRO_MODE		HDMI_CORE_BASE(0x00E8)
#define HDMI_V_BLANK_F_0		HDMI_CORE_BASE(0x0110)
#define HDMI_V_BLANK_F_1		HDMI_CORE_BASE(0x0114)
#define HDMI_V_BLANK_F_2		HDMI_CORE_BASE(0x0118)
#define HDMI_H_SYNC_GEN_0		HDMI_CORE_BASE(0x0120)
#define HDMI_H_SYNC_GEN_1		HDMI_CORE_BASE(0x0124)
#define HDMI_H_SYNC_GEN_2		HDMI_CORE_BASE(0x0128)
#define HDMI_V_SYNC_GEN_1_0		HDMI_CORE_BASE(0x0130)
#define HDMI_V_SYNC_GEN_1_1		HDMI_CORE_BASE(0x0134)
#define HDMI_V_SYNC_GEN_1_2		HDMI_CORE_BASE(0x0138)
#define HDMI_V_SYNC_GEN_2_0		HDMI_CORE_BASE(0x0140)
#define HDMI_V_SYNC_GEN_2_1		HDMI_CORE_BASE(0x0144)
#define HDMI_V_SYNC_GEN_2_2		HDMI_CORE_BASE(0x0148)
#define HDMI_V_SYNC_GEN_3_0		HDMI_CORE_BASE(0x0150)
#define HDMI_V_SYNC_GEN_3_1		HDMI_CORE_BASE(0x0154)
#define HDMI_V_SYNC_GEN_3_2		HDMI_CORE_BASE(0x0158)
#define HDMI_AVI_CON			HDMI_CORE_BASE(0x0300)
#define HDMI_AVI_BYTE(n)		HDMI_CORE_BASE(0x0320 + 4 * (n))
#define	HDMI_DC_CONTROL			HDMI_CORE_BASE(0x05C0)
#define HDMI_VIDEO_PATTERN_GEN		HDMI_CORE_BASE(0x05C4)
#define HDMI_HPD_GEN			HDMI_CORE_BASE(0x05C8)

/*                            */
#define HDMI_TG_CMD			HDMI_TG_BASE(0x0000)
#define HDMI_TG_H_FSZ_L			HDMI_TG_BASE(0x0018)
#define HDMI_TG_H_FSZ_H			HDMI_TG_BASE(0x001C)
#define HDMI_TG_HACT_ST_L		HDMI_TG_BASE(0x0020)
#define HDMI_TG_HACT_ST_H		HDMI_TG_BASE(0x0024)
#define HDMI_TG_HACT_SZ_L		HDMI_TG_BASE(0x0028)
#define HDMI_TG_HACT_SZ_H		HDMI_TG_BASE(0x002C)
#define HDMI_TG_V_FSZ_L			HDMI_TG_BASE(0x0030)
#define HDMI_TG_V_FSZ_H			HDMI_TG_BASE(0x0034)
#define HDMI_TG_VSYNC_L			HDMI_TG_BASE(0x0038)
#define HDMI_TG_VSYNC_H			HDMI_TG_BASE(0x003C)
#define HDMI_TG_VSYNC2_L		HDMI_TG_BASE(0x0040)
#define HDMI_TG_VSYNC2_H		HDMI_TG_BASE(0x0044)
#define HDMI_TG_VACT_ST_L		HDMI_TG_BASE(0x0048)
#define HDMI_TG_VACT_ST_H		HDMI_TG_BASE(0x004C)
#define HDMI_TG_VACT_SZ_L		HDMI_TG_BASE(0x0050)
#define HDMI_TG_VACT_SZ_H		HDMI_TG_BASE(0x0054)
#define HDMI_TG_FIELD_CHG_L		HDMI_TG_BASE(0x0058)
#define HDMI_TG_FIELD_CHG_H		HDMI_TG_BASE(0x005C)
#define HDMI_TG_VACT_ST2_L		HDMI_TG_BASE(0x0060)
#define HDMI_TG_VACT_ST2_H		HDMI_TG_BASE(0x0064)
#define HDMI_TG_VSYNC_TOP_HDMI_L	HDMI_TG_BASE(0x0078)
#define HDMI_TG_VSYNC_TOP_HDMI_H	HDMI_TG_BASE(0x007C)
#define HDMI_TG_VSYNC_BOT_HDMI_L	HDMI_TG_BASE(0x0080)
#define HDMI_TG_VSYNC_BOT_HDMI_H	HDMI_TG_BASE(0x0084)
#define HDMI_TG_FIELD_TOP_HDMI_L	HDMI_TG_BASE(0x0088)
#define HDMI_TG_FIELD_TOP_HDMI_H	HDMI_TG_BASE(0x008C)
#define HDMI_TG_FIELD_BOT_HDMI_L	HDMI_TG_BASE(0x0090)
#define HDMI_TG_FIELD_BOT_HDMI_H	HDMI_TG_BASE(0x0094)

/*
                      
 */

/*               */
#define HDMI_INTC_EN_GLOBAL		(1 << 6)
#define HDMI_INTC_EN_HPD_PLUG		(1 << 3)
#define HDMI_INTC_EN_HPD_UNPLUG		(1 << 2)

/*                */
#define HDMI_INTC_FLAG_HPD_PLUG		(1 << 3)
#define HDMI_INTC_FLAG_HPD_UNPLUG	(1 << 2)

/*                 */
#define HDMI_PHY_SW_RSTOUT		(1 << 0)

/*                  */
#define HDMI_CORE_SW_RSTOUT		(1 << 0)

/*            */
#define HDMI_BLUE_SCR_EN		(1 << 5)
#define HDMI_EN				(1 << 0)

/*            */
#define HDMI_DVI_PERAMBLE_EN		(1 << 5)
#define HDMI_DVI_BAND_EN		(1 << 1)

/*                 */
#define HDMI_PHY_STATUS_READY		(1 << 0)

/*               */
#define HDMI_MODE_HDMI_EN		(1 << 1)
#define HDMI_MODE_DVI_EN		(1 << 0)
#define HDMI_MODE_MASK			(3 << 0)

/*             */
#define HDMI_TG_EN			(1 << 0)

#endif /*                     */
