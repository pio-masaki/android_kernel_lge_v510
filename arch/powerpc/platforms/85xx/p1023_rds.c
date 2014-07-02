/*
 * Copyright 2010-2011 Freescale Semiconductor, Inc.
 *
 * Author: Roy Zang <tie-fei.zang@freescale.com>
 *
 * Description:
 * P1023 RDS Board Setup
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/fsl_devices.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include "smp.h"

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include "mpc85xx.h"

/*                                                                         
  
                         
  
 */
static void __init mpc85xx_rds_setup_arch(void)
{
	struct device_node *np;

	if (ppc_md.progress)
		ppc_md.progress("p1023_rds_setup_arch()", 0);

	/*               */
	np = of_find_node_by_name(NULL, "bcsr");
	if (np != NULL) {
		static u8 __iomem *bcsr_regs;

		bcsr_regs = of_iomap(np, 0);
		of_node_put(np);

		if (!bcsr_regs) {
			printk(KERN_ERR
			       "BCSR: Failed to map bcsr register space\n");
			return;
		} else {
#define BCSR15_I2C_BUS0_SEG_CLR		0x07
#define BCSR15_I2C_BUS0_SEG2		0x02
/*
                                           
  
                                                                
                                                               
                                                                
 */
#ifdef CONFIG_RTC_CLASS
			/*                                          */
			clrbits8(&bcsr_regs[15], BCSR15_I2C_BUS0_SEG_CLR);
			setbits8(&bcsr_regs[15], BCSR15_I2C_BUS0_SEG2);
#endif

			iounmap(bcsr_regs);
		}
	}

#ifdef CONFIG_PCI
	for_each_compatible_node(np, "pci", "fsl,p1023-pcie")
		fsl_add_bridge(np, 0);
#endif

	mpc85xx_smp_init();
}

machine_device_initcall(p1023_rds, mpc85xx_common_publish_devices);

static void __init mpc85xx_rds_pic_init(void)
{
	struct mpic *mpic = mpic_alloc(NULL, 0, MPIC_BIG_ENDIAN |
		MPIC_SINGLE_DEST_CPU,
		0, 256, " OpenPIC  ");

	BUG_ON(mpic == NULL);

	mpic_init(mpic);
}

static int __init p1023_rds_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1023RDS");

}

define_machine(p1023_rds) {
	.name			= "P1023 RDS",
	.probe			= p1023_rds_probe,
	.setup_arch		= mpc85xx_rds_setup_arch,
	.init_IRQ		= mpc85xx_rds_pic_init,
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
};

