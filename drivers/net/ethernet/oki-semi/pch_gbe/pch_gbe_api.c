/*
 * Copyright (C) 1999 - 2010 Intel Corporation.
 * Copyright (C) 2010 OKI SEMICONDUCTOR Co., LTD.
 *
 * This code was derived from the Intel e1000e Linux driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 */
#include "pch_gbe.h"
#include "pch_gbe_phy.h"

/*                 */
#define pch_gbe_bus_type_unknown	0
#define pch_gbe_bus_type_pci		1
#define pch_gbe_bus_type_pcix		2
#define pch_gbe_bus_type_pci_express	3
#define pch_gbe_bus_type_reserved	4

/*                  */
#define pch_gbe_bus_speed_unknown	0
#define pch_gbe_bus_speed_33		1
#define pch_gbe_bus_speed_66		2
#define pch_gbe_bus_speed_100		3
#define pch_gbe_bus_speed_120		4
#define pch_gbe_bus_speed_133		5
#define pch_gbe_bus_speed_2500		6
#define pch_gbe_bus_speed_reserved	7

/*                  */
#define pch_gbe_bus_width_unknown	0
#define pch_gbe_bus_width_pcie_x1	1
#define pch_gbe_bus_width_pcie_x2	2
#define pch_gbe_bus_width_pcie_x4	4
#define pch_gbe_bus_width_32		5
#define pch_gbe_bus_width_64		6
#define pch_gbe_bus_width_reserved	7

/* 
                                                                 
                                   
 */
static void pch_gbe_plat_get_bus_info(struct pch_gbe_hw *hw)
{
	hw->bus.type  = pch_gbe_bus_type_pci_express;
	hw->bus.speed = pch_gbe_bus_speed_2500;
	hw->bus.width = pch_gbe_bus_width_pcie_x1;
}

/* 
                                             
                                   
          
                   
                               
 */
static s32 pch_gbe_plat_init_hw(struct pch_gbe_hw *hw)
{
	s32 ret_val;

	ret_val = pch_gbe_phy_get_id(hw);
	if (ret_val) {
		pr_err("pch_gbe_phy_get_id error\n");
		return ret_val;
	}
	pch_gbe_phy_init_setting(hw);
	/*                                  */
#ifdef PCH_GBE_MAC_IFOP_RGMII
	pch_gbe_phy_set_rgmii(hw);
#endif
	return ret_val;
}

static const struct pch_gbe_functions pch_gbe_ops = {
	.get_bus_info      = pch_gbe_plat_get_bus_info,
	.init_hw           = pch_gbe_plat_init_hw,
	.read_phy_reg      = pch_gbe_phy_read_reg_miic,
	.write_phy_reg     = pch_gbe_phy_write_reg_miic,
	.reset_phy         = pch_gbe_phy_hw_reset,
	.sw_reset_phy      = pch_gbe_phy_sw_reset,
	.power_up_phy      = pch_gbe_phy_power_up,
	.power_down_phy    = pch_gbe_phy_power_down,
	.read_mac_addr     = pch_gbe_mac_read_mac_addr
};

/* 
                                                       
                                   
 */
static void pch_gbe_plat_init_function_pointers(struct pch_gbe_hw *hw)
{
	/*                   */
	hw->phy.reset_delay_us     = PCH_GBE_PHY_RESET_DELAY_US;
	/*                       */
	hw->func = &pch_gbe_ops;
}

/* 
                                                               
                                   
          
                  
                                     
 */
inline s32 pch_gbe_hal_setup_init_funcs(struct pch_gbe_hw *hw)
{
	if (!hw->reg) {
		pr_err("ERROR: Registers not mapped\n");
		return -ENOSYS;
	}
	pch_gbe_plat_init_function_pointers(hw);
	return 0;
}

/* 
                                                                
                                   
 */
inline void pch_gbe_hal_get_bus_info(struct pch_gbe_hw *hw)
{
	if (!hw->func->get_bus_info)
		pr_err("ERROR: configuration\n");
	else
		hw->func->get_bus_info(hw);
}

/* 
                                            
                                   
          
                  
                                     
 */
inline s32 pch_gbe_hal_init_hw(struct pch_gbe_hw *hw)
{
	if (!hw->func->init_hw) {
		pr_err("ERROR: configuration\n");
		return -ENOSYS;
	}
	return hw->func->init_hw(hw);
}

/* 
                                                
                                       
                                
                                                
          
                  
                         
 */
inline s32 pch_gbe_hal_read_phy_reg(struct pch_gbe_hw *hw, u32 offset,
					u16 *data)
{
	if (!hw->func->read_phy_reg)
		return 0;
	return hw->func->read_phy_reg(hw, offset, data);
}

/* 
                                                  
                                       
                                
                               
          
                  
                         
 */
inline s32 pch_gbe_hal_write_phy_reg(struct pch_gbe_hw *hw, u32 offset,
					u16 data)
{
	if (!hw->func->write_phy_reg)
		return 0;
	return hw->func->write_phy_reg(hw, offset, data);
}

/* 
                                            
                                       
 */
inline void pch_gbe_hal_phy_hw_reset(struct pch_gbe_hw *hw)
{
	if (!hw->func->reset_phy)
		pr_err("ERROR: configuration\n");
	else
		hw->func->reset_phy(hw);
}

/* 
                                            
                                       
 */
inline void pch_gbe_hal_phy_sw_reset(struct pch_gbe_hw *hw)
{
	if (!hw->func->sw_reset_phy)
		pr_err("ERROR: configuration\n");
	else
		hw->func->sw_reset_phy(hw);
}

/* 
                                                
                                   
          
                  
                                     
 */
inline s32 pch_gbe_hal_read_mac_addr(struct pch_gbe_hw *hw)
{
	if (!hw->func->read_mac_addr) {
		pr_err("ERROR: configuration\n");
		return -ENOSYS;
	}
	return hw->func->read_mac_addr(hw);
}

/* 
                                          
                                   
 */
inline void pch_gbe_hal_power_up_phy(struct pch_gbe_hw *hw)
{
	if (hw->func->power_up_phy)
		hw->func->power_up_phy(hw);
}

/* 
                                              
                                   
 */
inline void pch_gbe_hal_power_down_phy(struct pch_gbe_hw *hw)
{
	if (hw->func->power_down_phy)
		hw->func->power_down_phy(hw);
}
