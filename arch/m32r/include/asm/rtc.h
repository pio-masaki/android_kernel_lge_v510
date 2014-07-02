#ifndef __RTC_H__
#define __RTC_H__

   /*                                                */
#  define RTC_SECONDS      0
#  define RTC_MINUTES      1
#  define RTC_HOURS        2
#  define RTC_DAY_OF_MONTH 3
#  define RTC_MONTH        4
#  define RTC_WEEKDAY      5
#  define RTC_YEAR         6
#  define RTC_CONTROL      7

   /*                           */
#  define RTC_CONTROL_WRITEPROTECT 	0x80
#  define RTC_TRICKLECHARGER 		8

  /*                                                              */
#  define RTC_TCR_PATTERN 	0xA0	/*          */
#  define RTC_TCR_1DIOD 	0x04	/*          */
#  define RTC_TCR_2DIOD 	0x08	/*          */
#  define RTC_TCR_DISABLED 	0x00	/*                   */
#  define RTC_TCR_2KOHM 	0x01	/*                */
#  define RTC_TCR_4KOHM 	0x02	/*                */
#  define RTC_TCR_8KOHM 	0x03	/*                */

#ifdef CONFIG_DS1302
extern unsigned char ds1302_readreg(int reg);
extern void ds1302_writereg(int reg, unsigned char val);
extern int ds1302_init(void);
#  define CMOS_READ(x) ds1302_readreg(x)
#  define CMOS_WRITE(val,reg) ds1302_writereg(reg,val)
#  define RTC_INIT() ds1302_init()
#else
  /*                                                      */
#  define CMOS_READ(x) 42
#  define CMOS_WRITE(x,y)
#  define RTC_INIT() (-1)
#endif

/*
                                                                       
                                                                    
                                                               
 */
struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

/*                                                             */
#define RTC_MAGIC 'p'
#define RTC_RD_TIME		_IOR(RTC_MAGIC, 0x09, struct rtc_time)	/*                */
#define RTC_SET_TIME		_IOW(RTC_MAGIC, 0x0a, struct rtc_time)	/*               */
#define RTC_SET_CHARGE  	_IOW(RTC_MAGIC, 0x0b, int)
#define RTC_MAX_IOCTL 0x0b

#endif /*           */
