/*
                                   

                                        
                       
                     
                     
                     

 */

#define CHIP_AU8830

#define CARD_NAME "Aureal Vortex 2"
#define CARD_NAME_SHORT "au8830"

#define NR_ADB 0x20
#define NR_SRC 0x10
#define NR_A3D 0x10
#define NR_MIXIN 0x20
#define NR_MIXOUT 0x10
#define NR_WT 0x40

/*        */
#define VORTEX_ADBDMA_STAT 0x27e00	/*                               */
#define		POS_MASK 0x00000fff
#define     POS_SHIFT 0x0
#define 	ADB_SUBBUF_MASK 0x00003000	/*           */
#define     ADB_SUBBUF_SHIFT 0xc	/*           */
#define VORTEX_ADBDMA_CTRL 0x27a00	/*                                    */
#define		OFFSET_MASK 0x00000fff
#define     OFFSET_SHIFT 0x0
#define		IE_MASK 0x00001000	/*                   */
#define     IE_SHIFT 0xc
#define     DIR_MASK 0x00002000	/*            */
#define     DIR_SHIFT 0xd
#define		FMT_MASK 0x0003c000
#define		FMT_SHIFT 0xe
#define		ADB_FIFO_EN_SHIFT	0x15
#define		ADB_FIFO_EN			(1 << 0x15)
//                                                                                     
#define VORTEX_ADBDMA_BUFCFG0 0x27800
#define VORTEX_ADBDMA_BUFCFG1 0x27804
#define VORTEX_ADBDMA_BUFBASE 0x27400
#define VORTEX_ADBDMA_START 0x27c00	/*                        */

#define VORTEX_ADBDMA_STATUS 0x27A90	/*                                              */
/*                                                                         */
/*                                                                             */

/*     */
#define VORTEX_ENGINE_CTRL 0x27ae8
#define 	ENGINE_INIT 0x1380000

/*       */
#define VORTEX_WTDMA_CTRL 0x27900	/*                 */
#define VORTEX_WTDMA_STAT 0x27d00	/*                     */
#define     WT_SUBBUF_MASK 0x3
#define     WT_SUBBUF_SHIFT 0xc
#define VORTEX_WTDMA_BUFBASE 0x27000
#define VORTEX_WTDMA_BUFCFG0 0x27600
#define VORTEX_WTDMA_BUFCFG1 0x27604
#define VORTEX_WTDMA_START 0x27b00	/*                          */

/*     */
#define VORTEX_ADB_SR 0x28400	/*                            */
#define VORTEX_ADB_RTBASE 0x28000
#define VORTEX_ADB_RTBASE_COUNT 173
#define VORTEX_ADB_CHNBASE 0x282b4
#define VORTEX_ADB_CHNBASE_COUNT 24
#define 	ROUTE_MASK	0xffff
#define		SOURCE_MASK	0xff00
#define     ADB_MASK   0xff
#define		ADB_SHIFT 0x8
/*             */
#define		OFFSET_ADBDMA	0x00
#define		OFFSET_ADBDMAB	0x20
#define		OFFSET_SRCIN	0x40
#define		OFFSET_SRCOUT	0x20	/*         */
#define		OFFSET_MIXIN	0x50	/*         */
#define		OFFSET_MIXOUT	0x30	/*         */
#define		OFFSET_CODECIN	0x70 /*         */	/*            */
#define		OFFSET_CODECOUT	0x88 /*         */	/*            */
#define		OFFSET_SPORTIN	0x78	/*                               */
#define		OFFSET_SPORTOUT	0x90	/*                             */
#define		OFFSET_SPDIFIN	0x7A	/*                     */
#define		OFFSET_SPDIFOUT	0x92	/*                   */
#define		OFFSET_AC98IN	0x7c	/*                     */
#define		OFFSET_AC98OUT	0x94	/*                   */
#define		OFFSET_EQIN		0xa0	/*         */
#define		OFFSET_EQOUT	0x7e /*         */	/*                     */
#define		OFFSET_A3DIN	0x70	/*           */
#define		OFFSET_A3DOUT	0xA6	/*                                    */
#define		OFFSET_WT0		0x40	/*                               */
#define		OFFSET_WT1		0x80	/*                               */
/*                                              */
/*                                             */
#define		OFFSET_XTALKOUT	0x66	/*                                       */
#define		OFFSET_XTALKIN	0x96	/*                                       */
#define		OFFSET_EFXOUT	0x68	/*                       */
#define		OFFSET_EFXIN	0x80	/*                     */

/*                            */
#define ADB_DMA(x) (x)
#define ADB_SRCOUT(x) (x + OFFSET_SRCOUT)
#define ADB_SRCIN(x) (x + OFFSET_SRCIN)
#define ADB_MIXOUT(x) (x + OFFSET_MIXOUT)
#define ADB_MIXIN(x) (x + OFFSET_MIXIN)
#define ADB_CODECIN(x) (x + OFFSET_CODECIN)
#define ADB_CODECOUT(x) (x + OFFSET_CODECOUT)
#define ADB_SPORTIN(x) (x + OFFSET_SPORTIN)
#define ADB_SPORTOUT(x) (x + OFFSET_SPORTOUT)
#define ADB_SPDIFIN(x)	(x + OFFSET_SPDIFIN)
#define ADB_SPDIFOUT(x)	(x + OFFSET_SPDIFOUT)
#define ADB_EQIN(x) (x + OFFSET_EQIN)
#define ADB_EQOUT(x) (x + OFFSET_EQOUT)
#define ADB_A3DOUT(x) (x + OFFSET_A3DOUT)	/*                 */
#define ADB_A3DIN(x) (x + OFFSET_A3DIN)
//                                                                
#define ADB_WTOUT(x,y) (((x)==0)?((y) + OFFSET_WT0):((y) + OFFSET_WT1))
#define ADB_XTALKIN(x) ((x) + OFFSET_XTALKIN)
#define ADB_XTALKOUT(x) ((x) + OFFSET_XTALKOUT)

#define MIX_DEFIGAIN 0x08
#define MIX_DEFOGAIN 0x08	/*                                               */

/*       */
#define VORTEX_MIXER_SR 0x21f00
#define VORTEX_MIXER_CLIP 0x21f80
#define VORTEX_MIXER_CHNBASE 0x21e40
#define VORTEX_MIXER_RTBASE 0x21e00
#define 	MIXER_RTBASE_SIZE 0x38
#define VORTEX_MIX_ENIN 0x21a00	/*                                 */
#define VORTEX_MIX_SMP 0x21c00	/*                                   */

/*     */
#define VORTEX_MIX_INVOL_B 0x20000	/*                      */
#define VORTEX_MIX_VOL_B 0x20800	/*                       */
#define VORTEX_MIX_INVOL_A 0x21000	/*                     */
#define VORTEX_MIX_VOL_A 0x21800	/*                      */

#define 	VOL_MIN 0x80	/*                          */
#define		VOL_MAX 0x7f	/*                                     */

/*     */
#define VORTEX_SRC_CHNBASE		0x26c40
#define VORTEX_SRC_RTBASE		0x26c00
#define VORTEX_SRCBLOCK_SR		0x26cc0
#define VORTEX_SRC_SOURCE		0x26cc4
#define VORTEX_SRC_SOURCESIZE	0x26cc8
/*       
               
               
               
                   
               
                   
                                        
*/

#define VORTEX_SRC_CONVRATIO	0x26e40
#define VORTEX_SRC_DRIFT0		0x26e80
#define VORTEX_SRC_DRIFT1		0x26ec0
#define VORTEX_SRC_DRIFT2		0x26f40
#define VORTEX_SRC_U0			0x26e00
#define		U0_SLOWLOCK		0x200
#define VORTEX_SRC_U1			0x26f00
#define VORTEX_SRC_U2			0x26f80
#define VORTEX_SRC_DATA			0x26800	/*        */
#define VORTEX_SRC_DATA0		0x26000

/*      */
#define VORTEX_FIFO_ADBCTRL 0x16100	/*               */
#define VORTEX_FIFO_WTCTRL 0x16000
#define		FIFO_RDONLY	0x00000001
#define		FIFO_CTRL	0x00000002	/*               */
#define		FIFO_VALID	0x00000010
#define 	FIFO_EMPTY	0x00000020
#define		FIFO_U0		0x00002000	/*          */
#define		FIFO_U1		0x00040000
#define		FIFO_SIZE_BITS 6
#define		FIFO_SIZE	(1<<(FIFO_SIZE_BITS))	//     
#define 	FIFO_MASK	(FIFO_SIZE-1)	//                               
#define 	FIFO_BITS	0x1c400000
#define VORTEX_FIFO_ADBDATA 0x14000
#define VORTEX_FIFO_WTDATA 0x10000

#define VORTEX_FIFO_GIRT	0x17000	/*               */
#define		GIRT_COUNT	3

/*       */

#define VORTEX_CODEC_CHN 0x29080	/*                          */

#define VORTEX_CODEC_CTRL 0x29184
#define VORTEX_CODEC_IO 0x29188

#define VORTEX_CODEC_SPORTCTRL 0x2918c

#define VORTEX_CODEC_EN 0x29190
#define		EN_AUDIO0		0x00000300
#define		EN_MODEM		0x00000c00
#define		EN_AUDIO1		0x00003000
#define		EN_SPORT		0x00030000
#define		EN_SPDIF		0x000c0000
#define		EN_CODEC		(EN_AUDIO1 | EN_AUDIO0)

#define VORTEX_SPDIF_SMPRATE	0x29194

#define VORTEX_SPDIF_FLAGS		0x2205c
#define VORTEX_SPDIF_CFG0		0x291D0	/*             */
#define VORTEX_SPDIF_CFG1		0x291D4

#define VORTEX_SMP_TIME			0x29198	/*                      */
#define VORTEX_SMP_TIMER		0x2919c
#define VORTEX_CODEC2_CTRL		0x291a0

#define VORTEX_MODEM_CTRL		0x291ac

/*     */
#define VORTEX_IRQ_SOURCE 0x2a000	/*                         */
#define VORTEX_IRQ_CTRL 0x2a004	/*                        */

//                                      
#define VORTEX_STAT		0x2a008	/*                     */
#define 	STAT_IRQ	0x00000001	/*                                     */

#define VORTEX_CTRL		0x2a00c
#define 	CTRL_MIDI_EN	0x00000001
#define 	CTRL_MIDI_PORT	0x00000060
#define 	CTRL_GAME_EN	0x00000008
#define 	CTRL_GAME_PORT	0x00000e00
#define 	CTRL_IRQ_ENABLE	0x00004000
#define		CTRL_SPDIF		0x00000000	/*                                 */
#define 	CTRL_SPORT		0x00200000
#define 	CTRL_RST		0x00800000
#define 	CTRL_UNKNOWN	0x01000000

/*                                                   */
#define VORTEX_IRQ_STAT 0x2919c

		     /*      *//*       */
#define VORTEX_MIDI_DATA 0x28800
#define VORTEX_MIDI_CMD 0x28804	/*                             */

#define VORTEX_GAME_LEGACY 0x28808
#define VORTEX_CTRL2 0x2880c
#define		CTRL2_GAME_ADCMODE 0x40
#define VORTEX_GAME_AXIS 0x28810	/*                              */
#define		AXIS_SIZE 4
#define		AXIS_RANGE 0x1fff
