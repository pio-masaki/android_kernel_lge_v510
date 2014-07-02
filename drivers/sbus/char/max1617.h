/*                                                      */
#ifndef _MAX1617_H
#define _MAX1617_H

#define MAX1617_AMB_TEMP	0x00 /*                   */
#define MAX1617_CPU_TEMP	0x01 /*                         */
#define MAX1617_STATUS		0x02 /*                  */

/*                                             */
#define MAX1617_RD_CFG_BYTE	0x03 /*                  */
#define MAX1617_RD_CVRATE_BYTE	0x04 /*                      */
#define MAX1617_RD_AMB_HIGHLIM	0x05 /*                    */
#define MAX1617_RD_AMB_LOWLIM	0x06 /*                   */
#define MAX1617_RD_CPU_HIGHLIM	0x07 /*                      */
#define MAX1617_RD_CPU_LOWLIM	0x08 /*                     */

/*                                  */
#define MAX1617_WR_CFG_BYTE	0x09
#define MAX1617_WR_CVRATE_BYTE	0x0a
#define MAX1617_WR_AMB_HIGHLIM	0x0b
#define MAX1617_WR_AMB_LOWLIM	0x0c
#define MAX1617_WR_CPU_HIGHLIM	0x0d
#define MAX1617_WR_CPU_LOWLIM	0x0e

#define MAX1617_ONESHOT		0x0f

#endif /*            */
