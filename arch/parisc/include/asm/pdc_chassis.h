/*
 *	include/asm-parisc/pdc_chassis.h
 *
 *	Copyright (C) 2002 Laurent Canet <canetl@esiee.fr>
 *	Copyright (C) 2002 Thibaut Varene <varenet@parisc-linux.org>
 *
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License, version 2, as
 *      published by the Free Software Foundation.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *      TODO:	- handle processor number on SMP systems (Reporting Entity ID)
 *      	- handle message ID
 *      	- handle timestamps
 */
 

#ifndef _PARISC_PDC_CHASSIS_H
#define _PARISC_PDC_CHASSIS_H

/*
             
             
             
 */

int pdc_chassis_send_status(int message);
void parisc_pdc_chassis_init(void);


/*
                    
                    
                    
                                                                                 
 */

#define PDC_CHASSIS_DIRECT_BSTART	0
#define PDC_CHASSIS_DIRECT_BCOMPLETE	1
#define PDC_CHASSIS_DIRECT_SHUTDOWN	2
#define PDC_CHASSIS_DIRECT_PANIC	3
#define PDC_CHASSIS_DIRECT_HPMC		4
#define PDC_CHASSIS_DIRECT_LPMC		5
#define PDC_CHASSIS_DIRECT_DUMP		6	/*                     */
#define PDC_CHASSIS_DIRECT_OOPS		7	/*                     */


/*
               
               
               
                                              
 */

/*                     */
#define PDC_CHASSIS_DISP_DATA(v)	((unsigned long)(v) << 17)

/* 
                               
 */

#define PDC_CHASSIS_LED_RUN_OFF		(0ULL << 4)
#define PDC_CHASSIS_LED_RUN_FLASH	(1ULL << 4)
#define PDC_CHASSIS_LED_RUN_ON		(2ULL << 4)
#define PDC_CHASSIS_LED_RUN_NC		(3ULL << 4)
#define PDC_CHASSIS_LED_ATTN_OFF	(0ULL << 6)
#define PDC_CHASSIS_LED_ATTN_FLASH	(1ULL << 6)
#define PDC_CHASSIS_LED_ATTN_NC		(3ULL << 6)	/*                    */
#define PDC_CHASSIS_LED_FAULT_OFF	(0ULL << 8)
#define PDC_CHASSIS_LED_FAULT_FLASH	(1ULL << 8)
#define PDC_CHASSIS_LED_FAULT_ON	(2ULL << 8)
#define PDC_CHASSIS_LED_FAULT_NC	(3ULL << 8)
#define PDC_CHASSIS_LED_VALID		(1ULL << 10)

/* 
                                        
 */

/*                         */
#define PDC_CHASSIS_LSTATE_RUN_NORMAL	(PDC_CHASSIS_LED_RUN_ON		| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                 */
#define PDC_CHASSIS_LSTATE_RUN_CRASHREC	(PDC_CHASSIS_LED_RUN_ON		| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_FLASH	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                   */
#define PDC_CHASSIS_LSTATE_RUN_SYSINT	(PDC_CHASSIS_LED_RUN_ON		| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                     */
#define PDC_CHASSIS_LSTATE_RUN_NCRIT	(PDC_CHASSIS_LED_RUN_ON		| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_FLASH	| \
					 PDC_CHASSIS_LED_VALID		)
/*                       */
#define PDC_CHASSIS_LSTATE_NONOS	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                     */
#define PDC_CHASSIS_LSTATE_NONOS_BFAIL	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_ON	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                    */
#define PDC_CHASSIS_LSTATE_NONOS_UNEXP	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_FLASH	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                     */
#define PDC_CHASSIS_LSTATE_NONOS_NCRIT	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                   */
#define PDC_CHASSIS_LSTATE_BFAIL_NCRIT	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_ON	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                                    */
#define PDC_CHASSIS_LSTATE_UNEXP_NCRIT	(PDC_CHASSIS_LED_RUN_FLASH	| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_FLASH	| \
					 PDC_CHASSIS_LED_VALID		)
/*                    */
#define PDC_CHASSIS_LSTATE_CANNOT_PDC	(PDC_CHASSIS_LED_RUN_OFF	| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                         */
#define PDC_CHASSIS_LSTATE_FATAL_BFAIL	(PDC_CHASSIS_LED_RUN_OFF	| \
					 PDC_CHASSIS_LED_ATTN_OFF	| \
					 PDC_CHASSIS_LED_FAULT_ON	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                        */
#define PDC_CHASSIS_LSTATE_NOCODE_NCRIT	(PDC_CHASSIS_LED_RUN_OFF	| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_OFF	| \
					 PDC_CHASSIS_LED_VALID		)
/*                                                                                */
#define PDC_CHASSIS_LSTATE_FATAL_NCRIT	(PDC_CHASSIS_LED_RUN_OFF	| \
					 PDC_CHASSIS_LED_ATTN_FLASH	| \
					 PDC_CHASSIS_LED_FAULT_ON	| \
					 PDC_CHASSIS_LED_VALID		)
/*                              */


/*
                 
                 
                 
                                                                        
                                                                
                                                                
                                                                  
                                         
 */

/*             */
#define PDC_CHASSIS_ALERT_FORWARD	(0ULL << 36)	/*                     */
#define PDC_CHASSIS_ALERT_SERPROC	(1ULL << 36)	/*                           */
#define PDC_CHASSIS_ALERT_NURGENT	(2ULL << 36)	/*                          */
#define PDC_CHASSIS_ALERT_BLOCKED	(3ULL << 36)	/*                */
#define PDC_CHASSIS_ALERT_CONF_CHG	(4ULL << 36)	/*                                 */
#define PDC_CHASSIS_ALERT_ENV_PB	(5ULL << 36)	/*                                 */
#define PDC_CHASSIS_ALERT_PENDING	(6ULL << 36)	/*                                */
#define PDC_CHASSIS_ALERT_PERF_IMP	(8ULL << 36)	/*                                     */
#define PDC_CHASSIS_ALERT_FUNC_IMP	(10ULL << 36)	/*                                       */
#define PDC_CHASSIS_ALERT_SOFT_FAIL	(12ULL << 36)	/*                  */
#define PDC_CHASSIS_ALERT_HANG		(13ULL << 36)	/*             */
#define PDC_CHASSIS_ALERT_ENV_FATAL	(14ULL << 36)	/*                                 */
#define PDC_CHASSIS_ALERT_HW_FATAL	(15ULL << 36)	/*                        */

/*        */
#define PDC_CHASSIS_SRC_NONE		(0ULL << 28)	/*                           */
#define PDC_CHASSIS_SRC_PROC		(1ULL << 28)	/*           */
/*                 */
#define PDC_CHASSIS_SRC_PROC_CACHE	(2ULL << 28)	/*                */
#define PDC_CHASSIS_SRC_PDH		(3ULL << 28)	/*                              */
#define PDC_CHASSIS_SRC_PWR		(4ULL << 28)	/*       */
#define PDC_CHASSIS_SRC_FAB		(5ULL << 28)	/*                  */
#define PDC_CHASSIS_SRC_PLATi		(6ULL << 28)	/*          */
#define PDC_CHASSIS_SRC_MEM		(7ULL << 28)	/*        */
#define PDC_CHASSIS_SRC_IO		(8ULL << 28)	/*     */
#define PDC_CHASSIS_SRC_CELL		(9ULL << 28)	/*      */
#define PDC_CHASSIS_SRC_PD		(10ULL << 28)	/*                  */

/*                     */
#define PDC_CHASSIS_SRC_D_PROC		(1ULL << 24)	/*                   */

/*                                */
#define PDC_CHASSIS_SRC_ID_UNSPEC	(0ULL << 16)

/*                                           */
#define PDC_CHASSIS_PB_D_PROC_NONE	(0ULL << 32)	/*                   */
#define PDC_CHASSIS_PB_D_PROC_TIMEOUT	(4ULL << 32)	/*         */

/*                 */
#define PDC_CHASSIS_CALL_ACT_HPUX_BL	(7ULL << 12)	/*             */
#define PDC_CHASSIS_CALL_ACT_HPUX_PD	(8ULL << 12)	/*                   */
#define PDC_CHASSIS_CALL_ACT_HPUX_EVENT	(9ULL << 12)	/*                       */
#define PDC_CHASSIS_CALL_ACT_HPUX_IO	(10ULL << 12)	/*                   */
#define PDC_CHASSIS_CALL_ACT_HPUX_PANIC	(11ULL << 12)	/*              */
#define PDC_CHASSIS_CALL_ACT_HPUX_INIT	(12ULL << 12)	/*                       */
#define PDC_CHASSIS_CALL_ACT_HPUX_SHUT	(13ULL << 12)	/*                 */
#define PDC_CHASSIS_CALL_ACT_HPUX_WARN	(14ULL << 12)	/*                */
#define PDC_CHASSIS_CALL_ACT_HPUX_DU	(15ULL << 12)	/*                           */

/*                                            */
#define PDC_CHASSIS_ACT_STATUS_UNSPEC	(0ULL << 0)

/*                                               */
/*                              */
#define PDC_CHASSIS_CALL_SACT_UNSPEC	(0ULL << 4)	/*                          */

/*                       */
#define PDC_CHASSIS_RET_GENERICOS	(12ULL << 52)	/*              */
#define PDC_CHASSIS_RET_IA64_NT		(13ULL << 52)	/*          */
#define PDC_CHASSIS_RET_HPUX		(14ULL << 52)	/*       */
#define PDC_CHASSIS_RET_DIAG		(15ULL << 52)	/*                                 */

/*                     */
#define PDC_CHASSIS_REID_UNSPEC		(0ULL << 44)

/*           */
#define PDC_CHASSIS_DT_NONE		(0ULL << 59)	/*                   */
/*                                    */
#define PDC_CHASSIS_DT_PHYS_ADDR	(1ULL << 59)	/*                  */
#define PDC_CHASSIS_DT_DATA_EXPECT	(2ULL << 59)	/*               */
#define PDC_CHASSIS_DT_ACTUAL		(3ULL << 59)	/*             */
#define PDC_CHASSIS_DT_PHYS_LOC		(4ULL << 59)	/*                   */
#define PDC_CHASSIS_DT_PHYS_LOC_EXT	(5ULL << 59)	/*                             */
#define PDC_CHASSIS_DT_TAG		(6ULL << 59)	/*     */
#define PDC_CHASSIS_DT_SYNDROME		(7ULL << 59)	/*          */
#define PDC_CHASSIS_DT_CODE_ADDR	(8ULL << 59)	/*              */
#define PDC_CHASSIS_DT_ASCII_MSG	(9ULL << 59)	/*               */
#define PDC_CHASSIS_DT_POST		(10ULL << 59)	/*           */
#define PDC_CHASSIS_DT_TIMESTAMP	(11ULL << 59)	/*           */
#define PDC_CHASSIS_DT_DEV_STAT		(12ULL << 59)	/*               */
#define PDC_CHASSIS_DT_DEV_TYPE		(13ULL << 59)	/*             */
#define PDC_CHASSIS_DT_PB_DET		(14ULL << 59)	/*                */
#define PDC_CHASSIS_DT_ACT_LEV		(15ULL << 59)	/*                        */
#define PDC_CHASSIS_DT_SER_NUM		(16ULL << 59)	/*               */
#define PDC_CHASSIS_DT_REV_NUM		(17ULL << 59)	/*                 */
#define PDC_CHASSIS_DT_INTERRUPT	(18ULL << 59)	/*                          */
#define PDC_CHASSIS_DT_TEST_NUM		(19ULL << 59)	/*             */
#define PDC_CHASSIS_DT_STATE_CHG	(20ULL << 59)	/*                               */
#define PDC_CHASSIS_DT_PROC_DEALLOC	(21ULL << 59)	/*                      */
#define PDC_CHASSIS_DT_RESET		(30ULL << 59)	/*                      */
#define PDC_CHASSIS_DT_PA_LEGACY	(31ULL << 59)	/*                            */

/*                                                                  */
#define PDC_CHASSIS_SYSTATE_BSTART	(0ULL << 0)	/*            */
#define PDC_CHASSIS_SYSTATE_BCOMP	(1ULL << 0)	/*               */
#define PDC_CHASSIS_SYSTATE_CHANGE	(2ULL << 0)	/*              */
#define PDC_CHASSIS_SYSTATE_LED		(3ULL << 0)	/*            */
#define PDC_CHASSIS_SYSTATE_PANIC	(9ULL << 0)	/*          */
#define PDC_CHASSIS_SYSTATE_DUMP	(10ULL << 0)	/*             */
#define PDC_CHASSIS_SYSTATE_HPMC	(11ULL << 0)	/*                 */
#define PDC_CHASSIS_SYSTATE_HALT	(15ULL << 0)	/*               */

/*            */
#define PDC_CHASSIS_MSG_ID		(0ULL << 40)	/*                              */

/*                             */
#define PDC_CHASSIS_EOM_CLEAR		(0ULL << 43)
#define PDC_CHASSIS_EOM_SET		(1ULL << 43)

/*
                                  
 */

/*              */
#define PDC_CHASSIS_PMSG_BSTART		(PDC_CHASSIS_ALERT_SERPROC	| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_INIT	| \
					 PDC_CHASSIS_ACT_STATUS_UNSPEC	| \
					 PDC_CHASSIS_CALL_SACT_UNSPEC	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_REID_UNSPEC	| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_BSTART	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

/*               */
#define PDC_CHASSIS_PMSG_BCOMPLETE	(PDC_CHASSIS_ALERT_SERPROC	| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_INIT	| \
					 PDC_CHASSIS_ACT_STATUS_UNSPEC	| \
					 PDC_CHASSIS_CALL_SACT_UNSPEC	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_REID_UNSPEC	| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_BCOMP	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

/*          */
#define PDC_CHASSIS_PMSG_SHUTDOWN	(PDC_CHASSIS_ALERT_SERPROC	| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_SHUT	| \
					 PDC_CHASSIS_ACT_STATUS_UNSPEC	| \
					 PDC_CHASSIS_CALL_SACT_UNSPEC	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_REID_UNSPEC	| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_HALT	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

/*       */
#define PDC_CHASSIS_PMSG_PANIC		(PDC_CHASSIS_ALERT_SOFT_FAIL	| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_PANIC| \
					 PDC_CHASSIS_ACT_STATUS_UNSPEC	| \
					 PDC_CHASSIS_CALL_SACT_UNSPEC	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_REID_UNSPEC	| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_PANIC	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

//                         
/*      */
#define PDC_CHASSIS_PMSG_HPMC		(PDC_CHASSIS_ALERT_CONF_CHG /* */	| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_WARN	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_HPMC	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

/*      */
#define PDC_CHASSIS_PMSG_LPMC		(PDC_CHASSIS_ALERT_BLOCKED /* */| \
					 PDC_CHASSIS_SRC_PROC		| \
					 PDC_CHASSIS_SRC_D_PROC		| \
					 PDC_CHASSIS_SRC_ID_UNSPEC	| \
					 PDC_CHASSIS_PB_D_PROC_NONE	| \
					 PDC_CHASSIS_CALL_ACT_HPUX_WARN	| \
					 PDC_CHASSIS_ACT_STATUS_UNSPEC	| \
					 PDC_CHASSIS_CALL_SACT_UNSPEC	| \
					 PDC_CHASSIS_RET_HPUX		| \
					 PDC_CHASSIS_REID_UNSPEC	| \
					 PDC_CHASSIS_DT_STATE_CHG	| \
					 PDC_CHASSIS_SYSTATE_CHANGE	| \
					 PDC_CHASSIS_MSG_ID		| \
					 PDC_CHASSIS_EOM_SET		)

#endif /*                       */
/*               */
