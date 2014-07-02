/*
                                   
  
                                                        
 */

#include <linux/init.h>
#include <asm/io.h>
#include "proto.h"

void __init
es1888_init(void)
{
	/*                                                   */
	inb(0x0229);
	inb(0x0229);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x0220); /*                                     */

	/*                              */
	outb(0x01, 0x0226);		/*       */
	inb(0x0226);			/*       */
	outb(0x00, 0x0226);		/*               */
	while (!(inb(0x022e) & 0x80))	/*                         */
		continue;
	inb(0x022a);			/*       */
	outb(0xc6, 0x022c);		/*                      */
	inb(0x022a);			/*                              */
	while (inb(0x022c) & 0x80)	/*                            */
		continue;
	outb(0xb1, 0x022c);		/*                                 */
	while (inb(0x022c) & 0x80)	/*                            */
		continue;
	outb(0x14, 0x022c);		/*           */
	while (inb(0x022c) & 0x80)	/*                            */
		continue;
	outb(0xb2, 0x022c);		/*                           */
	while (inb(0x022c) & 0x80)	/*                            */
		continue;
	outb(0x18, 0x022c);		/*                   */
	inb(0x022c);			/*                 */
}
