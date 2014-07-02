#ifndef __bif_core_defs_asm_h
#define __bif_core_defs_asm_h

/*
                                  
                                                       
                                                                       
                                             
   
                                                                                                                       
                                                                            
                                 
  
                              
 */

#ifndef REG_FIELD
#define REG_FIELD( scope, reg, field, value ) \
  REG_FIELD_X_( value, reg_##scope##_##reg##___##field##___lsb )
#define REG_FIELD_X_( value, shift ) ((value) << shift)
#endif

#ifndef REG_STATE
#define REG_STATE( scope, reg, field, symbolic_value ) \
  REG_STATE_X_( regk_##scope##_##symbolic_value, reg_##scope##_##reg##___##field##___lsb )
#define REG_STATE_X_( k, shift ) (k << shift)
#endif

#ifndef REG_MASK
#define REG_MASK( scope, reg, field ) \
  REG_MASK_X_( reg_##scope##_##reg##___##field##___width, reg_##scope##_##reg##___##field##___lsb )
#define REG_MASK_X_( width, lsb ) (((1 << width)-1) << lsb)
#endif

#ifndef REG_LSB
#define REG_LSB( scope, reg, field ) reg_##scope##_##reg##___##field##___lsb
#endif

#ifndef REG_BIT
#define REG_BIT( scope, reg, field ) reg_##scope##_##reg##___##field##___bit
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) REG_ADDR_X_(inst, reg_##scope##_##reg##_offset)
#define REG_ADDR_X_( inst, offs ) ((inst) + offs)
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
         REG_ADDR_VECT_X_(inst, reg_##scope##_##reg##_offset, index, \
			 STRIDE_##scope##_##reg )
#define REG_ADDR_VECT_X_( inst, offs, index, stride ) \
                          ((inst) + offs + (index) * stride)
#endif

/*                                               */
#define reg_bif_core_rw_grp1_cfg___lw___lsb 0
#define reg_bif_core_rw_grp1_cfg___lw___width 6
#define reg_bif_core_rw_grp1_cfg___ew___lsb 6
#define reg_bif_core_rw_grp1_cfg___ew___width 3
#define reg_bif_core_rw_grp1_cfg___zw___lsb 9
#define reg_bif_core_rw_grp1_cfg___zw___width 3
#define reg_bif_core_rw_grp1_cfg___aw___lsb 12
#define reg_bif_core_rw_grp1_cfg___aw___width 2
#define reg_bif_core_rw_grp1_cfg___dw___lsb 14
#define reg_bif_core_rw_grp1_cfg___dw___width 2
#define reg_bif_core_rw_grp1_cfg___ewb___lsb 16
#define reg_bif_core_rw_grp1_cfg___ewb___width 2
#define reg_bif_core_rw_grp1_cfg___bw___lsb 18
#define reg_bif_core_rw_grp1_cfg___bw___width 1
#define reg_bif_core_rw_grp1_cfg___bw___bit 18
#define reg_bif_core_rw_grp1_cfg___wr_extend___lsb 19
#define reg_bif_core_rw_grp1_cfg___wr_extend___width 1
#define reg_bif_core_rw_grp1_cfg___wr_extend___bit 19
#define reg_bif_core_rw_grp1_cfg___erc_en___lsb 20
#define reg_bif_core_rw_grp1_cfg___erc_en___width 1
#define reg_bif_core_rw_grp1_cfg___erc_en___bit 20
#define reg_bif_core_rw_grp1_cfg___mode___lsb 21
#define reg_bif_core_rw_grp1_cfg___mode___width 1
#define reg_bif_core_rw_grp1_cfg___mode___bit 21
#define reg_bif_core_rw_grp1_cfg_offset 0

/*                                               */
#define reg_bif_core_rw_grp2_cfg___lw___lsb 0
#define reg_bif_core_rw_grp2_cfg___lw___width 6
#define reg_bif_core_rw_grp2_cfg___ew___lsb 6
#define reg_bif_core_rw_grp2_cfg___ew___width 3
#define reg_bif_core_rw_grp2_cfg___zw___lsb 9
#define reg_bif_core_rw_grp2_cfg___zw___width 3
#define reg_bif_core_rw_grp2_cfg___aw___lsb 12
#define reg_bif_core_rw_grp2_cfg___aw___width 2
#define reg_bif_core_rw_grp2_cfg___dw___lsb 14
#define reg_bif_core_rw_grp2_cfg___dw___width 2
#define reg_bif_core_rw_grp2_cfg___ewb___lsb 16
#define reg_bif_core_rw_grp2_cfg___ewb___width 2
#define reg_bif_core_rw_grp2_cfg___bw___lsb 18
#define reg_bif_core_rw_grp2_cfg___bw___width 1
#define reg_bif_core_rw_grp2_cfg___bw___bit 18
#define reg_bif_core_rw_grp2_cfg___wr_extend___lsb 19
#define reg_bif_core_rw_grp2_cfg___wr_extend___width 1
#define reg_bif_core_rw_grp2_cfg___wr_extend___bit 19
#define reg_bif_core_rw_grp2_cfg___erc_en___lsb 20
#define reg_bif_core_rw_grp2_cfg___erc_en___width 1
#define reg_bif_core_rw_grp2_cfg___erc_en___bit 20
#define reg_bif_core_rw_grp2_cfg___mode___lsb 21
#define reg_bif_core_rw_grp2_cfg___mode___width 1
#define reg_bif_core_rw_grp2_cfg___mode___bit 21
#define reg_bif_core_rw_grp2_cfg_offset 4

/*                                               */
#define reg_bif_core_rw_grp3_cfg___lw___lsb 0
#define reg_bif_core_rw_grp3_cfg___lw___width 6
#define reg_bif_core_rw_grp3_cfg___ew___lsb 6
#define reg_bif_core_rw_grp3_cfg___ew___width 3
#define reg_bif_core_rw_grp3_cfg___zw___lsb 9
#define reg_bif_core_rw_grp3_cfg___zw___width 3
#define reg_bif_core_rw_grp3_cfg___aw___lsb 12
#define reg_bif_core_rw_grp3_cfg___aw___width 2
#define reg_bif_core_rw_grp3_cfg___dw___lsb 14
#define reg_bif_core_rw_grp3_cfg___dw___width 2
#define reg_bif_core_rw_grp3_cfg___ewb___lsb 16
#define reg_bif_core_rw_grp3_cfg___ewb___width 2
#define reg_bif_core_rw_grp3_cfg___bw___lsb 18
#define reg_bif_core_rw_grp3_cfg___bw___width 1
#define reg_bif_core_rw_grp3_cfg___bw___bit 18
#define reg_bif_core_rw_grp3_cfg___wr_extend___lsb 19
#define reg_bif_core_rw_grp3_cfg___wr_extend___width 1
#define reg_bif_core_rw_grp3_cfg___wr_extend___bit 19
#define reg_bif_core_rw_grp3_cfg___erc_en___lsb 20
#define reg_bif_core_rw_grp3_cfg___erc_en___width 1
#define reg_bif_core_rw_grp3_cfg___erc_en___bit 20
#define reg_bif_core_rw_grp3_cfg___mode___lsb 21
#define reg_bif_core_rw_grp3_cfg___mode___width 1
#define reg_bif_core_rw_grp3_cfg___mode___bit 21
#define reg_bif_core_rw_grp3_cfg___gated_csp0___lsb 24
#define reg_bif_core_rw_grp3_cfg___gated_csp0___width 2
#define reg_bif_core_rw_grp3_cfg___gated_csp1___lsb 26
#define reg_bif_core_rw_grp3_cfg___gated_csp1___width 2
#define reg_bif_core_rw_grp3_cfg___gated_csp2___lsb 28
#define reg_bif_core_rw_grp3_cfg___gated_csp2___width 2
#define reg_bif_core_rw_grp3_cfg___gated_csp3___lsb 30
#define reg_bif_core_rw_grp3_cfg___gated_csp3___width 2
#define reg_bif_core_rw_grp3_cfg_offset 8

/*                                               */
#define reg_bif_core_rw_grp4_cfg___lw___lsb 0
#define reg_bif_core_rw_grp4_cfg___lw___width 6
#define reg_bif_core_rw_grp4_cfg___ew___lsb 6
#define reg_bif_core_rw_grp4_cfg___ew___width 3
#define reg_bif_core_rw_grp4_cfg___zw___lsb 9
#define reg_bif_core_rw_grp4_cfg___zw___width 3
#define reg_bif_core_rw_grp4_cfg___aw___lsb 12
#define reg_bif_core_rw_grp4_cfg___aw___width 2
#define reg_bif_core_rw_grp4_cfg___dw___lsb 14
#define reg_bif_core_rw_grp4_cfg___dw___width 2
#define reg_bif_core_rw_grp4_cfg___ewb___lsb 16
#define reg_bif_core_rw_grp4_cfg___ewb___width 2
#define reg_bif_core_rw_grp4_cfg___bw___lsb 18
#define reg_bif_core_rw_grp4_cfg___bw___width 1
#define reg_bif_core_rw_grp4_cfg___bw___bit 18
#define reg_bif_core_rw_grp4_cfg___wr_extend___lsb 19
#define reg_bif_core_rw_grp4_cfg___wr_extend___width 1
#define reg_bif_core_rw_grp4_cfg___wr_extend___bit 19
#define reg_bif_core_rw_grp4_cfg___erc_en___lsb 20
#define reg_bif_core_rw_grp4_cfg___erc_en___width 1
#define reg_bif_core_rw_grp4_cfg___erc_en___bit 20
#define reg_bif_core_rw_grp4_cfg___mode___lsb 21
#define reg_bif_core_rw_grp4_cfg___mode___width 1
#define reg_bif_core_rw_grp4_cfg___mode___bit 21
#define reg_bif_core_rw_grp4_cfg___gated_csp4___lsb 26
#define reg_bif_core_rw_grp4_cfg___gated_csp4___width 2
#define reg_bif_core_rw_grp4_cfg___gated_csp5___lsb 28
#define reg_bif_core_rw_grp4_cfg___gated_csp5___width 2
#define reg_bif_core_rw_grp4_cfg___gated_csp6___lsb 30
#define reg_bif_core_rw_grp4_cfg___gated_csp6___width 2
#define reg_bif_core_rw_grp4_cfg_offset 12

/*                                                     */
#define reg_bif_core_rw_sdram_cfg_grp0___bank_sel___lsb 0
#define reg_bif_core_rw_sdram_cfg_grp0___bank_sel___width 5
#define reg_bif_core_rw_sdram_cfg_grp0___ca___lsb 5
#define reg_bif_core_rw_sdram_cfg_grp0___ca___width 3
#define reg_bif_core_rw_sdram_cfg_grp0___type___lsb 8
#define reg_bif_core_rw_sdram_cfg_grp0___type___width 1
#define reg_bif_core_rw_sdram_cfg_grp0___type___bit 8
#define reg_bif_core_rw_sdram_cfg_grp0___bw___lsb 9
#define reg_bif_core_rw_sdram_cfg_grp0___bw___width 1
#define reg_bif_core_rw_sdram_cfg_grp0___bw___bit 9
#define reg_bif_core_rw_sdram_cfg_grp0___sh___lsb 10
#define reg_bif_core_rw_sdram_cfg_grp0___sh___width 3
#define reg_bif_core_rw_sdram_cfg_grp0___wmm___lsb 13
#define reg_bif_core_rw_sdram_cfg_grp0___wmm___width 1
#define reg_bif_core_rw_sdram_cfg_grp0___wmm___bit 13
#define reg_bif_core_rw_sdram_cfg_grp0___sh16___lsb 14
#define reg_bif_core_rw_sdram_cfg_grp0___sh16___width 1
#define reg_bif_core_rw_sdram_cfg_grp0___sh16___bit 14
#define reg_bif_core_rw_sdram_cfg_grp0___grp_sel___lsb 15
#define reg_bif_core_rw_sdram_cfg_grp0___grp_sel___width 5
#define reg_bif_core_rw_sdram_cfg_grp0_offset 16

/*                                                     */
#define reg_bif_core_rw_sdram_cfg_grp1___bank_sel___lsb 0
#define reg_bif_core_rw_sdram_cfg_grp1___bank_sel___width 5
#define reg_bif_core_rw_sdram_cfg_grp1___ca___lsb 5
#define reg_bif_core_rw_sdram_cfg_grp1___ca___width 3
#define reg_bif_core_rw_sdram_cfg_grp1___type___lsb 8
#define reg_bif_core_rw_sdram_cfg_grp1___type___width 1
#define reg_bif_core_rw_sdram_cfg_grp1___type___bit 8
#define reg_bif_core_rw_sdram_cfg_grp1___bw___lsb 9
#define reg_bif_core_rw_sdram_cfg_grp1___bw___width 1
#define reg_bif_core_rw_sdram_cfg_grp1___bw___bit 9
#define reg_bif_core_rw_sdram_cfg_grp1___sh___lsb 10
#define reg_bif_core_rw_sdram_cfg_grp1___sh___width 3
#define reg_bif_core_rw_sdram_cfg_grp1___wmm___lsb 13
#define reg_bif_core_rw_sdram_cfg_grp1___wmm___width 1
#define reg_bif_core_rw_sdram_cfg_grp1___wmm___bit 13
#define reg_bif_core_rw_sdram_cfg_grp1___sh16___lsb 14
#define reg_bif_core_rw_sdram_cfg_grp1___sh16___width 1
#define reg_bif_core_rw_sdram_cfg_grp1___sh16___bit 14
#define reg_bif_core_rw_sdram_cfg_grp1_offset 20

/*                                                   */
#define reg_bif_core_rw_sdram_timing___cl___lsb 0
#define reg_bif_core_rw_sdram_timing___cl___width 3
#define reg_bif_core_rw_sdram_timing___rcd___lsb 3
#define reg_bif_core_rw_sdram_timing___rcd___width 3
#define reg_bif_core_rw_sdram_timing___rp___lsb 6
#define reg_bif_core_rw_sdram_timing___rp___width 3
#define reg_bif_core_rw_sdram_timing___rc___lsb 9
#define reg_bif_core_rw_sdram_timing___rc___width 2
#define reg_bif_core_rw_sdram_timing___dpl___lsb 11
#define reg_bif_core_rw_sdram_timing___dpl___width 2
#define reg_bif_core_rw_sdram_timing___pde___lsb 13
#define reg_bif_core_rw_sdram_timing___pde___width 1
#define reg_bif_core_rw_sdram_timing___pde___bit 13
#define reg_bif_core_rw_sdram_timing___ref___lsb 14
#define reg_bif_core_rw_sdram_timing___ref___width 2
#define reg_bif_core_rw_sdram_timing___cpd___lsb 16
#define reg_bif_core_rw_sdram_timing___cpd___width 1
#define reg_bif_core_rw_sdram_timing___cpd___bit 16
#define reg_bif_core_rw_sdram_timing___sdcke___lsb 17
#define reg_bif_core_rw_sdram_timing___sdcke___width 1
#define reg_bif_core_rw_sdram_timing___sdcke___bit 17
#define reg_bif_core_rw_sdram_timing___sdclk___lsb 18
#define reg_bif_core_rw_sdram_timing___sdclk___width 1
#define reg_bif_core_rw_sdram_timing___sdclk___bit 18
#define reg_bif_core_rw_sdram_timing_offset 24

/*                                                */
#define reg_bif_core_rw_sdram_cmd___cmd___lsb 0
#define reg_bif_core_rw_sdram_cmd___cmd___width 3
#define reg_bif_core_rw_sdram_cmd___mrs_data___lsb 3
#define reg_bif_core_rw_sdram_cmd___mrs_data___width 15
#define reg_bif_core_rw_sdram_cmd_offset 28

/*                                                     */
#define reg_bif_core_rs_sdram_ref_stat___ok___lsb 0
#define reg_bif_core_rs_sdram_ref_stat___ok___width 1
#define reg_bif_core_rs_sdram_ref_stat___ok___bit 0
#define reg_bif_core_rs_sdram_ref_stat_offset 32

/*                                                   */
#define reg_bif_core_r_sdram_ref_stat___ok___lsb 0
#define reg_bif_core_r_sdram_ref_stat___ok___width 1
#define reg_bif_core_r_sdram_ref_stat___ok___bit 0
#define reg_bif_core_r_sdram_ref_stat_offset 36


/*           */
#define regk_bif_core_bank2                       0x00000000
#define regk_bif_core_bank4                       0x00000001
#define regk_bif_core_bit10                       0x0000000a
#define regk_bif_core_bit11                       0x0000000b
#define regk_bif_core_bit12                       0x0000000c
#define regk_bif_core_bit13                       0x0000000d
#define regk_bif_core_bit14                       0x0000000e
#define regk_bif_core_bit15                       0x0000000f
#define regk_bif_core_bit16                       0x00000010
#define regk_bif_core_bit17                       0x00000011
#define regk_bif_core_bit18                       0x00000012
#define regk_bif_core_bit19                       0x00000013
#define regk_bif_core_bit20                       0x00000014
#define regk_bif_core_bit21                       0x00000015
#define regk_bif_core_bit22                       0x00000016
#define regk_bif_core_bit23                       0x00000017
#define regk_bif_core_bit24                       0x00000018
#define regk_bif_core_bit25                       0x00000019
#define regk_bif_core_bit26                       0x0000001a
#define regk_bif_core_bit27                       0x0000001b
#define regk_bif_core_bit28                       0x0000001c
#define regk_bif_core_bit29                       0x0000001d
#define regk_bif_core_bit9                        0x00000009
#define regk_bif_core_bw16                        0x00000001
#define regk_bif_core_bw32                        0x00000000
#define regk_bif_core_bwe                         0x00000000
#define regk_bif_core_cwe                         0x00000001
#define regk_bif_core_e15us                       0x00000001
#define regk_bif_core_e7800ns                     0x00000002
#define regk_bif_core_grp0                        0x00000000
#define regk_bif_core_grp1                        0x00000001
#define regk_bif_core_mrs                         0x00000003
#define regk_bif_core_no                          0x00000000
#define regk_bif_core_none                        0x00000000
#define regk_bif_core_nop                         0x00000000
#define regk_bif_core_off                         0x00000000
#define regk_bif_core_pre                         0x00000002
#define regk_bif_core_r_sdram_ref_stat_default    0x00000001
#define regk_bif_core_rd                          0x00000002
#define regk_bif_core_ref                         0x00000001
#define regk_bif_core_rs_sdram_ref_stat_default   0x00000001
#define regk_bif_core_rw_grp1_cfg_default         0x000006cf
#define regk_bif_core_rw_grp2_cfg_default         0x000006cf
#define regk_bif_core_rw_grp3_cfg_default         0x000006cf
#define regk_bif_core_rw_grp4_cfg_default         0x000006cf
#define regk_bif_core_rw_sdram_cfg_grp1_default   0x00000000
#define regk_bif_core_slf                         0x00000004
#define regk_bif_core_wr                          0x00000001
#define regk_bif_core_yes                         0x00000001
#endif /*                       */
