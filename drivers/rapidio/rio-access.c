/*
 * RapidIO configuration space access support
 *
 * Copyright 2005 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/rio.h>
#include <linux/module.h>

/*
                                                             
                                           
 */
static DEFINE_SPINLOCK(rio_config_lock);
static DEFINE_SPINLOCK(rio_doorbell_lock);

/*
                                                                         
                                                                     
                      
 */

#define RIO_8_BAD 0
#define RIO_16_BAD (offset & 1)
#define RIO_32_BAD (offset & 3)

/* 
                                                            
                                                           
                                  
                                                           
  
                                                             
                                                     
 */
#define RIO_LOP_READ(size,type,len) \
int __rio_local_read_config_##size \
	(struct rio_mport *mport, u32 offset, type *value)		\
{									\
	int res;							\
	unsigned long flags;						\
	u32 data = 0;							\
	if (RIO_##size##_BAD) return RIO_BAD_SIZE;			\
	spin_lock_irqsave(&rio_config_lock, flags);			\
	res = mport->ops->lcread(mport, mport->id, offset, len, &data);	\
	*value = (type)data;						\
	spin_unlock_irqrestore(&rio_config_lock, flags);		\
	return res;							\
}

/* 
                                                              
                                                            
                                  
                                                            
  
                                                              
                                                     
 */
#define RIO_LOP_WRITE(size,type,len) \
int __rio_local_write_config_##size \
	(struct rio_mport *mport, u32 offset, type value)		\
{									\
	int res;							\
	unsigned long flags;						\
	if (RIO_##size##_BAD) return RIO_BAD_SIZE;			\
	spin_lock_irqsave(&rio_config_lock, flags);			\
	res = mport->ops->lcwrite(mport, mport->id, offset, len, value);\
	spin_unlock_irqrestore(&rio_config_lock, flags);		\
	return res;							\
}

RIO_LOP_READ(8, u8, 1)
RIO_LOP_READ(16, u16, 2)
RIO_LOP_READ(32, u32, 4)
RIO_LOP_WRITE(8, u8, 1)
RIO_LOP_WRITE(16, u16, 2)
RIO_LOP_WRITE(32, u32, 4)

EXPORT_SYMBOL_GPL(__rio_local_read_config_8);
EXPORT_SYMBOL_GPL(__rio_local_read_config_16);
EXPORT_SYMBOL_GPL(__rio_local_read_config_32);
EXPORT_SYMBOL_GPL(__rio_local_write_config_8);
EXPORT_SYMBOL_GPL(__rio_local_write_config_16);
EXPORT_SYMBOL_GPL(__rio_local_write_config_32);

/* 
                                                           
                                                           
                                  
                                                           
  
                                                             
                                                     
 */
#define RIO_OP_READ(size,type,len) \
int rio_mport_read_config_##size \
	(struct rio_mport *mport, u16 destid, u8 hopcount, u32 offset, type *value)	\
{									\
	int res;							\
	unsigned long flags;						\
	u32 data = 0;							\
	if (RIO_##size##_BAD) return RIO_BAD_SIZE;			\
	spin_lock_irqsave(&rio_config_lock, flags);			\
	res = mport->ops->cread(mport, mport->id, destid, hopcount, offset, len, &data); \
	*value = (type)data;						\
	spin_unlock_irqrestore(&rio_config_lock, flags);		\
	return res;							\
}

/* 
                                                             
                                                            
                                  
                                                            
  
                                                              
                                                     
 */
#define RIO_OP_WRITE(size,type,len) \
int rio_mport_write_config_##size \
	(struct rio_mport *mport, u16 destid, u8 hopcount, u32 offset, type value)	\
{									\
	int res;							\
	unsigned long flags;						\
	if (RIO_##size##_BAD) return RIO_BAD_SIZE;			\
	spin_lock_irqsave(&rio_config_lock, flags);			\
	res = mport->ops->cwrite(mport, mport->id, destid, hopcount, offset, len, value); \
	spin_unlock_irqrestore(&rio_config_lock, flags);		\
	return res;							\
}

RIO_OP_READ(8, u8, 1)
RIO_OP_READ(16, u16, 2)
RIO_OP_READ(32, u32, 4)
RIO_OP_WRITE(8, u8, 1)
RIO_OP_WRITE(16, u16, 2)
RIO_OP_WRITE(32, u32, 4)

EXPORT_SYMBOL_GPL(rio_mport_read_config_8);
EXPORT_SYMBOL_GPL(rio_mport_read_config_16);
EXPORT_SYMBOL_GPL(rio_mport_read_config_32);
EXPORT_SYMBOL_GPL(rio_mport_write_config_8);
EXPORT_SYMBOL_GPL(rio_mport_write_config_16);
EXPORT_SYMBOL_GPL(rio_mport_write_config_32);

/* 
                                                    
  
                          
                                     
                               
  
                                                                
                                                         
 */
int rio_mport_send_doorbell(struct rio_mport *mport, u16 destid, u16 data)
{
	int res;
	unsigned long flags;

	spin_lock_irqsave(&rio_doorbell_lock, flags);
	res = mport->ops->dsend(mport, mport->id, destid, data);
	spin_unlock_irqrestore(&rio_doorbell_lock, flags);

	return res;
}

EXPORT_SYMBOL_GPL(rio_mport_send_doorbell);
