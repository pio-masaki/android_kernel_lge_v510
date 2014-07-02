#ifndef _ASM_M32R_M32R_MP_FPGA_
#define _ASM_M32R_M32R_MP_FPGA_

/*
 * Renesas M32R-MP-FPGA
 *
 * Copyright (c) 2002  Hitoshi Yamamoto
 * Copyright (c) 2003, 2004  Renesas Technology Corp.
 */

/*
                                                           
                          
                                                           
                                
                                
                                                  
                                              
                                                  
                                             
                                               
                                                  
                                             
                                             
                                                  
                                             
                                                 
                                     
                                                           
                                
                                                   
                                     
                                                           
                                
                                                           
                                
                                                           
                                
                                                           
                                
                                                           
                                
                                                           
                                
                                                           
                   
                                 
                                     
                                 
                                     
                                                           
                           
                                
                                     
                                           
                                     
                                        
             
                                                           
 */

/*                                                                       
                            
                                                                        */
#define M32R_SFR_OFFSET  (0x00E00000)  /*                             */

/*
                  
 */
#define M32R_FPGA_TOP  (0x000F0000+M32R_SFR_OFFSET)

#define M32R_FPGA_NUM_OF_CPUS_PORTL  (0x00+M32R_FPGA_TOP)
#define M32R_FPGA_CPU_NAME0_PORTL    (0x10+M32R_FPGA_TOP)
#define M32R_FPGA_CPU_NAME1_PORTL    (0x14+M32R_FPGA_TOP)
#define M32R_FPGA_CPU_NAME2_PORTL    (0x18+M32R_FPGA_TOP)
#define M32R_FPGA_CPU_NAME3_PORTL    (0x1C+M32R_FPGA_TOP)
#define M32R_FPGA_MODEL_ID0_PORTL    (0x20+M32R_FPGA_TOP)
#define M32R_FPGA_MODEL_ID1_PORTL    (0x24+M32R_FPGA_TOP)
#define M32R_FPGA_MODEL_ID2_PORTL    (0x28+M32R_FPGA_TOP)
#define M32R_FPGA_MODEL_ID3_PORTL    (0x2C+M32R_FPGA_TOP)
#define M32R_FPGA_VERSION0_PORTL     (0x30+M32R_FPGA_TOP)
#define M32R_FPGA_VERSION1_PORTL     (0x34+M32R_FPGA_TOP)

/*
                                     
 */
#define M32R_CPM_OFFSET  (0x000F4000+M32R_SFR_OFFSET)

#define M32R_CPM_CPUCLKCR_PORTL  (0x00+M32R_CPM_OFFSET)
#define M32R_CPM_CLKMOD_PORTL    (0x04+M32R_CPM_OFFSET)
#define M32R_CPM_PLLCR_PORTL     (0x08+M32R_CPM_OFFSET)

/*
                                     
 */
#define M32R_BSELC_OFFSET  (0x000F5000+M32R_SFR_OFFSET)

#define M32R_BSEL0_CR0_PORTL  (0x000+M32R_BSELC_OFFSET)
#define M32R_BSEL0_CR1_PORTL  (0x004+M32R_BSELC_OFFSET)
#define M32R_BSEL1_CR0_PORTL  (0x100+M32R_BSELC_OFFSET)
#define M32R_BSEL1_CR1_PORTL  (0x104+M32R_BSELC_OFFSET)
#define M32R_BSEL2_CR0_PORTL  (0x200+M32R_BSELC_OFFSET)
#define M32R_BSEL2_CR1_PORTL  (0x204+M32R_BSELC_OFFSET)
#define M32R_BSEL3_CR0_PORTL  (0x300+M32R_BSELC_OFFSET)
#define M32R_BSEL3_CR1_PORTL  (0x304+M32R_BSELC_OFFSET)
#define M32R_BSEL4_CR0_PORTL  (0x400+M32R_BSELC_OFFSET)
#define M32R_BSEL4_CR1_PORTL  (0x404+M32R_BSELC_OFFSET)
#define M32R_BSEL5_CR0_PORTL  (0x500+M32R_BSELC_OFFSET)
#define M32R_BSEL5_CR1_PORTL  (0x504+M32R_BSELC_OFFSET)
#define M32R_BSEL6_CR0_PORTL  (0x600+M32R_BSELC_OFFSET)
#define M32R_BSEL6_CR1_PORTL  (0x604+M32R_BSELC_OFFSET)
#define M32R_BSEL7_CR0_PORTL  (0x700+M32R_BSELC_OFFSET)
#define M32R_BSEL7_CR1_PORTL  (0x704+M32R_BSELC_OFFSET)

/*
                                  
 */
#define M32R_MFT_OFFSET        (0x000FC000+M32R_SFR_OFFSET)

#define M32R_MFTCR_PORTL       (0x000+M32R_MFT_OFFSET)  /*             */
#define M32R_MFTRPR_PORTL      (0x004+M32R_MFT_OFFSET)  /*               */

#define M32R_MFT0_OFFSET       (0x100+M32R_MFT_OFFSET)
#define M32R_MFT0MOD_PORTL     (0x00+M32R_MFT0_OFFSET)  /*           */
#define M32R_MFT0BOS_PORTL     (0x04+M32R_MFT0_OFFSET)  /*                           */
#define M32R_MFT0CUT_PORTL     (0x08+M32R_MFT0_OFFSET)  /*            */
#define M32R_MFT0RLD_PORTL     (0x0C+M32R_MFT0_OFFSET)  /*             */
#define M32R_MFT0CMPRLD_PORTL  (0x10+M32R_MFT0_OFFSET)  /*                     */

#define M32R_MFT1_OFFSET       (0x200+M32R_MFT_OFFSET)
#define M32R_MFT1MOD_PORTL     (0x00+M32R_MFT1_OFFSET)  /*           */
#define M32R_MFT1BOS_PORTL     (0x04+M32R_MFT1_OFFSET)  /*                           */
#define M32R_MFT1CUT_PORTL     (0x08+M32R_MFT1_OFFSET)  /*            */
#define M32R_MFT1RLD_PORTL     (0x0C+M32R_MFT1_OFFSET)  /*             */
#define M32R_MFT1CMPRLD_PORTL  (0x10+M32R_MFT1_OFFSET)  /*                     */

#define M32R_MFT2_OFFSET       (0x300+M32R_MFT_OFFSET)
#define M32R_MFT2MOD_PORTL     (0x00+M32R_MFT2_OFFSET)  /*           */
#define M32R_MFT2BOS_PORTL     (0x04+M32R_MFT2_OFFSET)  /*                           */
#define M32R_MFT2CUT_PORTL     (0x08+M32R_MFT2_OFFSET)  /*            */
#define M32R_MFT2RLD_PORTL     (0x0C+M32R_MFT2_OFFSET)  /*             */
#define M32R_MFT2CMPRLD_PORTL  (0x10+M32R_MFT2_OFFSET)  /*                     */

#define M32R_MFT3_OFFSET       (0x400+M32R_MFT_OFFSET)
#define M32R_MFT3MOD_PORTL     (0x00+M32R_MFT3_OFFSET)  /*           */
#define M32R_MFT3BOS_PORTL     (0x04+M32R_MFT3_OFFSET)  /*                           */
#define M32R_MFT3CUT_PORTL     (0x08+M32R_MFT3_OFFSET)  /*            */
#define M32R_MFT3RLD_PORTL     (0x0C+M32R_MFT3_OFFSET)  /*             */
#define M32R_MFT3CMPRLD_PORTL  (0x10+M32R_MFT3_OFFSET)  /*                     */

#define M32R_MFT4_OFFSET       (0x500+M32R_MFT_OFFSET)
#define M32R_MFT4MOD_PORTL     (0x00+M32R_MFT4_OFFSET)  /*           */
#define M32R_MFT4BOS_PORTL     (0x04+M32R_MFT4_OFFSET)  /*                           */
#define M32R_MFT4CUT_PORTL     (0x08+M32R_MFT4_OFFSET)  /*            */
#define M32R_MFT4RLD_PORTL     (0x0C+M32R_MFT4_OFFSET)  /*             */
#define M32R_MFT4CMPRLD_PORTL  (0x10+M32R_MFT4_OFFSET)  /*                     */

#define M32R_MFT5_OFFSET       (0x600+M32R_MFT_OFFSET)
#define M32R_MFT5MOD_PORTL     (0x00+M32R_MFT5_OFFSET)  /*           */
#define M32R_MFT5BOS_PORTL     (0x04+M32R_MFT5_OFFSET)  /*                           */
#define M32R_MFT5CUT_PORTL     (0x08+M32R_MFT5_OFFSET)  /*            */
#define M32R_MFT5RLD_PORTL     (0x0C+M32R_MFT5_OFFSET)  /*             */
#define M32R_MFT5CMPRLD_PORTL  (0x10+M32R_MFT5_OFFSET)  /*                     */

#define M32R_MFTCR_MFT0MSK  (1UL<<15)  /*     */
#define M32R_MFTCR_MFT1MSK  (1UL<<14)  /*     */
#define M32R_MFTCR_MFT2MSK  (1UL<<13)  /*     */
#define M32R_MFTCR_MFT3MSK  (1UL<<12)  /*     */
#define M32R_MFTCR_MFT4MSK  (1UL<<11)  /*     */
#define M32R_MFTCR_MFT5MSK  (1UL<<10)  /*     */
#define M32R_MFTCR_MFT0EN   (1UL<<7)   /*     */
#define M32R_MFTCR_MFT1EN   (1UL<<6)   /*     */
#define M32R_MFTCR_MFT2EN   (1UL<<5)   /*     */
#define M32R_MFTCR_MFT3EN   (1UL<<4)   /*     */
#define M32R_MFTCR_MFT4EN   (1UL<<3)   /*     */
#define M32R_MFTCR_MFT5EN   (1UL<<2)   /*     */

#define M32R_MFTMOD_CC_MASK    (1UL<<15)  /*     */
#define M32R_MFTMOD_TCCR       (1UL<<13)  /*     */
#define M32R_MFTMOD_GTSEL000   (0UL<<8)   /*              */
#define M32R_MFTMOD_GTSEL001   (1UL<<8)   /*              */
#define M32R_MFTMOD_GTSEL010   (2UL<<8)   /*              */
#define M32R_MFTMOD_GTSEL011   (3UL<<8)   /*              */
#define M32R_MFTMOD_GTSEL110   (6UL<<8)   /*              */
#define M32R_MFTMOD_GTSEL111   (7UL<<8)   /*              */
#define M32R_MFTMOD_CMSEL      (1UL<<3)   /*     */
#define M32R_MFTMOD_CSSEL000   (0UL<<0)   /*               */
#define M32R_MFTMOD_CSSEL001   (1UL<<0)   /*               */
#define M32R_MFTMOD_CSSEL010   (2UL<<0)   /*               */
#define M32R_MFTMOD_CSSEL011   (3UL<<0)   /*               */
#define M32R_MFTMOD_CSSEL100   (4UL<<0)   /*               */
#define M32R_MFTMOD_CSSEL110   (6UL<<0)   /*               */

/*
                        
 */
#define M32R_SIO_OFFSET  (0x000FD000+M32R_SFR_OFFSET)

#define M32R_SIO0_CR_PORTL     (0x000+M32R_SIO_OFFSET)
#define M32R_SIO0_MOD0_PORTL   (0x004+M32R_SIO_OFFSET)
#define M32R_SIO0_MOD1_PORTL   (0x008+M32R_SIO_OFFSET)
#define M32R_SIO0_STS_PORTL    (0x00C+M32R_SIO_OFFSET)
#define M32R_SIO0_TRCR_PORTL   (0x010+M32R_SIO_OFFSET)
#define M32R_SIO0_BAUR_PORTL   (0x014+M32R_SIO_OFFSET)
#define M32R_SIO0_RBAUR_PORTL  (0x018+M32R_SIO_OFFSET)
#define M32R_SIO0_TXB_PORTL    (0x01C+M32R_SIO_OFFSET)
#define M32R_SIO0_RXB_PORTL    (0x020+M32R_SIO_OFFSET)

/*
                                    
 */
#define M32R_ICU_OFFSET  (0x000FF000+M32R_SFR_OFFSET)

#define M32R_ICU_ISTS_PORTL     (0x004+M32R_ICU_OFFSET)
#define M32R_ICU_IREQ0_PORTL    (0x008+M32R_ICU_OFFSET)
#define M32R_ICU_IREQ1_PORTL    (0x00C+M32R_ICU_OFFSET)
#define M32R_ICU_SBICR_PORTL    (0x018+M32R_ICU_OFFSET)
#define M32R_ICU_IMASK_PORTL    (0x01C+M32R_ICU_OFFSET)
#define M32R_ICU_CR1_PORTL      (0x200+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR2_PORTL      (0x204+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR3_PORTL      (0x208+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR4_PORTL      (0x20C+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR5_PORTL      (0x210+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR6_PORTL      (0x214+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR7_PORTL      (0x218+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR8_PORTL      (0x218+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR32_PORTL     (0x27C+M32R_ICU_OFFSET)  /*         */
#define M32R_ICU_CR33_PORTL     (0x280+M32R_ICU_OFFSET)  /*         */
#define M32R_ICU_CR40_PORTL     (0x29C+M32R_ICU_OFFSET)  /*       */
#define M32R_ICU_CR41_PORTL     (0x2A0+M32R_ICU_OFFSET)  /*       */
#define M32R_ICU_CR48_PORTL     (0x2BC+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR49_PORTL     (0x2C0+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR50_PORTL     (0x2C4+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR51_PORTL     (0x2C8+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR52_PORTL     (0x2CC+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_CR53_PORTL     (0x2D0+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR0_PORTL   (0x2DC+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR1_PORTL   (0x2E0+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR2_PORTL   (0x2E4+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR3_PORTL   (0x2E8+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR4_PORTL   (0x2EC+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR5_PORTL   (0x2F0+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR6_PORTL   (0x2F4+M32R_ICU_OFFSET)  /*      */
#define M32R_ICU_IPICR7_PORTL   (0x2FC+M32R_ICU_OFFSET)  /*      */

#define M32R_ICUISTS_VECB(val)  ((val>>28) & 0xF)
#define M32R_ICUISTS_ISN(val)   ((val>>22) & 0x3F)
#define M32R_ICUISTS_PIML(val)  ((val>>16) & 0x7)

#define M32R_ICUIMASK_IMSK0  (0UL<<16)  /*                            */
#define M32R_ICUIMASK_IMSK1  (1UL<<16)  /*                                   */
#define M32R_ICUIMASK_IMSK2  (2UL<<16)  /*                                     */
#define M32R_ICUIMASK_IMSK3  (3UL<<16)  /*                                     */
#define M32R_ICUIMASK_IMSK4  (4UL<<16)  /*                                     */
#define M32R_ICUIMASK_IMSK5  (5UL<<16)  /*                                     */
#define M32R_ICUIMASK_IMSK6  (6UL<<16)  /*                                     */
#define M32R_ICUIMASK_IMSK7  (7UL<<16)  /*                                     */

#define M32R_ICUCR_IEN      (1UL<<12)  /*                       */
#define M32R_ICUCR_IRQ      (1UL<<8)   /*                        */
#define M32R_ICUCR_ISMOD00  (0UL<<4)   /*                                         */
#define M32R_ICUCR_ISMOD01  (1UL<<4)   /*                                       */
#define M32R_ICUCR_ISMOD10  (2UL<<4)   /*                                        */
#define M32R_ICUCR_ISMOD11  (3UL<<4)   /*                                       */
#define M32R_ICUCR_ILEVEL0  (0UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL1  (1UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL2  (2UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL3  (3UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL4  (4UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL5  (5UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL6  (6UL<<0)   /*                                     */
#define M32R_ICUCR_ILEVEL7  (7UL<<0)   /*                            */
#define M32R_ICUCR_ILEVEL_MASK  (7UL)

#define M32R_IRQ_INT0    (1)   /*      */
#define M32R_IRQ_INT1    (2)   /*      */
#define M32R_IRQ_INT2    (3)   /*      */
#define M32R_IRQ_INT3    (4)   /*      */
#define M32R_IRQ_INT4    (5)   /*      */
#define M32R_IRQ_INT5    (6)   /*      */
#define M32R_IRQ_INT6    (7)   /*      */
#define M32R_IRQ_INT7    (8)   /*      */
#define M32R_IRQ_MFT0    (16)  /*      */
#define M32R_IRQ_MFT1    (17)  /*      */
#define M32R_IRQ_MFT2    (18)  /*      */
#define M32R_IRQ_MFT3    (19)  /*      */
#define M32R_IRQ_MFT4    (20)  /*      */
#define M32R_IRQ_MFT5    (21)  /*      */
#define M32R_IRQ_DMAC0   (32)  /*       */
#define M32R_IRQ_DMAC1   (33)  /*       */
#define M32R_IRQ_SIO0_R  (48)  /*              */
#define M32R_IRQ_SIO0_S  (49)  /*              */
#define M32R_IRQ_SIO1_R  (50)  /*              */
#define M32R_IRQ_SIO1_S  (51)  /*              */
#define M32R_IRQ_IPI0    (56)  /*      */
#define M32R_IRQ_IPI1    (57)  /*      */
#define M32R_IRQ_IPI2    (58)  /*      */
#define M32R_IRQ_IPI3    (59)  /*      */
#define M32R_IRQ_IPI4    (60)  /*      */
#define M32R_IRQ_IPI5    (61)  /*      */
#define M32R_IRQ_IPI6    (62)  /*      */
#define M32R_IRQ_IPI7    (63)  /*      */

/*                                                                       
      
                                                                        */

#define M32R_CPUID_PORTL   (0xFFFFFFE0)
#define M32R_MCICAR_PORTL  (0xFFFFFFF0)
#define M32R_MCDCAR_PORTL  (0xFFFFFFF4)
#define M32R_MCCR_PORTL    (0xFFFFFFFC)

#endif  /*                         */
