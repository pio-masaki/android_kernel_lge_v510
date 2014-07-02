#ifndef LINUX_BCMA_DRIVER_PCI_H_
#define LINUX_BCMA_DRIVER_PCI_H_

#include <linux/types.h>

struct pci_dev;

/*                       */
#define BCMA_CORE_PCI_CTL			0x0000	/*             */
#define  BCMA_CORE_PCI_CTL_RST_OE		0x00000001 /*                         */
#define  BCMA_CORE_PCI_CTL_RST			0x00000002 /*                             */
#define  BCMA_CORE_PCI_CTL_CLK_OE		0x00000004 /*                          */
#define  BCMA_CORE_PCI_CTL_CLK			0x00000008 /*                                  */
#define BCMA_CORE_PCI_ARBCTL			0x0010	/*                     */
#define  BCMA_CORE_PCI_ARBCTL_INTERN		0x00000001 /*                      */
#define  BCMA_CORE_PCI_ARBCTL_EXTERN		0x00000002 /*                      */
#define  BCMA_CORE_PCI_ARBCTL_PARKID		0x00000006 /*                                                    */
#define   BCMA_CORE_PCI_ARBCTL_PARKID_LAST	0x00000000 /*                */
#define   BCMA_CORE_PCI_ARBCTL_PARKID_4710	0x00000002 /*      */
#define   BCMA_CORE_PCI_ARBCTL_PARKID_EXT0	0x00000004 /*                      */
#define   BCMA_CORE_PCI_ARBCTL_PARKID_EXT1	0x00000006 /*                      */
#define BCMA_CORE_PCI_ISTAT			0x0020	/*                  */
#define  BCMA_CORE_PCI_ISTAT_INTA		0x00000001 /*           */
#define  BCMA_CORE_PCI_ISTAT_INTB		0x00000002 /*           */
#define  BCMA_CORE_PCI_ISTAT_SERR		0x00000004 /*                            */
#define  BCMA_CORE_PCI_ISTAT_PERR		0x00000008 /*                            */
#define  BCMA_CORE_PCI_ISTAT_PME		0x00000010 /*          */
#define BCMA_CORE_PCI_IMASK			0x0024	/*                */
#define  BCMA_CORE_PCI_IMASK_INTA		0x00000001 /*           */
#define  BCMA_CORE_PCI_IMASK_INTB		0x00000002 /*           */
#define  BCMA_CORE_PCI_IMASK_SERR		0x00000004 /*           */
#define  BCMA_CORE_PCI_IMASK_PERR		0x00000008 /*           */
#define  BCMA_CORE_PCI_IMASK_PME		0x00000010 /*          */
#define BCMA_CORE_PCI_MBOX			0x0028	/*                          */
#define  BCMA_CORE_PCI_MBOX_F0_0		0x00000100 /*                       */
#define  BCMA_CORE_PCI_MBOX_F0_1		0x00000200 /*                       */
#define  BCMA_CORE_PCI_MBOX_F1_0		0x00000400 /*                       */
#define  BCMA_CORE_PCI_MBOX_F1_1		0x00000800 /*                       */
#define  BCMA_CORE_PCI_MBOX_F2_0		0x00001000 /*                       */
#define  BCMA_CORE_PCI_MBOX_F2_1		0x00002000 /*                       */
#define  BCMA_CORE_PCI_MBOX_F3_0		0x00004000 /*                       */
#define  BCMA_CORE_PCI_MBOX_F3_1		0x00008000 /*                       */
#define BCMA_CORE_PCI_BCAST_ADDR		0x0050	/*                             */
#define  BCMA_CORE_PCI_BCAST_ADDR_MASK		0x000000FF
#define BCMA_CORE_PCI_BCAST_DATA		0x0054	/*                          */
#define BCMA_CORE_PCI_GPIO_IN			0x0060	/*               */
#define BCMA_CORE_PCI_GPIO_OUT			0x0064	/*               */
#define BCMA_CORE_PCI_GPIO_ENABLE		0x0068	/*               */
#define BCMA_CORE_PCI_GPIO_CTL			0x006C	/*               */
#define BCMA_CORE_PCI_SBTOPCI0			0x0100	/*                                           */
#define  BCMA_CORE_PCI_SBTOPCI0_MASK		0xFC000000
#define BCMA_CORE_PCI_SBTOPCI1			0x0104	/*                                           */
#define  BCMA_CORE_PCI_SBTOPCI1_MASK		0xFC000000
#define BCMA_CORE_PCI_SBTOPCI2			0x0108	/*                                           */
#define  BCMA_CORE_PCI_SBTOPCI2_MASK		0xC0000000
#define BCMA_CORE_PCI_CONFIG_ADDR		0x0120	/*                          */
#define BCMA_CORE_PCI_CONFIG_DATA		0x0124	/*                          */
#define BCMA_CORE_PCI_MDIO_CONTROL		0x0128	/*                          */
#define  BCMA_CORE_PCI_MDIOCTL_DIVISOR_MASK	0x7f	/*                          */
#define  BCMA_CORE_PCI_MDIOCTL_DIVISOR_VAL	0x2
#define  BCMA_CORE_PCI_MDIOCTL_PREAM_EN		0x80	/*                         */
#define  BCMA_CORE_PCI_MDIOCTL_ACCESS_DONE	0x100	/*                     */
#define BCMA_CORE_PCI_MDIO_DATA			0x012c	/*                         */
#define  BCMA_CORE_PCI_MDIODATA_MASK		0x0000ffff /*              */
#define  BCMA_CORE_PCI_MDIODATA_TA		0x00020000 /*            */
#define  BCMA_CORE_PCI_MDIODATA_REGADDR_SHF_OLD	18	/*                          */
#define  BCMA_CORE_PCI_MDIODATA_REGADDR_MASK_OLD	0x003c0000 /*                         */
#define  BCMA_CORE_PCI_MDIODATA_DEVADDR_SHF_OLD	22	/*                                    */
#define  BCMA_CORE_PCI_MDIODATA_DEVADDR_MASK_OLD	0x0fc00000 /*                                   */
#define  BCMA_CORE_PCI_MDIODATA_REGADDR_SHF	18	/*               */
#define  BCMA_CORE_PCI_MDIODATA_REGADDR_MASK	0x007c0000 /*              */
#define  BCMA_CORE_PCI_MDIODATA_DEVADDR_SHF	23	/*                         */
#define  BCMA_CORE_PCI_MDIODATA_DEVADDR_MASK	0x0f800000 /*                        */
#define  BCMA_CORE_PCI_MDIODATA_WRITE		0x10000000 /*                   */
#define  BCMA_CORE_PCI_MDIODATA_READ		0x20000000 /*                  */
#define  BCMA_CORE_PCI_MDIODATA_START		0x40000000 /*                      */
#define  BCMA_CORE_PCI_MDIODATA_DEV_ADDR	0x0	/*                        */
#define  BCMA_CORE_PCI_MDIODATA_BLK_ADDR	0x1F	/*                        */
#define  BCMA_CORE_PCI_MDIODATA_DEV_PLL		0x1d	/*                */
#define  BCMA_CORE_PCI_MDIODATA_DEV_TX		0x1e	/*               */
#define  BCMA_CORE_PCI_MDIODATA_DEV_RX		0x1f	/*               */
#define BCMA_CORE_PCI_PCIEIND_ADDR		0x0130	/*                                          */
#define BCMA_CORE_PCI_PCIEIND_DATA		0x0134	/*                                    */
#define BCMA_CORE_PCI_CLKREQENCTRL		0x0138	/*                                */
#define BCMA_CORE_PCI_PCICFG0			0x0400	/*                               */
#define BCMA_CORE_PCI_PCICFG1			0x0500	/*                               */
#define BCMA_CORE_PCI_PCICFG2			0x0600	/*                               */
#define BCMA_CORE_PCI_PCICFG3			0x0700	/*                               */
#define BCMA_CORE_PCI_SPROM(wordoffset)		(0x0800 + ((wordoffset) * 2)) /*                              */

/*          */
#define BCMA_CORE_PCI_SBTOPCI_MEM		0x00000000
#define BCMA_CORE_PCI_SBTOPCI_IO		0x00000001
#define BCMA_CORE_PCI_SBTOPCI_CFG0		0x00000002
#define BCMA_CORE_PCI_SBTOPCI_CFG1		0x00000003
#define BCMA_CORE_PCI_SBTOPCI_PREF		0x00000004 /*                 */
#define BCMA_CORE_PCI_SBTOPCI_BURST		0x00000008 /*              */
#define BCMA_CORE_PCI_SBTOPCI_MRM		0x00000020 /*                      */
#define BCMA_CORE_PCI_SBTOPCI_RC		0x00000030 /*                               */
#define  BCMA_CORE_PCI_SBTOPCI_RC_READ		0x00000000 /*             */
#define  BCMA_CORE_PCI_SBTOPCI_RC_READL		0x00000010 /*                  */
#define  BCMA_CORE_PCI_SBTOPCI_RC_READM		0x00000020 /*                      */

/*                                        */
#define BCMA_CORE_PCI_PLP_MODEREG		0x200	/*      */
#define BCMA_CORE_PCI_PLP_STATUSREG		0x204	/*        */
#define  BCMA_CORE_PCI_PLP_POLARITYINV_STAT	0x10	/*                               */
#define BCMA_CORE_PCI_PLP_LTSSMCTRLREG		0x208	/*               */
#define BCMA_CORE_PCI_PLP_LTLINKNUMREG		0x20c	/*                           */
#define BCMA_CORE_PCI_PLP_LTLANENUMREG		0x210	/*                           */
#define BCMA_CORE_PCI_PLP_LTNFTSREG		0x214	/*                     */
#define BCMA_CORE_PCI_PLP_ATTNREG		0x218	/*           */
#define BCMA_CORE_PCI_PLP_ATTNMASKREG		0x21C	/*                */
#define BCMA_CORE_PCI_PLP_RXERRCTR		0x220	/*          */
#define BCMA_CORE_PCI_PLP_RXFRMERRCTR		0x224	/*                  */
#define BCMA_CORE_PCI_PLP_RXERRTHRESHREG	0x228	/*                    */
#define BCMA_CORE_PCI_PLP_TESTCTRLREG		0x22C	/*                  */
#define BCMA_CORE_PCI_PLP_SERDESCTRLOVRDREG	0x230	/*                         */
#define BCMA_CORE_PCI_PLP_TIMINGOVRDREG		0x234	/*                       */
#define BCMA_CORE_PCI_PLP_RXTXSMDIAGREG		0x238	/*                         */
#define BCMA_CORE_PCI_PLP_LTSSMDIAGREG		0x23C	/*                          */

/*                                         */
#define BCMA_CORE_PCI_DLLP_LCREG		0x100	/*              */
#define BCMA_CORE_PCI_DLLP_LSREG		0x104	/*             */
#define BCMA_CORE_PCI_DLLP_LAREG		0x108	/*                */
#define  BCMA_CORE_PCI_DLLP_LSREG_LINKUP	(1 << 16)
#define BCMA_CORE_PCI_DLLP_LAMASKREG		0x10C	/*                     */
#define BCMA_CORE_PCI_DLLP_NEXTTXSEQNUMREG	0x110	/*                 */
#define BCMA_CORE_PCI_DLLP_ACKEDTXSEQNUMREG	0x114	/*                  */
#define BCMA_CORE_PCI_DLLP_PURGEDTXSEQNUMREG	0x118	/*                   */
#define BCMA_CORE_PCI_DLLP_RXSEQNUMREG		0x11C	/*                    */
#define BCMA_CORE_PCI_DLLP_LRREG		0x120	/*             */
#define BCMA_CORE_PCI_DLLP_LACKTOREG		0x124	/*                  */
#define BCMA_CORE_PCI_DLLP_PMTHRESHREG		0x128	/*                            */
#define BCMA_CORE_PCI_DLLP_RTRYWPREG		0x12C	/*                        */
#define BCMA_CORE_PCI_DLLP_RTRYRPREG		0x130	/*                       */
#define BCMA_CORE_PCI_DLLP_RTRYPPREG		0x134	/*                         */
#define BCMA_CORE_PCI_DLLP_RTRRWREG		0x138	/*                         */
#define BCMA_CORE_PCI_DLLP_ECTHRESHREG		0x13C	/*                       */
#define BCMA_CORE_PCI_DLLP_TLPERRCTRREG		0x140	/*                   */
#define BCMA_CORE_PCI_DLLP_ERRCTRREG		0x144	/*               */
#define BCMA_CORE_PCI_DLLP_NAKRXCTRREG		0x148	/*                      */
#define BCMA_CORE_PCI_DLLP_TESTREG		0x14C	/*      */
#define BCMA_CORE_PCI_DLLP_PKTBIST		0x150	/*             */
#define BCMA_CORE_PCI_DLLP_PCIE11		0x154	/*                   */

/*                     */
#define BCMA_CORE_PCI_SERDES_RX_CTRL		1	/*          */
#define  BCMA_CORE_PCI_SERDES_RX_CTRL_FORCE	0x80	/*                  */
#define  BCMA_CORE_PCI_SERDES_RX_CTRL_POLARITY	0x40	/*                  */
#define BCMA_CORE_PCI_SERDES_RX_TIMER1		2	/*           */
#define BCMA_CORE_PCI_SERDES_RX_CDR		6	/*     */
#define BCMA_CORE_PCI_SERDES_RX_CDRBW		7	/*        */

/*                      */
#define BCMA_CORE_PCI_SERDES_PLL_CTRL		1	/*                 */
#define BCMA_CORE_PCI_PLL_CTRL_FREQDET_EN	0x4000	/*                      */

/*                             */
#define BCMA_CORE_PCI_BFL_NOPCI			0x00000400 /*                           */

/*                                    */
#define BCMA_CORE_PCI_CFG_BUS_SHIFT		24	/*           */
#define BCMA_CORE_PCI_CFG_SLOT_SHIFT		19	/*                   */
#define BCMA_CORE_PCI_CFG_FUN_SHIFT		16	/*                */
#define BCMA_CORE_PCI_CFG_OFF_SHIFT		0	/*                */

#define BCMA_CORE_PCI_CFG_BUS_MASK		0xff	/*          */
#define BCMA_CORE_PCI_CFG_SLOT_MASK		0x1f	/*                  */
#define BCMA_CORE_PCI_CFG_FUN_MASK		7	/*               */
#define BCMA_CORE_PCI_CFG_OFF_MASK		0xfff	/*               */

/*                                                     */
#define BCMA_CORE_PCI_RC_CRS_VISIBILITY		0x0001

struct bcma_drv_pci;

#ifdef CONFIG_BCMA_DRIVER_PCI_HOSTMODE
struct bcma_drv_pci_host {
	struct bcma_drv_pci *pdev;

	u32 host_cfg_addr;
	spinlock_t cfgspace_lock;

	struct pci_controller pci_controller;
	struct pci_ops pci_ops;
	struct resource mem_resource;
	struct resource io_resource;
};
#endif

struct bcma_drv_pci {
	struct bcma_device *core;
	u8 setup_done:1;
	u8 hostmode:1;

#ifdef CONFIG_BCMA_DRIVER_PCI_HOSTMODE
	struct bcma_drv_pci_host *host_controller;
#endif
};

/*                 */
#define pcicore_read32(pc, offset)		bcma_read32((pc)->core, offset)
#define pcicore_write32(pc, offset, val)	bcma_write32((pc)->core, offset, val)

extern void __devinit bcma_core_pci_init(struct bcma_drv_pci *pc);
extern int bcma_core_pci_irq_ctl(struct bcma_drv_pci *pc,
				 struct bcma_device *core, bool enable);

extern int bcma_core_pci_pcibios_map_irq(const struct pci_dev *dev);
extern int bcma_core_pci_plat_dev_init(struct pci_dev *dev);

#endif /*                          */
