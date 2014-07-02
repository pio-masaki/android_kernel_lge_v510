/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __ASM_ARCH_MSM_IRQS_8625_H
#define __ASM_ARCH_MSM_IRQS_8625_H

#define GIC_PPI_START		16
#define GIC_SPI_START		32

/*                                       */
#define MSM8625_INT_A5_PMU_IRQ		(GIC_PPI_START + 1)
#define MSM8625_INT_DEBUG_TIMER_EXP	(GIC_PPI_START + 2)
#define MSM8625_INT_GP_TIMER_EXP	(GIC_PPI_START + 3)
#define MSM8625_INT_COMMRX		(GIC_PPI_START + 4)
#define MSM8625_INT_COMMTX		(GIC_PPI_START + 5)

/*                           
 */

#define MSM8625_INT_A9_M2A_0		(GIC_SPI_START + 0)
#define MSM8625_INT_A9_M2A_1		(GIC_SPI_START + 1)
#define MSM8625_INT_A9_M2A_2		(GIC_SPI_START + 2)
#define MSM8625_INT_A9_M2A_3		(GIC_SPI_START + 3)
#define MSM8625_INT_A9_M2A_4		(GIC_SPI_START + 4)
#define MSM8625_INT_A9_M2A_5		(GIC_SPI_START + 5)
#define MSM8625_INT_A9_M2A_6		(GIC_SPI_START + 6)
#define MSM8625_INT_ACSR_MP_CORE_IPC0	(GIC_SPI_START + 7)
#define MSM8625_INT_ACSR_MP_CORE_IPC1	(GIC_SPI_START + 8)
#define MSM8625_INT_UART1		(GIC_SPI_START + 9)
#define MSM8625_INT_UART2		(GIC_SPI_START + 10)
#define MSM8625_INT_UART3		(GIC_SPI_START + 11)
#define MSM8625_INT_UART1_RX		(GIC_SPI_START + 12)
#define MSM8625_INT_UART2_RX		(GIC_SPI_START + 13)
#define MSM8625_INT_UART3_RX		(GIC_SPI_START + 14)
#define MSM8625_INT_USB_OTG		(GIC_SPI_START + 15)
#define MSM8625_INT_DSI_IRQ		(GIC_SPI_START + 16)
#define MSM8625_INT_CSI_IRQ_1		(GIC_SPI_START + 17)
#define MSM8625_INT_CSI_IRQ_0		(GIC_SPI_START + 18)
#define MSM8625_INT_MDP			(GIC_SPI_START + 19)
#define MSM8625_INT_GRAPHICS		(GIC_SPI_START + 20)
#define MSM8625_INT_ADM_AARM		(GIC_SPI_START + 21)
#define MSM8625_INT_ADSP_A11		(GIC_SPI_START + 22)
#define MSM8625_INT_ADSP_A9_A11		(GIC_SPI_START + 23)
#define MSM8625_INT_SDC1_0		(GIC_SPI_START + 24)
#define MSM8625_INT_SDC1_1		(GIC_SPI_START + 25)
#define MSM8625_INT_SDC2_0		(GIC_SPI_START + 26)
#define MSM8625_INT_SDC2_1		(GIC_SPI_START + 27)
#define MSM8625_INT_KEYSENSE		(GIC_SPI_START + 28)
#define MSM8625_INT_TCHSCRN_SSBI	(GIC_SPI_START + 29)
#define MSM8625_INT_TCHSCRN1		(GIC_SPI_START + 30)
#define MSM8625_INT_TCHSCRN2		(GIC_SPI_START + 31)

#define MSM8625_INT_GPIO_GROUP1		(GIC_SPI_START + 32 + 0)
#define MSM8625_INT_GPIO_GROUP2		(GIC_SPI_START + 32 + 1)
#define MSM8625_INT_PWB_I2C		(GIC_SPI_START + 32 + 2)
#define MSM8625_INT_SOFTRESET		(GIC_SPI_START + 32 + 3)
#define MSM8625_INT_NAND_WR_ER_DONE	(GIC_SPI_START + 32 + 4)
#define MSM8625_INT_NAND_OP_DONE	(GIC_SPI_START + 32 + 5)
#define MSM8625_INT_PBUS_ARM11		(GIC_SPI_START + 32 + 6)
#define MSM8625_INT_AXI_MPU_SMI		(GIC_SPI_START + 32 + 7)
#define MSM8625_INT_AXI_MPU_EBI1	(GIC_SPI_START + 32 + 8)
#define MSM8625_INT_AD_HSSD		(GIC_SPI_START + 32 + 9)
#define MSM8625_INT_NOTUSED		(GIC_SPI_START + 32 + 10)
#define MSM8625_INT_ARM11_DMA		(GIC_SPI_START + 32 + 11)
#define MSM8625_INT_TSIF_IRQ		(GIC_SPI_START + 32 + 12)
#define MSM8625_INT_UART1DM_IRQ		(GIC_SPI_START + 32 + 13)
#define MSM8625_INT_UART1DM_RX		(GIC_SPI_START + 32 + 14)
#define MSM8625_INT_USB_HS		(GIC_SPI_START + 32 + 15)
#define MSM8625_INT_SDC3_0		(GIC_SPI_START + 32 + 16)
#define MSM8625_INT_SDC3_1		(GIC_SPI_START + 32 + 17)
#define MSM8625_INT_SDC4_0		(GIC_SPI_START + 32 + 18)
#define MSM8625_INT_SDC4_1		(GIC_SPI_START + 32 + 19)
#define MSM8625_INT_UART2DM_IRQ		(GIC_SPI_START + 32 + 20)
#define MSM8625_INT_UART2DM_RX		(GIC_SPI_START + 32 + 21)
#define MSM8625_INT_L2CC_EM		(GIC_SPI_START + 32 + 22)
#define MSM8625_INT_L2CC_INTR		(GIC_SPI_START + 32 + 23)
#define MSM8625_INT_CE_IRQ		(GIC_SPI_START + 32 + 24)
#define MSM8625_INT_CPR_IRQ0		(GIC_SPI_START + 32 + 25)
#define MSM8625_INT_CPR_IRQ1		(GIC_SPI_START + 32 + 26)
#define MSM8625_INT_CPR_IRQ2		(GIC_SPI_START + 32 + 27)

#define MSM8625_INT_ADSP_A11_SMSM	MSM8625_INT_ADSP_A11
#endif
