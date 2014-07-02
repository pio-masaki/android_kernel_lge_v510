#ifndef QDSP5AUDPPMSG_H
#define QDSP5AUDPPMSG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       Q D S P 5  A U D I O   P O S T   P R O C E S S I N G   M S G

GENERAL DESCRIPTION
  Messages sent by AUDPPTASK to ARM

REFERENCES
  None

EXTERNALIZED FUNCTIONS
  None

Copyright (c) 1992-2009, The Linux Foundation. All rights reserved.

This software is licensed under the terms of the GNU General Public
License version 2, as published by the Free Software Foundation, and
may be copied, distributed, and modified under those terms.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*
                                                          
                  
                   
                                                                       
 */

/*
                                                                       
 */

#define AUDPP_MSG_FEAT_QUERY_DM_DONE 0x000b

#define AUDPP_MSG_STATUS_MSG		0x0001
#define AUDPP_MSG_STATUS_MSG_LEN	\
	sizeof(struct audpp_msg_status_msg)

#define AUDPP_MSG_STATUS_SLEEP		0x0000
#define AUDPP_MSG_STATUS_INIT		0x0001
#define AUDPP_MSG_STATUS_CFG		0x0002
#define AUDPP_MSG_STATUS_PLAY		0x0003

#define AUDPP_MSG_REASON_NONE	0x0000
#define AUDPP_MSG_REASON_MEM	0x0001
#define AUDPP_MSG_REASON_NODECODER 0x0002

struct audpp_msg_status_msg {
	unsigned short dec_id;
	unsigned short status;
	unsigned short reason;
} __attribute__((packed));

/*
                                                                   
 */
#define AUDPP_MSG_SPA_BANDS		0x0002
#define AUDPP_MSG_SPA_BANDS_LEN	\
	sizeof(struct audpp_msg_spa_bands)

struct audpp_msg_spa_bands {
	unsigned short			current_object;
	unsigned short			spa_band_1;
	unsigned short			spa_band_2;
	unsigned short			spa_band_3;
	unsigned short			spa_band_4;
	unsigned short			spa_band_5;
	unsigned short			spa_band_6;
	unsigned short			spa_band_7;
	unsigned short			spa_band_8;
	unsigned short			spa_band_9;
	unsigned short			spa_band_10;
	unsigned short			spa_band_11;
	unsigned short			spa_band_12;
	unsigned short			spa_band_13;
	unsigned short			spa_band_14;
	unsigned short			spa_band_15;
	unsigned short			spa_band_16;
	unsigned short			spa_band_17;
	unsigned short			spa_band_18;
	unsigned short			spa_band_19;
	unsigned short			spa_band_20;
	unsigned short			spa_band_21;
	unsigned short			spa_band_22;
	unsigned short			spa_band_23;
	unsigned short			spa_band_24;
	unsigned short			spa_band_25;
	unsigned short			spa_band_26;
	unsigned short			spa_band_27;
	unsigned short			spa_band_28;
	unsigned short			spa_band_29;
	unsigned short			spa_band_30;
	unsigned short			spa_band_31;
	unsigned short			spa_band_32;
} __attribute__((packed));

/*
                                                      
 */
#define  AUDPP_MSG_HOST_PCM_INTF_MSG		0x0003
#define  AUDPP_MSG_HOST_PCM_INTF_MSG_LEN	\
	sizeof(struct audpp_msg_host_pcm_intf_msg)

#define AUDPP_MSG_HOSTPCM_ID_TX_ARM	0x0000
#define AUDPP_MSG_HOSTPCM_ID_ARM_TX	0x0001
#define AUDPP_MSG_HOSTPCM_ID_RX_ARM	0x0002
#define AUDPP_MSG_HOSTPCM_ID_ARM_RX	0x0003

#define AUDPP_MSG_SAMP_FREQ_INDX_96000	0x0000
#define AUDPP_MSG_SAMP_FREQ_INDX_88200	0x0001
#define AUDPP_MSG_SAMP_FREQ_INDX_64000	0x0002
#define AUDPP_MSG_SAMP_FREQ_INDX_48000	0x0003
#define AUDPP_MSG_SAMP_FREQ_INDX_44100	0x0004
#define AUDPP_MSG_SAMP_FREQ_INDX_32000	0x0005
#define AUDPP_MSG_SAMP_FREQ_INDX_24000	0x0006
#define AUDPP_MSG_SAMP_FREQ_INDX_22050	0x0007
#define AUDPP_MSG_SAMP_FREQ_INDX_16000	0x0008
#define AUDPP_MSG_SAMP_FREQ_INDX_12000	0x0009
#define AUDPP_MSG_SAMP_FREQ_INDX_11025	0x000A
#define AUDPP_MSG_SAMP_FREQ_INDX_8000	0x000B

#define AUDPP_MSG_CHANNEL_MODE_MONO		0x0001
#define AUDPP_MSG_CHANNEL_MODE_STEREO	0x0002

struct audpp_msg_host_pcm_intf_msg {
	unsigned short obj_num;
	unsigned short numbers_of_samples;
	unsigned short host_pcm_id;
	unsigned short buf_indx;
	unsigned short samp_freq_indx;
	unsigned short channel_mode;
} __attribute__((packed));


/*
                                                                            
                                     
 */

#define AUDPP_MSG_QAFX_POS		0x0004
#define AUDPP_MSG_QAFX_POS_LEN		\
	sizeof(struct audpp_msg_qafx_pos)

struct audpp_msg_qafx_pos {
	unsigned short	current_object;
	unsigned short	x_pos_lis_msw;
	unsigned short	x_pos_lis_lsw;
	unsigned short	y_pos_lis_msw;
	unsigned short	y_pos_lis_lsw;
	unsigned short	z_pos_lis_msw;
	unsigned short	z_pos_lis_lsw;
	unsigned short	x_fwd_msw;
	unsigned short	x_fwd_lsw;
	unsigned short	y_fwd_msw;
	unsigned short	y_fwd_lsw;
	unsigned short	z_fwd_msw;
	unsigned short	z_fwd_lsw;
	unsigned short 	x_up_msw;
	unsigned short	x_up_lsw;
	unsigned short 	y_up_msw;
	unsigned short	y_up_lsw;
	unsigned short 	z_up_msw;
	unsigned short	z_up_lsw;
	unsigned short 	x_vel_lis_msw;
	unsigned short 	x_vel_lis_lsw;
	unsigned short 	y_vel_lis_msw;
	unsigned short 	y_vel_lis_lsw;
	unsigned short 	z_vel_lis_msw;
	unsigned short 	z_vel_lis_lsw;
	unsigned short	threed_enable_flag;
	unsigned short 	volume;
	unsigned short	x_pos_source_msw;
	unsigned short	x_pos_source_lsw;
	unsigned short	y_pos_source_msw;
	unsigned short	y_pos_source_lsw;
	unsigned short	z_pos_source_msw;
	unsigned short	z_pos_source_lsw;
	unsigned short	max_dist_0_msw;
	unsigned short	max_dist_0_lsw;
	unsigned short	min_dist_0_msw;
	unsigned short	min_dist_0_lsw;
	unsigned short	roll_off_factor;
	unsigned short	mute_after_max_flag;
	unsigned short	x_vel_source_msw;
	unsigned short	x_vel_source_lsw;
	unsigned short	y_vel_source_msw;
	unsigned short	y_vel_source_lsw;
	unsigned short	z_vel_source_msw;
	unsigned short	z_vel_source_lsw;
} __attribute__((packed));

/*
                                                 
 */

#define AUDPP_MSG_AVSYNC_MSG		0x0005
#define AUDPP_MSG_AVSYNC_MSG_LEN	\
	sizeof(struct audpp_msg_avsync_msg)

struct audpp_msg_avsync_msg {
	unsigned short	active_flag;
	unsigned short	num_samples_counter0_HSW;
	unsigned short	num_samples_counter0_MSW;
	unsigned short	num_samples_counter0_LSW;
	unsigned short	num_bytes_counter0_HSW;
	unsigned short	num_bytes_counter0_MSW;
	unsigned short	num_bytes_counter0_LSW;
	unsigned short	samp_freq_obj_0;
	unsigned short	samp_freq_obj_1;
	unsigned short	samp_freq_obj_2;
	unsigned short	samp_freq_obj_3;
	unsigned short	samp_freq_obj_4;
	unsigned short	samp_freq_obj_5;
	unsigned short	samp_freq_obj_6;
	unsigned short	samp_freq_obj_7;
	unsigned short	samp_freq_obj_8;
	unsigned short	samp_freq_obj_9;
	unsigned short	samp_freq_obj_10;
	unsigned short	samp_freq_obj_11;
	unsigned short	samp_freq_obj_12;
	unsigned short	samp_freq_obj_13;
	unsigned short	samp_freq_obj_14;
	unsigned short	samp_freq_obj_15;
	unsigned short	num_samples_counter4_HSW;
	unsigned short	num_samples_counter4_MSW;
	unsigned short	num_samples_counter4_LSW;
	unsigned short	num_bytes_counter4_HSW;
	unsigned short	num_bytes_counter4_MSW;
	unsigned short	num_bytes_counter4_LSW;
} __attribute__((packed));

/*
                                                             
 */

#define  AUDPP_MSG_PCMDMAMISSED	0x0006
#define  AUDPP_MSG_PCMDMAMISSED_LEN	\
	sizeof(struct audpp_msg_pcmdmamissed);

struct audpp_msg_pcmdmamissed {
	/*
                                             
                                         
                                             
                                         
                                             
                                         
                                             
                                         
                                             
                                         
 */
	unsigned short pcmdmamissed;
} __attribute__((packed));

/*
                                                          
 */

#define AUDPP_MSG_CFG_MSG	0x0007
#define AUDPP_MSG_CFG_MSG_LEN	\
    sizeof(struct audpp_msg_cfg_msg)

#define AUDPP_MSG_ENA_ENA	0xFFFF
#define AUDPP_MSG_ENA_DIS	0x0000

struct audpp_msg_cfg_msg {
	/*                    
                 
 */
	unsigned short enabled;
} __attribute__((packed));

/*
                                                   
 */

#define AUDPP_MSG_QREVERB_VOLUME	0x0008
#define AUDPP_MSG_QREVERB_VOLUME_LEN	\
	sizeof(struct audpp_msg_qreverb_volume)


struct audpp_msg_qreverb_volume {
	unsigned short	obj_0_gain;
	unsigned short	obj_1_gain;
	unsigned short	obj_2_gain;
	unsigned short	obj_3_gain;
	unsigned short	obj_4_gain;
	unsigned short	hpcm_obj_volume;
} __attribute__((packed));

#define AUDPP_MSG_ROUTING_ACK 0x0009
#define AUDPP_MSG_ROUTING_ACK_LEN \
	sizeof(struct audpp_msg_routing_ack)

struct audpp_msg_routing_ack {
	unsigned short dec_id;
	unsigned short routing_mode;
} __attribute__((packed));

#define AUDPP_MSG_FLUSH_ACK 0x000A

#endif /*                 */
