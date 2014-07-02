/*
 * Driver for the Octeon bootbus compact flash.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 - 2009 Cavium Networks
 * Copyright (C) 2008 Wind River Systems
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/libata.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <scsi/scsi_host.h>

#include <asm/octeon/octeon.h>

/*
                                                                      
                                                           
  
                            
                            
                                                 
  
                                                                     
                                                                        
  
 */

#define DRV_NAME	"pata_octeon_cf"
#define DRV_VERSION	"2.1"


struct octeon_cf_port {
	struct workqueue_struct *wq;
	struct delayed_work delayed_finish;
	struct ata_port *ap;
	int dma_finished;
};

static struct scsi_host_template octeon_cf_sht = {
	ATA_PIO_SHT(DRV_NAME),
};

/* 
                                                       
                                                     
 */
static unsigned int ns_to_tim_reg(unsigned int tim_mult, unsigned int nsecs)
{
	unsigned int val;

	/*
                                                          
                
  */
	val = DIV_ROUND_UP(nsecs * (octeon_get_io_clock_rate() / 1000000),
			  1000 * tim_mult);

	return val;
}

static void octeon_cf_set_boot_reg_cfg(int cs)
{
	union cvmx_mio_boot_reg_cfgx reg_cfg;
	reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(cs));
	reg_cfg.s.dmack = 0;	/*                              */
	reg_cfg.s.tim_mult = 2;	/*                     */
	reg_cfg.s.rd_dly = 0;	/*                                   */
	reg_cfg.s.sam = 0;	/*                                       */
	reg_cfg.s.we_ext = 0;	/*                           */
	reg_cfg.s.oe_ext = 0;	/*                          */
	reg_cfg.s.en = 1;	/*                    */
	reg_cfg.s.orbit = 0;	/*                                    */
	reg_cfg.s.ale = 0;	/*                               */
	cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(cs), reg_cfg.u64);
}

/* 
                                                           
                                                              
                                       
  
                                
                      
 */
static void octeon_cf_set_piomode(struct ata_port *ap, struct ata_device *dev)
{
	struct octeon_cf_data *ocd = ap->dev->platform_data;
	union cvmx_mio_boot_reg_timx reg_tim;
	int cs = ocd->base_region;
	int T;
	struct ata_timing timing;

	int use_iordy;
	int trh;
	int pause;
	/*                                                     */
	int t1;
	int t2;
	int t2i;

	T = (int)(2000000000000LL / octeon_get_clock_rate());

	if (ata_timing_compute(dev, dev->pio_mode, &timing, T, T))
		BUG();

	t1 = timing.setup;
	if (t1)
		t1--;
	t2 = timing.active;
	if (t2)
		t2--;
	t2i = timing.act8b;
	if (t2i)
		t2i--;

	trh = ns_to_tim_reg(2, 20);
	if (trh)
		trh--;

	pause = timing.cycle - timing.active - timing.setup - trh;
	if (pause)
		pause--;

	octeon_cf_set_boot_reg_cfg(cs);
	if (ocd->dma_engine >= 0)
		/*                                            */
		octeon_cf_set_boot_reg_cfg(cs + 1);


	use_iordy = ata_pio_need_iordy(dev);

	reg_tim.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_TIMX(cs));
	/*                   */
	reg_tim.s.pagem = 0;
	/*                       */
	reg_tim.s.waitm = use_iordy;
	/*                    */
	reg_tim.s.pages = 0;
	/*                                       */
	reg_tim.s.ale = 0;
	/*          */
	reg_tim.s.page = 0;
	/*                                                */
	reg_tim.s.wait = 0;
	/*                                     */
	reg_tim.s.pause = pause;
	/*                                                 */
	reg_tim.s.wr_hld = trh;
	/*                                                */
	reg_tim.s.rd_hld = trh;
	/*                                   */
	reg_tim.s.we = t2;
	/*                                  */
	reg_tim.s.oe = t2;
	/*                                      */
	reg_tim.s.ce = ns_to_tim_reg(2, 5);
	/*                                      */
	reg_tim.s.adr = 0;

	/*                                                                  */
	cvmx_write_csr(CVMX_MIO_BOOT_REG_TIMX(cs), reg_tim.u64);
	if (ocd->dma_engine >= 0)
		/*                                            */
		cvmx_write_csr(CVMX_MIO_BOOT_REG_TIMX(cs + 1), reg_tim.u64);
}

static void octeon_cf_set_dmamode(struct ata_port *ap, struct ata_device *dev)
{
	struct octeon_cf_data *ocd = dev->link->ap->dev->platform_data;
	union cvmx_mio_boot_dma_timx dma_tim;
	unsigned int oe_a;
	unsigned int oe_n;
	unsigned int dma_ackh;
	unsigned int dma_arq;
	unsigned int pause;
	unsigned int T0, Tkr, Td;
	unsigned int tim_mult;

	const struct ata_timing *timing;

	timing = ata_timing_find_mode(dev->dma_mode);
	T0	= timing->cycle;
	Td	= timing->active;
	Tkr	= timing->recover;
	dma_ackh = timing->dmack_hold;

	dma_tim.u64 = 0;
	/*                               */
	tim_mult = 4;

	/*                                                         */
	dma_arq = 8;
	pause = 25 - dma_arq * 1000 /
		(octeon_get_clock_rate() / 1000000); /*    */

	oe_a = Td;
	/*                                         */
	oe_n = max(T0 - oe_a, Tkr);

	dma_tim.s.dmack_pi = 1;

	dma_tim.s.oe_n = ns_to_tim_reg(tim_mult, oe_n);
	dma_tim.s.oe_a = ns_to_tim_reg(tim_mult, oe_a);

	/*
                                                            
                       
  */
	dma_tim.s.dmack_s = ns_to_tim_reg(tim_mult, 20);
	dma_tim.s.dmack_h = ns_to_tim_reg(tim_mult, dma_ackh);

	dma_tim.s.dmarq = dma_arq;
	dma_tim.s.pause = ns_to_tim_reg(tim_mult, pause);

	dma_tim.s.rd_dly = 0;	/*                      */

	/*              */
	dma_tim.s.we_n = ns_to_tim_reg(tim_mult, oe_n);
	dma_tim.s.we_a = ns_to_tim_reg(tim_mult, oe_a);

	pr_debug("ns to ticks (mult %d) of %d is: %d\n", tim_mult, 60,
		 ns_to_tim_reg(tim_mult, 60));
	pr_debug("oe_n: %d, oe_a: %d, dmack_s: %d, dmack_h: "
		 "%d, dmarq: %d, pause: %d\n",
		 dma_tim.s.oe_n, dma_tim.s.oe_a, dma_tim.s.dmack_s,
		 dma_tim.s.dmack_h, dma_tim.s.dmarq, dma_tim.s.pause);

	cvmx_write_csr(CVMX_MIO_BOOT_DMA_TIMX(ocd->dma_engine),
		       dma_tim.u64);

}

/* 
                               
  
                                
                           
                                     
                              
 */
static unsigned int octeon_cf_data_xfer8(struct ata_device *dev,
					 unsigned char *buffer,
					 unsigned int buflen,
					 int rw)
{
	struct ata_port *ap		= dev->link->ap;
	void __iomem *data_addr		= ap->ioaddr.data_addr;
	unsigned long words;
	int count;

	words = buflen;
	if (rw) {
		count = 16;
		while (words--) {
			iowrite8(*buffer, data_addr);
			buffer++;
			/*
                                              
                           
    */
			if (--count == 0) {
				ioread8(ap->ioaddr.altstatus_addr);
				count = 16;
			}
		}
	} else {
		ioread8_rep(data_addr, buffer, words);
	}
	return buflen;
}

/* 
                               
  
                                
                           
                                     
                              
 */
static unsigned int octeon_cf_data_xfer16(struct ata_device *dev,
					  unsigned char *buffer,
					  unsigned int buflen,
					  int rw)
{
	struct ata_port *ap		= dev->link->ap;
	void __iomem *data_addr		= ap->ioaddr.data_addr;
	unsigned long words;
	int count;

	words = buflen / 2;
	if (rw) {
		count = 16;
		while (words--) {
			iowrite16(*(uint16_t *)buffer, data_addr);
			buffer += sizeof(uint16_t);
			/*
                                              
                           
    */
			if (--count == 0) {
				ioread8(ap->ioaddr.altstatus_addr);
				count = 16;
			}
		}
	} else {
		while (words--) {
			*(uint16_t *)buffer = ioread16(data_addr);
			buffer += sizeof(uint16_t);
		}
	}
	/*                                   */
	if (unlikely(buflen & 0x01)) {
		__le16 align_buf[1] = { 0 };

		if (rw == READ) {
			align_buf[0] = cpu_to_le16(ioread16(data_addr));
			memcpy(buffer, align_buf, 1);
		} else {
			memcpy(align_buf, buffer, 1);
			iowrite16(le16_to_cpu(align_buf[0]), data_addr);
		}
		words++;
	}
	return buflen;
}

/* 
                                                 
 */
static void octeon_cf_tf_read16(struct ata_port *ap, struct ata_taskfile *tf)
{
	u16 blob;
	/*                                                   */
	void __iomem *base = ap->ioaddr.data_addr;

	blob = __raw_readw(base + 0xc);
	tf->feature = blob >> 8;

	blob = __raw_readw(base + 2);
	tf->nsect = blob & 0xff;
	tf->lbal = blob >> 8;

	blob = __raw_readw(base + 4);
	tf->lbam = blob & 0xff;
	tf->lbah = blob >> 8;

	blob = __raw_readw(base + 6);
	tf->device = blob & 0xff;
	tf->command = blob >> 8;

	if (tf->flags & ATA_TFLAG_LBA48) {
		if (likely(ap->ioaddr.ctl_addr)) {
			iowrite8(tf->ctl | ATA_HOB, ap->ioaddr.ctl_addr);

			blob = __raw_readw(base + 0xc);
			tf->hob_feature = blob >> 8;

			blob = __raw_readw(base + 2);
			tf->hob_nsect = blob & 0xff;
			tf->hob_lbal = blob >> 8;

			blob = __raw_readw(base + 4);
			tf->hob_lbam = blob & 0xff;
			tf->hob_lbah = blob >> 8;

			iowrite8(tf->ctl, ap->ioaddr.ctl_addr);
			ap->last_ctl = tf->ctl;
		} else {
			WARN_ON(1);
		}
	}
}

static u8 octeon_cf_check_status16(struct ata_port *ap)
{
	u16 blob;
	void __iomem *base = ap->ioaddr.data_addr;

	blob = __raw_readw(base + 6);
	return blob >> 8;
}

static int octeon_cf_softreset16(struct ata_link *link, unsigned int *classes,
				 unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	void __iomem *base = ap->ioaddr.data_addr;
	int rc;
	u8 err;

	DPRINTK("about to softreset\n");
	__raw_writew(ap->ctl, base + 0xe);
	udelay(20);
	__raw_writew(ap->ctl | ATA_SRST, base + 0xe);
	udelay(20);
	__raw_writew(ap->ctl, base + 0xe);

	rc = ata_sff_wait_after_reset(link, 1, deadline);
	if (rc) {
		ata_link_err(link, "SRST failed (errno=%d)\n", rc);
		return rc;
	}

	/*                                                             */
	classes[0] = ata_sff_dev_classify(&link->device[0], 1, &err);
	DPRINTK("EXIT, classes[0]=%u [1]=%u\n", classes[0], classes[1]);
	return 0;
}

/* 
                                                                     
                                                              
 */
static void octeon_cf_tf_load16(struct ata_port *ap,
				const struct ata_taskfile *tf)
{
	unsigned int is_addr = tf->flags & ATA_TFLAG_ISADDR;
	/*                                                   */
	void __iomem *base = ap->ioaddr.data_addr;

	if (tf->ctl != ap->last_ctl) {
		iowrite8(tf->ctl, ap->ioaddr.ctl_addr);
		ap->last_ctl = tf->ctl;
		ata_wait_idle(ap);
	}
	if (is_addr && (tf->flags & ATA_TFLAG_LBA48)) {
		__raw_writew(tf->hob_feature << 8, base + 0xc);
		__raw_writew(tf->hob_nsect | tf->hob_lbal << 8, base + 2);
		__raw_writew(tf->hob_lbam | tf->hob_lbah << 8, base + 4);
		VPRINTK("hob: feat 0x%X nsect 0x%X, lba 0x%X 0x%X 0x%X\n",
			tf->hob_feature,
			tf->hob_nsect,
			tf->hob_lbal,
			tf->hob_lbam,
			tf->hob_lbah);
	}
	if (is_addr) {
		__raw_writew(tf->feature << 8, base + 0xc);
		__raw_writew(tf->nsect | tf->lbal << 8, base + 2);
		__raw_writew(tf->lbam | tf->lbah << 8, base + 4);
		VPRINTK("feat 0x%X nsect 0x%X, lba 0x%X 0x%X 0x%X\n",
			tf->feature,
			tf->nsect,
			tf->lbal,
			tf->lbam,
			tf->lbah);
	}
	ata_wait_idle(ap);
}


static void octeon_cf_dev_select(struct ata_port *ap, unsigned int device)
{
/*                                        */
	return;
}

/*
                                                                      
                                                              
 */
static void octeon_cf_exec_command16(struct ata_port *ap,
				const struct ata_taskfile *tf)
{
	/*                                                   */
	void __iomem *base = ap->ioaddr.data_addr;
	u16 blob;

	if (tf->flags & ATA_TFLAG_DEVICE) {
		VPRINTK("device 0x%X\n", tf->device);
		blob = tf->device;
	} else {
		blob = 0;
	}

	DPRINTK("ata%u: cmd 0x%X\n", ap->print_id, tf->command);
	blob |= (tf->command << 8);
	__raw_writew(blob, base + 6);


	ata_wait_idle(ap);
}

static void octeon_cf_irq_on(struct ata_port *ap)
{
}

static void octeon_cf_irq_clear(struct ata_port *ap)
{
	return;
}

static void octeon_cf_dma_setup(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;
	struct octeon_cf_port *cf_port;

	cf_port = ap->private_data;
	DPRINTK("ENTER\n");
	/*                   */
	qc->cursg = qc->sg;
	cf_port->dma_finished = 0;
	ap->ops->sff_exec_command(ap, &qc->tf);
	DPRINTK("EXIT\n");
}

/* 
                                              
  
                                     
 */
static void octeon_cf_dma_start(struct ata_queued_cmd *qc)
{
	struct octeon_cf_data *ocd = qc->ap->dev->platform_data;
	union cvmx_mio_boot_dma_cfgx mio_boot_dma_cfg;
	union cvmx_mio_boot_dma_intx mio_boot_dma_int;
	struct scatterlist *sg;

	VPRINTK("%d scatterlists\n", qc->n_elem);

	/*                                                */
	sg = qc->cursg;
	BUG_ON(!sg);

	/*
                                  
  */
	mio_boot_dma_int.u64 = 0;
	mio_boot_dma_int.s.done = 1;
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(ocd->dma_engine),
		       mio_boot_dma_int.u64);

	/*                        */
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_INT_ENX(ocd->dma_engine),
		       mio_boot_dma_int.u64);

	/*                              */
	mio_boot_dma_cfg.u64 = 0;
	mio_boot_dma_cfg.s.en = 1;
	mio_boot_dma_cfg.s.rw = ((qc->tf.flags & ATA_TFLAG_WRITE) != 0);

	/*
                                                          
                                                           
                                                              
                                                              
                                                               
         
  */
	mio_boot_dma_cfg.s.clr = 0;

	/*                                                         */
	mio_boot_dma_cfg.s.size = sg_dma_len(sg) / 2 - 1;

	/*                                                         */
	mio_boot_dma_cfg.s.swap8 = 1;

	mio_boot_dma_cfg.s.adr = sg_dma_address(sg);

	VPRINTK("%s %d bytes address=%p\n",
		(mio_boot_dma_cfg.s.rw) ? "write" : "read", sg->length,
		(void *)(unsigned long)mio_boot_dma_cfg.s.adr);

	cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(ocd->dma_engine),
		       mio_boot_dma_cfg.u64);
}

/* 
  
           
                               
  
 */
static unsigned int octeon_cf_dma_finished(struct ata_port *ap,
					struct ata_queued_cmd *qc)
{
	struct ata_eh_info *ehi = &ap->link.eh_info;
	struct octeon_cf_data *ocd = ap->dev->platform_data;
	union cvmx_mio_boot_dma_cfgx dma_cfg;
	union cvmx_mio_boot_dma_intx dma_int;
	struct octeon_cf_port *cf_port;
	u8 status;

	VPRINTK("ata%u: protocol %d task_state %d\n",
		ap->print_id, qc->tf.protocol, ap->hsm_task_state);


	if (ap->hsm_task_state != HSM_ST_LAST)
		return 0;

	cf_port = ap->private_data;

	dma_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_DMA_CFGX(ocd->dma_engine));
	if (dma_cfg.s.size != 0xfffff) {
		/*                                        */
		qc->err_mask |= AC_ERR_HOST_BUS;
		ap->hsm_task_state = HSM_ST_ERR;
	}

	/*                                 */
	dma_cfg.u64 = 0;
	dma_cfg.s.size = -1;
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(ocd->dma_engine), dma_cfg.u64);

	/*                         */
	dma_int.u64 = 0;
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_INT_ENX(ocd->dma_engine), dma_int.u64);

	/*                               */
	dma_int.s.done = 1;
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(ocd->dma_engine), dma_int.u64);

	status = ap->ops->sff_check_status(ap);

	ata_sff_hsm_move(ap, qc, status, 0);

	if (unlikely(qc->err_mask) && (qc->tf.protocol == ATA_PROT_DMA))
		ata_ehi_push_desc(ehi, "DMA stat 0x%x", status);

	return 1;
}

/*
                                                                    
                                              
 */
static irqreturn_t octeon_cf_interrupt(int irq, void *dev_instance)
{
	struct ata_host *host = dev_instance;
	struct octeon_cf_port *cf_port;
	int i;
	unsigned int handled = 0;
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);

	DPRINTK("ENTER\n");
	for (i = 0; i < host->n_ports; i++) {
		u8 status;
		struct ata_port *ap;
		struct ata_queued_cmd *qc;
		union cvmx_mio_boot_dma_intx dma_int;
		union cvmx_mio_boot_dma_cfgx dma_cfg;
		struct octeon_cf_data *ocd;

		ap = host->ports[i];
		ocd = ap->dev->platform_data;
		cf_port = ap->private_data;
		dma_int.u64 =
			cvmx_read_csr(CVMX_MIO_BOOT_DMA_INTX(ocd->dma_engine));
		dma_cfg.u64 =
			cvmx_read_csr(CVMX_MIO_BOOT_DMA_CFGX(ocd->dma_engine));

		qc = ata_qc_from_tag(ap, ap->link.active_tag);

		if (qc && !(qc->tf.flags & ATA_TFLAG_POLLING)) {
			if (dma_int.s.done && !dma_cfg.s.en) {
				if (!sg_is_last(qc->cursg)) {
					qc->cursg = sg_next(qc->cursg);
					handled = 1;
					octeon_cf_dma_start(qc);
					continue;
				} else {
					cf_port->dma_finished = 1;
				}
			}
			if (!cf_port->dma_finished)
				continue;
			status = ioread8(ap->ioaddr.altstatus_addr);
			if (status & (ATA_BUSY | ATA_DRQ)) {
				/*
                                    
                                       
                                     
                                      
                               
     */
				/*                */
				dma_int.u64 = 0;
				dma_int.s.done = 1;
				cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(ocd->dma_engine),
					       dma_int.u64);

				queue_delayed_work(cf_port->wq,
						   &cf_port->delayed_finish, 1);
				handled = 1;
			} else {
				handled |= octeon_cf_dma_finished(ap, qc);
			}
		}
	}
	spin_unlock_irqrestore(&host->lock, flags);
	DPRINTK("EXIT\n");
	return IRQ_RETVAL(handled);
}

static void octeon_cf_delayed_finish(struct work_struct *work)
{
	struct octeon_cf_port *cf_port = container_of(work,
						      struct octeon_cf_port,
						      delayed_finish.work);
	struct ata_port *ap = cf_port->ap;
	struct ata_host *host = ap->host;
	struct ata_queued_cmd *qc;
	unsigned long flags;
	u8 status;

	spin_lock_irqsave(&host->lock, flags);

	/*
                                                           
                                                 
                            
  */
	if (ap->hsm_task_state != HSM_ST_LAST || !cf_port->dma_finished)
		goto out;

	status = ioread8(ap->ioaddr.altstatus_addr);
	if (status & (ATA_BUSY | ATA_DRQ)) {
		/*                        */
		queue_delayed_work(cf_port->wq,
				   &cf_port->delayed_finish, 1);
		goto out;
	}
	qc = ata_qc_from_tag(ap, ap->link.active_tag);
	if (qc && !(qc->tf.flags & ATA_TFLAG_POLLING))
		octeon_cf_dma_finished(ap, qc);
out:
	spin_unlock_irqrestore(&host->lock, flags);
}

static void octeon_cf_dev_config(struct ata_device *dev)
{
	/*
                                                            
                                                           
                                                            
  */
	dev->max_sectors = min(dev->max_sectors, 4095U);
}

/*
                                     
 */
static int octeon_cf_check_atapi_dma(struct ata_queued_cmd *qc)
{
	return 0;
}

static unsigned int octeon_cf_qc_issue(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;

	switch (qc->tf.protocol) {
	case ATA_PROT_DMA:
		WARN_ON(qc->tf.flags & ATA_TFLAG_POLLING);

		ap->ops->sff_tf_load(ap, &qc->tf);  /*                   */
		octeon_cf_dma_setup(qc);	    /*            */
		octeon_cf_dma_start(qc);	    /*              */
		ap->hsm_task_state = HSM_ST_LAST;
		break;

	case ATAPI_PROT_DMA:
		dev_err(ap->dev, "Error, ATAPI not supported\n");
		BUG();

	default:
		return ata_sff_qc_issue(qc);
	}

	return 0;
}

static struct ata_port_operations octeon_cf_ops = {
	.inherits		= &ata_sff_port_ops,
	.check_atapi_dma	= octeon_cf_check_atapi_dma,
	.qc_prep		= ata_noop_qc_prep,
	.qc_issue		= octeon_cf_qc_issue,
	.sff_dev_select		= octeon_cf_dev_select,
	.sff_irq_on		= octeon_cf_irq_on,
	.sff_irq_clear		= octeon_cf_irq_clear,
	.cable_detect		= ata_cable_40wire,
	.set_piomode		= octeon_cf_set_piomode,
	.set_dmamode		= octeon_cf_set_dmamode,
	.dev_config		= octeon_cf_dev_config,
};

static int __devinit octeon_cf_probe(struct platform_device *pdev)
{
	struct resource *res_cs0, *res_cs1;

	void __iomem *cs0;
	void __iomem *cs1 = NULL;
	struct ata_host *host;
	struct ata_port *ap;
	struct octeon_cf_data *ocd;
	int irq = 0;
	irq_handler_t irq_handler = NULL;
	void __iomem *base;
	struct octeon_cf_port *cf_port;
	char version[32];

	res_cs0 = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!res_cs0)
		return -EINVAL;

	ocd = pdev->dev.platform_data;

	cs0 = devm_ioremap_nocache(&pdev->dev, res_cs0->start,
				   resource_size(res_cs0));

	if (!cs0)
		return -ENOMEM;

	/*                                                            */
	if (ocd->dma_engine >= 0) {
		res_cs1 = platform_get_resource(pdev, IORESOURCE_MEM, 1);
		if (!res_cs1)
			return -EINVAL;

		cs1 = devm_ioremap_nocache(&pdev->dev, res_cs1->start,
					   resource_size(res_cs1));

		if (!cs1)
			return -ENOMEM;
	}

	cf_port = kzalloc(sizeof(*cf_port), GFP_KERNEL);
	if (!cf_port)
		return -ENOMEM;

	/*               */
	host = ata_host_alloc(&pdev->dev, 1);
	if (!host)
		goto free_cf_port;

	ap = host->ports[0];
	ap->private_data = cf_port;
	cf_port->ap = ap;
	ap->ops = &octeon_cf_ops;
	ap->pio_mask = ATA_PIO6;
	ap->flags |= ATA_FLAG_NO_ATAPI | ATA_FLAG_PIO_POLLING;

	base = cs0 + ocd->base_region_bias;
	if (!ocd->is16bit) {
		ap->ioaddr.cmd_addr	= base;
		ata_sff_std_ports(&ap->ioaddr);

		ap->ioaddr.altstatus_addr = base + 0xe;
		ap->ioaddr.ctl_addr	= base + 0xe;
		octeon_cf_ops.sff_data_xfer = octeon_cf_data_xfer8;
	} else if (cs1) {
		/*                                           */
		ap->ioaddr.cmd_addr	= base + (ATA_REG_CMD << 1) + 1;
		ap->ioaddr.data_addr	= base + (ATA_REG_DATA << 1);
		ap->ioaddr.error_addr	= base + (ATA_REG_ERR << 1) + 1;
		ap->ioaddr.feature_addr	= base + (ATA_REG_FEATURE << 1) + 1;
		ap->ioaddr.nsect_addr	= base + (ATA_REG_NSECT << 1) + 1;
		ap->ioaddr.lbal_addr	= base + (ATA_REG_LBAL << 1) + 1;
		ap->ioaddr.lbam_addr	= base + (ATA_REG_LBAM << 1) + 1;
		ap->ioaddr.lbah_addr	= base + (ATA_REG_LBAH << 1) + 1;
		ap->ioaddr.device_addr	= base + (ATA_REG_DEVICE << 1) + 1;
		ap->ioaddr.status_addr	= base + (ATA_REG_STATUS << 1) + 1;
		ap->ioaddr.command_addr	= base + (ATA_REG_CMD << 1) + 1;
		ap->ioaddr.altstatus_addr = cs1 + (6 << 1) + 1;
		ap->ioaddr.ctl_addr	= cs1 + (6 << 1) + 1;
		octeon_cf_ops.sff_data_xfer = octeon_cf_data_xfer16;

		ap->mwdma_mask	= ATA_MWDMA4;
		irq = platform_get_irq(pdev, 0);
		irq_handler = octeon_cf_interrupt;

		/*                                                         */
		cf_port->wq = create_singlethread_workqueue(DRV_NAME);
		if (!cf_port->wq)
			goto free_cf_port;
		INIT_DELAYED_WORK(&cf_port->delayed_finish,
				  octeon_cf_delayed_finish);

	} else {
		/*                         */
		octeon_cf_ops.sff_data_xfer	= octeon_cf_data_xfer16;
		octeon_cf_ops.softreset		= octeon_cf_softreset16;
		octeon_cf_ops.sff_check_status	= octeon_cf_check_status16;
		octeon_cf_ops.sff_tf_read	= octeon_cf_tf_read16;
		octeon_cf_ops.sff_tf_load	= octeon_cf_tf_load16;
		octeon_cf_ops.sff_exec_command	= octeon_cf_exec_command16;

		ap->ioaddr.data_addr	= base + ATA_REG_DATA;
		ap->ioaddr.nsect_addr	= base + ATA_REG_NSECT;
		ap->ioaddr.lbal_addr	= base + ATA_REG_LBAL;
		ap->ioaddr.ctl_addr	= base + 0xe;
		ap->ioaddr.altstatus_addr = base + 0xe;
	}

	ata_port_desc(ap, "cmd %p ctl %p", base, ap->ioaddr.ctl_addr);


	snprintf(version, sizeof(version), "%s %d bit%s",
		 DRV_VERSION,
		 (ocd->is16bit) ? 16 : 8,
		 (cs1) ? ", True IDE" : "");
	ata_print_version_once(&pdev->dev, version);

	return ata_host_activate(host, irq, irq_handler, 0, &octeon_cf_sht);

free_cf_port:
	kfree(cf_port);
	return -ENOMEM;
}

static struct platform_driver octeon_cf_driver = {
	.probe		= octeon_cf_probe,
	.driver		= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init octeon_cf_init(void)
{
	return platform_driver_register(&octeon_cf_driver);
}


MODULE_AUTHOR("David Daney <ddaney@caviumnetworks.com>");
MODULE_DESCRIPTION("low-level driver for Cavium OCTEON Compact Flash PATA");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("platform:" DRV_NAME);

module_init(octeon_cf_init);
