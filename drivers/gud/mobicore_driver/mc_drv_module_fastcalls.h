/**
 * Header file of MobiCore Driver Kernel Module.
 *
 * @addtogroup MobiCore_Driver_Kernel_Module
 * @{
 * Internal structures of the McDrvModule
 * @file
 *
 * MobiCore Fast Call interface
 *
 * <!-- Copyright Giesecke & Devrient GmbH 2009-2012 -->
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _MC_DRV_MODULE_FC_H_
#define _MC_DRV_MODULE_FC_H_

#include "mc_drv_module.h"

/* 
                
 */
enum mc_smc_codes {
	MC_SMC_N_YIELD  = 0x3, /*                                    */
	MC_SMC_N_SIQ    = 0x4  /*                                  */
};

/* 
                                             
 */
enum mc_fast_call_codes {
	MC_FC_INIT      = -1,
	MC_FC_INFO      = -2,
	MC_FC_POWER     = -3,
	MC_FC_DUMP      = -4,
	MC_FC_NWD_TRACE = -31 /*                            */
};

/* 
                             
 */
enum mc_fast_calls_result {
	MC_FC_RET_OK                       = 0,
	MC_FC_RET_ERR_INVALID              = 1,
	MC_FC_RET_ERR_ALREADY_INITIALIZED  = 5
};



/*                                                                              
                                          
                                                                              */

/*                               */
union fc_generic {
	struct {
		uint32_t cmd;
		uint32_t param[3];
	} as_in;
	struct {
		uint32_t resp;
		uint32_t ret;
		uint32_t param[2];
	} as_out;
};


/*                 */
union mc_fc_init {
	union fc_generic as_generic;
	struct {
		uint32_t cmd;
		uint32_t base;
		uint32_t nq_info;
		uint32_t mcp_info;
	} as_in;
	struct {
		uint32_t resp;
		uint32_t ret;
		uint32_t rfu[2];
	} as_out;
};


/*                            */
union mc_fc_info {
	union fc_generic as_generic;
	struct {
		uint32_t cmd;
		uint32_t ext_info_id;
		uint32_t rfu[2];
	} as_in;
	struct {
		uint32_t resp;
		uint32_t ret;
		uint32_t state;
		uint32_t ext_info;
	} as_out;
};


/*                               */
union mc_fc_s_yield {
	union fc_generic as_generic;
	struct {
		uint32_t cmd;
		uint32_t rfu[3];
	} as_in;
	struct {
		uint32_t resp;
		uint32_t ret;
		uint32_t rfu[2];
	} as_out;
};


/*                             */
union mc_fc_nsiq {
	union fc_generic as_generic;
	struct {
		uint32_t cmd;
		uint32_t rfu[3];
	} as_in;
	struct {
		uint32_t resp;
		uint32_t ret;
		uint32_t rfu[2];
	} as_out;
};


/*                                                                            */
/* 
                        
  
                                              
 */
static inline void mc_fastcall(
	union fc_generic *fc_generic
)
{
	MCDRV_ASSERT(fc_generic != NULL);
	/*                                                                   
                */
	MCDRV_ASSERT(raw_smp_processor_id() == 0);
	mb();
#ifdef MC_SMC_FASTCALL
	{
		int ret = 0;
		MCDRV_DBG("Going into SCM()");
		ret = smc_fastcall((void *)fc_generic, sizeof(*fc_generic));
		MCDRV_DBG("Coming from SCM, scm_call=%i, resp=%d/0x%x\n",
			ret,
			fc_generic->as_out.resp, fc_generic->as_out.resp);
	}
#else
	{
		/*                            */
		register u32 reg0 __asm__("r0") = fc_generic->as_in.cmd;
		register u32 reg1 __asm__("r1") = fc_generic->as_in.param[0];
		register u32 reg2 __asm__("r2") = fc_generic->as_in.param[1];
		register u32 reg3 __asm__("r3") = fc_generic->as_in.param[2];

		/*                                                           */
#define __STR2(x)   #x
#define __STR(x)    __STR2(x)

		/*                                               
                             */
#define ASM_ARM_SMC         0xE1600070
		/*                                                          
                               */
#define ASM_ARM_BPKT        0xE1200070
#define ASM_THUMB_BPKT      0xBE00


		__asm__ volatile (
			".word " __STR(ASM_ARM_SMC) "\n"
			: "+r"(reg0), "+r"(reg1), "+r"(reg2), "+r"(reg3)
		);

		/*              */
		fc_generic->as_out.resp     = reg0;
		fc_generic->as_out.ret      = reg1;
		fc_generic->as_out.param[0] = reg2;
		fc_generic->as_out.param[1] = reg3;
	}
#endif
}


/*                                                                            */
/* 
                                                                  
  
 */
static inline int convert_fc_ret(
	uint32_t sret
)
{
	int         ret = -EFAULT;

	switch (sret) {

	case MC_FC_RET_OK:
		ret = 0;
		break;

	case MC_FC_RET_ERR_INVALID:
		ret = -EINVAL;
		break;

	case MC_FC_RET_ERR_ALREADY_INITIALIZED:
		ret = -EBUSY;
		break;

	default:
		break;
	} /*                    */
	return ret;
}

#endif /*                      */
/*     */
