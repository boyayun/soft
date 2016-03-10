/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************//*!
 *
 * @file      FRACLIB.h
 *
 * @author    R55013
 *
 * @version   1.0.2.0
 *
 * @date      Feb-09-2013
 *
 * @brief     Header file containing common data types, macros and list of 
 *            exported functions supporting 32-bit fractional arithmetic.
 *
 ******************************************************************************/
#ifndef __FRACLIB_H
#define __FRACLIB_H

/******************************************************************************
 * include header files                                                       *
 ******************************************************************************/
#include "FRACLIB_Types.h"
#include "FRACLIB_Mul32.h"
#include "FRACLIB_Inlines.h" 

/******************************************************************************
 * exported function prototypes                                               *
 ******************************************************************************/
extern Frac16 S_div_ll  (register Frac32 num, register Frac32 den);
extern Frac32 L_div     (register Frac32 num, register Frac32 den);
extern Frac32 L_div_lll (register Frac64 num, register Frac32 den);
extern Frac16 S_div_llll(register Frac64 num, register Frac64 den);
extern Frac64 LL_div    (register Frac64 num, register Frac64 den);

extern Frac32 L_sqr     (register Frac32 x);
extern Frac32 L_sqr_ll  (register Frac64 x);
extern Frac64 LL_sqr    (register Frac64 x);

extern Frac32 L_fir_ss  (register Frac16 x, register Frac16 *px, const Frac16 *pcoef, 
                         register Word16 len);
extern Frac32 L_fir_sl  (register Frac16 x, register Frac16 *px, const Frac32 *pcoef, 
                         register Word16 len);
extern Frac32 L_fir_ls  (register Frac32 x, register Frac32 *px, const Frac16 *pcoef, 
                         register Word16 len);
extern Frac32 L_fir     (register Frac32 x, register Frac32 *px, const Frac32 *pcoef, 
                         register Word16 len);
extern Frac64 LL_fir_ll (register Frac32 x, register Frac32 *px, const Frac32 *pcoef, 
                         register Word16 len);
extern Frac64 LL_fir_lll(register Frac64 x, register Frac64 *px, const Frac32 *pcoef, 
                         register Word16 len);

extern Frac32 L_iir_ss  (register Frac16 x, register Frac16 *px, register Word16 len_x, 
                         register Frac32 *py, register Word16 len_y, const Frac16 *pcoef);
extern Frac32 L_iir     (register Frac32 x, register Frac32 *px, register Word16 len_x, 
                         register Frac32 *py, register Word16 len_y, const Frac32 *pcoef);
			   	     
#endif /* __FRACLIB_H */
			   
