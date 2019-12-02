/*
 * bit.h
 *
 *  Created on: 2018Äê2ÔÂ4ÈÕ
 *      Author: WJY
 */

#ifndef SRC_SYS_BIT_H_
#define SRC_SYS_BIT_H_


#define BIT(x) 			(1 << (x))
#define BM(m, l) 		(1 << ((m)+1) - (1 << (l)))
#define HwReg(addr)		*(volatile u32 *)(addr)
//#define CLR(reg, bm) 	(reg) &= ~(bm)
//#define SET(reg, bm) 	(reg) |= (bm)


#endif /* SRC_SYS_BIT_H_ */
