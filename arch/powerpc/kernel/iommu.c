/*
 * Copyright (C) 2001 Mike Corrigan & Dave Engebretsen, IBM Corporation
 * 
 * Rewrite, cleanup, new allocation schemes, virtual merging: 
 * Copyright (C) 2004 Olof Johansson, IBM Corporation
 *               and  Ben. Herrenschmidt, IBM Corporation
 *
 * Dynamic DMA mapping support, bus-independent parts.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */


#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>
#include <linux/bitmap.h>
#include <linux/iommu-helper.h>
#include <linux/crash_dump.h>
#include <asm/io.h>
#include <asm/prom.h>
#include <asm/iommu.h>
#include <asm/pci-bridge.h>
#include <asm/machdep.h>
#include <asm/kdump.h>
#include <asm/fadump.h>

#define DBG(...)

static int novmerge;

static void __iommu_free(struct iommu_table *, dma_addr_t, unsigned int);

static int __init setup_iommu(char *str)
{
	if (!strcmp(str, "novmerge"))
		novmerge = 1;
	else if (!strcmp(str, "vmerge"))
		novmerge = 0;
	return 1;
}

__setup("iommu=", setup_iommu);

static unsigned long iommu_range_alloc(struct device *dev,
				       struct iommu_table *tbl,
                                       unsigned long npages,
                                       unsigned long *handle,
                                       unsigned long mask,
                                       unsigned int align_order)
{ 
	unsigned long n, end, start;
	unsigned long limit;
	int largealloc = npages > 15;
	int pass = 0;
	unsigned long align_mask;
	unsigned long boundary_size;

	align_mask = 0xffffffffffffffffl >> (64 - align_order);

	/*                                                            */

	/*              */
	if (unlikely(npages == 0)) {
		if (printk_ratelimit())
			WARN_ON(1);
		return DMA_ERROR_CODE;
	}

	if (handle && *handle)
		start = *handle;
	else
		start = largealloc ? tbl->it_largehint : tbl->it_hint;

	/*                                                                */
	limit = largealloc ? tbl->it_size : tbl->it_halfpoint;

	if (largealloc && start < tbl->it_halfpoint)
		start = tbl->it_halfpoint;

	/*                                                              
                                                                 
                                                             
  */
	if (start >= limit)
		start = largealloc ? tbl->it_largehint : tbl->it_hint;

 again:

	if (limit + tbl->it_offset > mask) {
		limit = mask - tbl->it_offset + 1;
		/*                                                 
                                                        
                                    
   */
		if ((start & mask) >= limit || pass > 0)
			start = 0;
		else
			start &= mask;
	}

	if (dev)
		boundary_size = ALIGN(dma_get_seg_boundary(dev) + 1,
				      1 << IOMMU_PAGE_SHIFT);
	else
		boundary_size = ALIGN(1UL << 32, 1 << IOMMU_PAGE_SHIFT);
	/*                                                      */

	n = iommu_area_alloc(tbl->it_map, limit, start, npages,
			     tbl->it_offset, boundary_size >> IOMMU_PAGE_SHIFT,
			     align_mask);
	if (n == -1) {
		if (likely(pass < 2)) {
			/*                                                  
                                                         
    */
			start = (largealloc ^ pass) ? tbl->it_halfpoint : 0;
			limit = pass ? tbl->it_size : limit;
			pass++;
			goto again;
		} else {
			/*                        */
			return DMA_ERROR_CODE;
		}
	}

	end = n + npages;

	/*                                                */
	if (largealloc) {
		/*                                                */
		tbl->it_largehint = end;
	} else {
		/*                                                       */
		tbl->it_hint = (end + tbl->it_blocksize - 1) &
		                ~(tbl->it_blocksize - 1);
	}

	/*                                  */
	if (handle)
		*handle = end;

	return n;
}

static dma_addr_t iommu_alloc(struct device *dev, struct iommu_table *tbl,
			      void *page, unsigned int npages,
			      enum dma_data_direction direction,
			      unsigned long mask, unsigned int align_order,
			      struct dma_attrs *attrs)
{
	unsigned long entry, flags;
	dma_addr_t ret = DMA_ERROR_CODE;
	int build_fail;

	spin_lock_irqsave(&(tbl->it_lock), flags);

	entry = iommu_range_alloc(dev, tbl, npages, NULL, mask, align_order);

	if (unlikely(entry == DMA_ERROR_CODE)) {
		spin_unlock_irqrestore(&(tbl->it_lock), flags);
		return DMA_ERROR_CODE;
	}

	entry += tbl->it_offset;	/*                            */
	ret = entry << IOMMU_PAGE_SHIFT;	/*                            */

	/*                              */
	build_fail = ppc_md.tce_build(tbl, entry, npages,
	                              (unsigned long)page & IOMMU_PAGE_MASK,
	                              direction, attrs);

	/*                                                               
                                                     
                                                             
                
  */
	if (unlikely(build_fail)) {
		__iommu_free(tbl, ret, npages);

		spin_unlock_irqrestore(&(tbl->it_lock), flags);
		return DMA_ERROR_CODE;
	}

	/*                                          */
	if (ppc_md.tce_flush)
		ppc_md.tce_flush(tbl);

	spin_unlock_irqrestore(&(tbl->it_lock), flags);

	/*                                        */
	mb();

	return ret;
}

static void __iommu_free(struct iommu_table *tbl, dma_addr_t dma_addr, 
			 unsigned int npages)
{
	unsigned long entry, free_entry;

	entry = dma_addr >> IOMMU_PAGE_SHIFT;
	free_entry = entry - tbl->it_offset;

	if (((free_entry + npages) > tbl->it_size) ||
	    (entry < tbl->it_offset)) {
		if (printk_ratelimit()) {
			printk(KERN_INFO "iommu_free: invalid entry\n");
			printk(KERN_INFO "\tentry     = 0x%lx\n", entry); 
			printk(KERN_INFO "\tdma_addr  = 0x%llx\n", (u64)dma_addr);
			printk(KERN_INFO "\tTable     = 0x%llx\n", (u64)tbl);
			printk(KERN_INFO "\tbus#      = 0x%llx\n", (u64)tbl->it_busno);
			printk(KERN_INFO "\tsize      = 0x%llx\n", (u64)tbl->it_size);
			printk(KERN_INFO "\tstartOff  = 0x%llx\n", (u64)tbl->it_offset);
			printk(KERN_INFO "\tindex     = 0x%llx\n", (u64)tbl->it_index);
			WARN_ON(1);
		}
		return;
	}

	ppc_md.tce_free(tbl, entry, npages);
	bitmap_clear(tbl->it_map, free_entry, npages);
}

static void iommu_free(struct iommu_table *tbl, dma_addr_t dma_addr,
		unsigned int npages)
{
	unsigned long flags;

	spin_lock_irqsave(&(tbl->it_lock), flags);

	__iommu_free(tbl, dma_addr, npages);

	/*                                                         
                                                              
                          
  */
	if (ppc_md.tce_flush)
		ppc_md.tce_flush(tbl);

	spin_unlock_irqrestore(&(tbl->it_lock), flags);
}

int iommu_map_sg(struct device *dev, struct iommu_table *tbl,
		 struct scatterlist *sglist, int nelems,
		 unsigned long mask, enum dma_data_direction direction,
		 struct dma_attrs *attrs)
{
	dma_addr_t dma_next = 0, dma_addr;
	unsigned long flags;
	struct scatterlist *s, *outs, *segstart;
	int outcount, incount, i, build_fail = 0;
	unsigned int align;
	unsigned long handle;
	unsigned int max_seg_size;

	BUG_ON(direction == DMA_NONE);

	if ((nelems == 0) || !tbl)
		return 0;

	outs = s = segstart = &sglist[0];
	outcount = 1;
	incount = nelems;
	handle = 0;

	/*                                                  */
	outs->dma_length = 0;

	DBG("sg mapping %d elements:\n", nelems);

	spin_lock_irqsave(&(tbl->it_lock), flags);

	max_seg_size = dma_get_max_seg_size(dev);
	for_each_sg(sglist, s, nelems, i) {
		unsigned long vaddr, npages, entry, slen;

		slen = s->length;
		/*              */
		if (slen == 0) {
			dma_next = 0;
			continue;
		}
		/*                                         */
		vaddr = (unsigned long) sg_virt(s);
		npages = iommu_num_pages(vaddr, slen, IOMMU_PAGE_SIZE);
		align = 0;
		if (IOMMU_PAGE_SHIFT < PAGE_SHIFT && slen >= PAGE_SIZE &&
		    (vaddr & ~PAGE_MASK) == 0)
			align = PAGE_SHIFT - IOMMU_PAGE_SHIFT;
		entry = iommu_range_alloc(dev, tbl, npages, &handle,
					  mask >> IOMMU_PAGE_SHIFT, align);

		DBG("  - vaddr: %lx, size: %lx\n", vaddr, slen);

		/*                */
		if (unlikely(entry == DMA_ERROR_CODE)) {
			if (printk_ratelimit())
				dev_info(dev, "iommu_alloc failed, tbl %p "
					 "vaddr %lx npages %lu\n", tbl, vaddr,
					 npages);
			goto failure;
		}

		/*                               */
		entry += tbl->it_offset;
		dma_addr = entry << IOMMU_PAGE_SHIFT;
		dma_addr |= (s->offset & ~IOMMU_PAGE_MASK);

		DBG("  - %lu pages, entry: %lx, dma_addr: %lx\n",
			    npages, entry, dma_addr);

		/*                      */
		build_fail = ppc_md.tce_build(tbl, entry, npages,
		                              vaddr & IOMMU_PAGE_MASK,
		                              direction, attrs);
		if(unlikely(build_fail))
			goto failure;

		/*                                           */
		if (segstart != s) {
			DBG("  - trying merge...\n");
			/*                    
                                                                  
    */
			if (novmerge || (dma_addr != dma_next) ||
			    (outs->dma_length + s->length > max_seg_size)) {
				/*                                   */
				segstart = s;
				outcount++;
				outs = sg_next(outs);
				DBG("    can't merge, new segment.\n");
			} else {
				outs->dma_length += s->length;
				DBG("    merged, new len: %ux\n", outs->dma_length);
			}
		}

		if (segstart == s) {
			/*                                     */
			DBG("  - filling new segment.\n");
			outs->dma_address = dma_addr;
			outs->dma_length = slen;
		}

		/*                                                  */
		dma_next = dma_addr + slen;

		DBG("  - dma next is: %lx\n", dma_next);
	}

	/*                                          */
	if (ppc_md.tce_flush)
		ppc_md.tce_flush(tbl);

	spin_unlock_irqrestore(&(tbl->it_lock), flags);

	DBG("mapped %d elements:\n", outcount);

	/*                                                               
                                                                  
  */
	if (outcount < incount) {
		outs = sg_next(outs);
		outs->dma_address = DMA_ERROR_CODE;
		outs->dma_length = 0;
	}

	/*                                        */
	mb();

	return outcount;

 failure:
	for_each_sg(sglist, s, nelems, i) {
		if (s->dma_length != 0) {
			unsigned long vaddr, npages;

			vaddr = s->dma_address & IOMMU_PAGE_MASK;
			npages = iommu_num_pages(s->dma_address, s->dma_length,
						 IOMMU_PAGE_SIZE);
			__iommu_free(tbl, vaddr, npages);
			s->dma_address = DMA_ERROR_CODE;
			s->dma_length = 0;
		}
		if (s == outs)
			break;
	}
	spin_unlock_irqrestore(&(tbl->it_lock), flags);
	return 0;
}


void iommu_unmap_sg(struct iommu_table *tbl, struct scatterlist *sglist,
		int nelems, enum dma_data_direction direction,
		struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	unsigned long flags;

	BUG_ON(direction == DMA_NONE);

	if (!tbl)
		return;

	spin_lock_irqsave(&(tbl->it_lock), flags);

	sg = sglist;
	while (nelems--) {
		unsigned int npages;
		dma_addr_t dma_handle = sg->dma_address;

		if (sg->dma_length == 0)
			break;
		npages = iommu_num_pages(dma_handle, sg->dma_length,
					 IOMMU_PAGE_SIZE);
		__iommu_free(tbl, dma_handle, npages);
		sg = sg_next(sg);
	}

	/*                                                            
                                                                 
                 
  */
	if (ppc_md.tce_flush)
		ppc_md.tce_flush(tbl);

	spin_unlock_irqrestore(&(tbl->it_lock), flags);
}

static void iommu_table_clear(struct iommu_table *tbl)
{
	/*
                                                               
                                                                  
                                                              
  */
	if (!is_kdump_kernel() || is_fadump_active()) {
		/*                                                         */
		ppc_md.tce_free(tbl, tbl->it_offset, tbl->it_size);
		return;
	}

#ifdef CONFIG_CRASH_DUMP
	if (ppc_md.tce_get) {
		unsigned long index, tceval, tcecount = 0;

		/*                                                         */
		for (index = 0; index < tbl->it_size; index++) {
			tceval = ppc_md.tce_get(tbl, index + tbl->it_offset);
			/*
                                                         
    */
			if (tceval && (tceval != 0x7fffffffffffffffUL)) {
				__set_bit(index, tbl->it_map);
				tcecount++;
			}
		}

		if ((tbl->it_size - tcecount) < KDUMP_MIN_TCE_ENTRIES) {
			printk(KERN_WARNING "TCE table is full; freeing ");
			printk(KERN_WARNING "%d entries for the kdump boot\n",
				KDUMP_MIN_TCE_ENTRIES);
			for (index = tbl->it_size - KDUMP_MIN_TCE_ENTRIES;
				index < tbl->it_size; index++)
				__clear_bit(index, tbl->it_map);
		}
	}
#endif
}

/*
                                                                
                                                 
 */
struct iommu_table *iommu_init_table(struct iommu_table *tbl, int nid)
{
	unsigned long sz;
	static int welcomed = 0;
	struct page *page;

	/*                                                   */
	tbl->it_halfpoint = tbl->it_size * 3 / 4;

	/*                                       */
	sz = (tbl->it_size + 7) >> 3;

	page = alloc_pages_node(nid, GFP_ATOMIC, get_order(sz));
	if (!page)
		panic("iommu_init_table: Can't allocate %ld bytes\n", sz);
	tbl->it_map = page_address(page);
	memset(tbl->it_map, 0, sz);

	/*
                                                           
                                                                
                                           
  */
	if (tbl->it_offset == 0)
		set_bit(0, tbl->it_map);

	tbl->it_hint = 0;
	tbl->it_largehint = tbl->it_halfpoint;
	spin_lock_init(&tbl->it_lock);

	iommu_table_clear(tbl);

	if (!welcomed) {
		printk(KERN_INFO "IOMMU table initialized, virtual merging %s\n",
		       novmerge ? "disabled" : "enabled");
		welcomed = 1;
	}

	return tbl;
}

void iommu_free_table(struct iommu_table *tbl, const char *node_name)
{
	unsigned long bitmap_sz, i;
	unsigned int order;

	if (!tbl || !tbl->it_map) {
		printk(KERN_ERR "%s: expected TCE map for %s\n", __func__,
				node_name);
		return;
	}

	/*                                       */
	/*                                                         */
	for (i = 0; i < (tbl->it_size/64); i++) {
		if (tbl->it_map[i] != 0) {
			printk(KERN_WARNING "%s: Unexpected TCEs for %s\n",
				__func__, node_name);
			break;
		}
	}

	/*                                */
	bitmap_sz = (tbl->it_size + 7) / 8;

	/*             */
	order = get_order(bitmap_sz);
	free_pages((unsigned long) tbl->it_map, order);

	/*            */
	kfree(tbl);
}

/*                                                                  
                                                                         
                                                                     
                                                                         
 */
dma_addr_t iommu_map_page(struct device *dev, struct iommu_table *tbl,
			  struct page *page, unsigned long offset, size_t size,
			  unsigned long mask, enum dma_data_direction direction,
			  struct dma_attrs *attrs)
{
	dma_addr_t dma_handle = DMA_ERROR_CODE;
	void *vaddr;
	unsigned long uaddr;
	unsigned int npages, align;

	BUG_ON(direction == DMA_NONE);

	vaddr = page_address(page) + offset;
	uaddr = (unsigned long)vaddr;
	npages = iommu_num_pages(uaddr, size, IOMMU_PAGE_SIZE);

	if (tbl) {
		align = 0;
		if (IOMMU_PAGE_SHIFT < PAGE_SHIFT && size >= PAGE_SIZE &&
		    ((unsigned long)vaddr & ~PAGE_MASK) == 0)
			align = PAGE_SHIFT - IOMMU_PAGE_SHIFT;

		dma_handle = iommu_alloc(dev, tbl, vaddr, npages, direction,
					 mask >> IOMMU_PAGE_SHIFT, align,
					 attrs);
		if (dma_handle == DMA_ERROR_CODE) {
			if (printk_ratelimit())  {
				dev_info(dev, "iommu_alloc failed, tbl %p "
					 "vaddr %p npages %d\n", tbl, vaddr,
					 npages);
			}
		} else
			dma_handle |= (uaddr & ~IOMMU_PAGE_MASK);
	}

	return dma_handle;
}

void iommu_unmap_page(struct iommu_table *tbl, dma_addr_t dma_handle,
		      size_t size, enum dma_data_direction direction,
		      struct dma_attrs *attrs)
{
	unsigned int npages;

	BUG_ON(direction == DMA_NONE);

	if (tbl) {
		npages = iommu_num_pages(dma_handle, size, IOMMU_PAGE_SIZE);
		iommu_free(tbl, dma_handle, npages);
	}
}

/*                                                                 
                                                                
                                                  
 */
void *iommu_alloc_coherent(struct device *dev, struct iommu_table *tbl,
			   size_t size,	dma_addr_t *dma_handle,
			   unsigned long mask, gfp_t flag, int node)
{
	void *ret = NULL;
	dma_addr_t mapping;
	unsigned int order;
	unsigned int nio_pages, io_order;
	struct page *page;

	size = PAGE_ALIGN(size);
	order = get_order(size);

 	/*
                                                               
                                                               
                   
  */
	if (order >= IOMAP_MAX_ORDER) {
		dev_info(dev, "iommu_alloc_consistent size too large: 0x%lx\n",
			 size);
		return NULL;
	}

	if (!tbl)
		return NULL;

	/*                                        */
	page = alloc_pages_node(node, flag, order);
	if (!page)
		return NULL;
	ret = page_address(page);
	memset(ret, 0, size);

	/*                                          */
	nio_pages = size >> IOMMU_PAGE_SHIFT;
	io_order = get_iommu_order(size);
	mapping = iommu_alloc(dev, tbl, ret, nio_pages, DMA_BIDIRECTIONAL,
			      mask >> IOMMU_PAGE_SHIFT, io_order, NULL);
	if (mapping == DMA_ERROR_CODE) {
		free_pages((unsigned long)ret, order);
		return NULL;
	}
	*dma_handle = mapping;
	return ret;
}

void iommu_free_coherent(struct iommu_table *tbl, size_t size,
			 void *vaddr, dma_addr_t dma_handle)
{
	if (tbl) {
		unsigned int nio_pages;

		size = PAGE_ALIGN(size);
		nio_pages = size >> IOMMU_PAGE_SHIFT;
		iommu_free(tbl, dma_handle, nio_pages);
		size = PAGE_ALIGN(size);
		free_pages((unsigned long)vaddr, get_order(size));
	}
}
