
/*
 * This file contains defines for the
 *   Micro Memory MM5415
 * family PCI Memory Module with Battery Backup.
 *
 * Copyright Micro Memory INC 2001.  All rights reserved.
 * Release under the terms of the GNU GENERAL PUBLIC LICENSE version 2.
 * See the file COPYING.
 */

#ifndef _DRIVERS_BLOCK_MM_H
#define _DRIVERS_BLOCK_MM_H


#define IRQ_TIMEOUT (1 * HZ)

/*                         */
#define MEMCTRLSTATUS_MAGIC	0x00
#define  MM_MAGIC_VALUE		(unsigned char)0x59

#define MEMCTRLSTATUS_BATTERY	0x04
#define  BATTERY_1_DISABLED	0x01
#define  BATTERY_1_FAILURE	0x02
#define  BATTERY_2_DISABLED	0x04
#define  BATTERY_2_FAILURE	0x08

#define MEMCTRLSTATUS_MEMORY	0x07
#define  MEM_128_MB		0xfe
#define  MEM_256_MB		0xfc
#define  MEM_512_MB		0xf8
#define  MEM_1_GB		0xf0
#define  MEM_2_GB		0xe0

#define MEMCTRLCMD_LEDCTRL	0x08
#define  LED_REMOVE		2
#define  LED_FAULT		4
#define  LED_POWER		6
#define	 LED_FLIP		255
#define  LED_OFF		0x00
#define  LED_ON			0x01
#define  LED_FLASH_3_5		0x02
#define  LED_FLASH_7_0		0x03
#define  LED_POWER_ON		0x00
#define  LED_POWER_OFF		0x01
#define  USER_BIT1		0x01
#define  USER_BIT2		0x02

#define MEMORY_INITIALIZED	USER_BIT1

#define MEMCTRLCMD_ERRCTRL	0x0C
#define  EDC_NONE_DEFAULT	0x00
#define  EDC_NONE		0x01
#define  EDC_STORE_READ		0x02
#define  EDC_STORE_CORRECT	0x03

#define MEMCTRLCMD_ERRCNT	0x0D
#define MEMCTRLCMD_ERRSTATUS	0x0E

#define ERROR_DATA_LOG		0x20
#define ERROR_ADDR_LOG		0x28
#define ERROR_COUNT		0x3D
#define ERROR_SYNDROME		0x3E
#define ERROR_CHECK		0x3F

#define DMA_PCI_ADDR		0x40
#define DMA_LOCAL_ADDR		0x48
#define DMA_TRANSFER_SIZE	0x50
#define DMA_DESCRIPTOR_ADDR	0x58
#define DMA_SEMAPHORE_ADDR	0x60
#define DMA_STATUS_CTRL		0x68
#define  DMASCR_GO		0x00001
#define  DMASCR_TRANSFER_READ	0x00002
#define  DMASCR_CHAIN_EN	0x00004
#define  DMASCR_SEM_EN		0x00010
#define  DMASCR_DMA_COMP_EN	0x00020
#define  DMASCR_CHAIN_COMP_EN	0x00040
#define  DMASCR_ERR_INT_EN	0x00080
#define  DMASCR_PARITY_INT_EN	0x00100
#define  DMASCR_ANY_ERR		0x00800
#define  DMASCR_MBE_ERR		0x01000
#define  DMASCR_PARITY_ERR_REP	0x02000
#define  DMASCR_PARITY_ERR_DET	0x04000
#define  DMASCR_SYSTEM_ERR_SIG	0x08000
#define  DMASCR_TARGET_ABT	0x10000
#define  DMASCR_MASTER_ABT	0x20000
#define  DMASCR_DMA_COMPLETE	0x40000
#define  DMASCR_CHAIN_COMPLETE	0x80000

/*
                                                                     
                                                                   
                                                                          
                                                                      
                                                                          
                                                                          
                    
*/
#define        DMASCR_READ   0x60000000
#define        DMASCR_READLINE   0xE0000000
#define        DMASCR_READMULTI   0xC0000000


#define DMASCR_ERROR_MASK	(DMASCR_MASTER_ABT | DMASCR_TARGET_ABT | DMASCR_SYSTEM_ERR_SIG | DMASCR_PARITY_ERR_DET | DMASCR_MBE_ERR | DMASCR_ANY_ERR)
#define DMASCR_HARD_ERROR	(DMASCR_MASTER_ABT | DMASCR_TARGET_ABT | DMASCR_SYSTEM_ERR_SIG | DMASCR_PARITY_ERR_DET | DMASCR_MBE_ERR)

#define WINDOWMAP_WINNUM	0x7B

#define DMA_READ_FROM_HOST 0
#define DMA_WRITE_TO_HOST 1

struct mm_dma_desc {
	__le64	pci_addr;
	__le64	local_addr;
	__le32	transfer_size;
	u32	zero1;
	__le64	next_desc_addr;
	__le64	sem_addr;
	__le32	control_bits;
	u32	zero2;

	dma_addr_t data_dma_handle;

	/*                  */
	__le64	sem_control_bits;
} __attribute__((aligned(8)));

/*                      */
#define UM_FLAG_DMA_IN_REGS		1
#define UM_FLAG_NO_BYTE_STATUS		2
#define UM_FLAG_NO_BATTREG		4
#define	UM_FLAG_NO_BATT			8
#endif
