/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED.
 *
 ****************************************************************************//*!
 *
 * @file      FRACLIB_Inlines.h
 *
 * @author    R55013
 *
 * @version   1.0.1.0
 *
 * @date      Sep-12-2011
 *
 * @brief     Source file containing routines for calculation 32-bit fractional
 *            arithmetic.
 * 
 *******************************************************************************/ 
#ifndef __FRACLIB_INLINES_H
#define __FRACLIB_INLINES_H  

#include "FRACLIB_Types.h"
#include "FRACLIB_Mul32.h"

/*! USE_FRACLIB_MUL32 source code type selector definition. Comment it out    */
/*! if C-version of the 32x32=64 bit multiplications shall be used instead    */
/*! of assembler functions. Note that fraclib.a must be rebuilt after this    */
/*! constant change                                                           */
#define USE_FRACLIB_MUL32

/****************************************************************************//*!
 *
 * @brief   Extracts the 16 MSBs of a 32-bit fractional value. 
 *          Corresponds to truncation when applied to fractional values.
 *
 * @param   lsrc    - Input 32-bit fractional value.
 *
 * @return  Function returns 16-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac16 S_extract_hi (register Frac32 lsrc)
{
  return (Frac16)((Frac32)lsrc>>16);
}

/****************************************************************************//*!
 *
 * @brief   Extracts the 32 MSBs of a 64-bit fractional value. 
 *          Corresponds to truncation when applied to fractional values.
 *
 * @param   llsrc    - Input 64-bit fractional value.
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_extract_hi (register Frac64 llsrc)
{
  return (Frac32)((Frac64)llsrc>>32);
}

/****************************************************************************//*!
 *
 * @brief   Absolute value of a 32-bit fractional value, returning a 32-bit result.
 *
 * @param   lsrc    - Input 32-bit fractional value.
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language. Where the input is 0x80000000, saturation 
 *          occurs and 0x7fffffff is returned. 
 *
 *******************************************************************************/
static inline Frac32 L_abs (register Frac32 lsrc) 
{ 
  register Frac32 mask = lsrc>>31;
  if (lsrc == 0x80000000) { return      0x7fffffff;   }
  else                    { return (lsrc^mask)-mask;  }
}

/****************************************************************************//*!
 *
 * @brief   Absolute value of a 64-bit fractional value, returning a 64-bit result.
 *
 * @param   lsrc    - Input 64-bit fractional value.
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language. Where the input is 0x8000000000000000, 
 *          saturation occurs and 0x7fffffffffffffff is returned. 
 *
 *******************************************************************************/
static inline Frac64 LL_abs (register Frac64 llsrc) 
{  
  register Frac64 mask = llsrc>>63;
  if (llsrc == 0x8000000000000000) { return 0x7fffffffffffffff; }
  else                             { return  (llsrc^mask)-mask; }
}

/****************************************************************************//*!
 *
 * @brief   Addition of two 32-bit fractional values, returning a 32-bit result.
 *          Doesn't perform saturation.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_add (register Frac32 lsrc1, register Frac32 lsrc2)
{
  return lsrc1+lsrc2;   
}

/****************************************************************************//*!
 *
 * @brief   Addition of two 64-bit fractional values, returning a 64-bit result.
 *          Doesn't perform saturation.
 *
 * @param   llsrc1  - Input 64-bit fractional value.
 * @param   llsrc2  - Input 64-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac64 LL_add (register Frac64 llsrc1, register Frac64 llsrc2)
{
  return llsrc1+llsrc2;   
}

/****************************************************************************//*!
 *
 * @brief   Subtraction of two 32-bit fractional values, returning a 32-bit 
 *          result. Doesn't perform saturation.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_sub (register Frac32 lsrc1, register Frac32 lsrc2)
{
  return lsrc1-lsrc2;
}

/****************************************************************************//*!
 *
 * @brief   Subtraction of two 64-bit fractional values, returning a 64-bit 
 *          result. Doesn't perform saturation.
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac64 LL_sub (register Frac64 llsrc1, register Frac64 llsrc2)
{
  return llsrc1-llsrc2;
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 16-bit fractional values generating 32-bit fractional 
 *          result.
 *
 * @param   ssrc1   - Input 16-bit fractional value.
 * @param   ssrc2   - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_mul_ss (register Frac16 ssrc1, register Frac16 ssrc2)
{
  return ((Frac32)ssrc1*(Frac32)ssrc2)<<1;
}

/****************************************************************************//*!
 *
 * @brief   Multiply 16-bit fractional values with 32-bit fractional value
 *          and return 32-bit fractional result.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   ssrc1   - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_mul_ls (register Frac32 lsrc1, register Frac16 ssrc1)
{
    return (((Frac32)(lsrc1&0xffff)*(Frac32)ssrc1)>>15) +
           (((Frac32)(lsrc1>>16)*(Frac32)ssrc1)<<1); 
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values generating 32-bit fractional 
 *          result.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
#if defined (USE_FRACLIB_MUL32)
  #define L_mul(lsrc1,lsrc2)      (Frac32)(FRACLIB_FFMUL32(lsrc1,lsrc2)>>32)  
#else
  static inline Frac32 L_mul (register Frac32 lsrc1, register Frac32 lsrc2)
  {
    register Frac64 tmp = ((Frac64)lsrc1*(Frac64)lsrc2);  
    return (tmp+tmp)>>32;        
  }
#endif

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values generating 64-bit fractional 
 *          result.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
#if defined (USE_FRACLIB_MUL32)
  #define LL_mul_ll(lsrc1,lsrc2)  (Frac64)FRACLIB_FFMUL32(lsrc1,lsrc2)  
#else
  static inline Frac64 LL_mul_ll (register Frac32 lsrc1, register Frac32 lsrc2)
  {
    register Frac64 tmp = ((Frac64)lsrc1*(Frac64)lsrc2);  
    return tmp+tmp;
  }
#endif   

/****************************************************************************//*!
 *
 * @brief   Multiply 32-bit fractional value with 64-bit fractional value
 *          and return 64-bit fractional result.
 *
 * @param   llsrc1  - Input 64-bit fractional value.
 * @param   lsrc1   - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
#if defined (USE_FRACLIB_MUL32)
  static inline Frac64 LL_mul_lll (register Frac64 llsrc1, register Frac32 lsrc1)
  { 
    return ((FRACLIB_USMUL32 ((Frac32)llsrc1,lsrc1)>>31) +    
             FRACLIB_FFMUL32 ((Frac32)(llsrc1>>32),lsrc1)); 
  }
#else
  static inline Frac64 LL_mul_lll (register Frac64 llsrc1, register Frac32 lsrc1)
  { 
    return (((Frac64)(llsrc1&0xffffffff)*(Frac64)lsrc1)>>31) +
            (((Frac64)(llsrc1>>32)*(Frac64)lsrc1)<<1);
  }
#endif

/****************************************************************************//*!
 *
 * @brief   Multiply two 16-bit fractional values and add to 32-bit fractional 
 *          value. Doesn't perform saturation during accumulation.  
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac32 L_mac_ss (register Frac32 lsrc1, register Frac16 ssrc1, 
                               register Frac16 ssrc2)
{
  return lsrc1+L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 16-bit fractional values and add to 64-bit fractional 
 *          value. Doesn't perform saturation during accumulation.  
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac64 LL_mac_ss (register Frac64 llsrc1, register Frac16 ssrc1, 
                               register Frac16 ssrc2)
{
  return llsrc1+(Frac64)L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional value with 16-bit fractional value 
 *          and add result to 32-bit fractional value. Accumulation doesn't 
 *          saturate.
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac32 L_mac_ls (register Frac32 lsrc1, register Frac32 lsrc2, 
                               register Frac16 ssrc1)
{
  return lsrc1+L_mul_ls(lsrc2,ssrc1);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values and add to 32-bit fractional 
 *          value. Doesn't perform saturation during accumulation.  
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   lsrc3    - Input 32-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac32 L_mac (register Frac32 lsrc1, register Frac32 lsrc2, 
                            register Frac32 lsrc3)
{
  return lsrc1+L_mul(lsrc2,lsrc3);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values and add to 64-bit fractional 
 *          value. Doesn't perform saturation during accumulation.  
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac64 LL_mac_ll (register Frac64 llsrc1, register Frac32 lsrc1, 
                                register Frac32 lsrc2)
{
  return llsrc1+LL_mul_ll(lsrc1,lsrc2);
}

/****************************************************************************//*!
 *
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          add result to 64-bit fractional value. Doesn't perform saturation 
 *          during accumulation.  
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac64 LL_mac_lll (register Frac64 llsrc1, register Frac64 llsrc2, 
                                 register Frac32 lsrc1)
{
  return llsrc1+LL_mul_lll(llsrc2,lsrc1);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 16-bit fractional values and subtract from 32-bit 
 *          fractional value. Doesn't perform saturation during accumulation.  
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac32 L_msu_ss (register Frac32 lsrc1, register Frac16 ssrc1, 
                               register Frac16 ssrc2)
{
  return lsrc1-L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional value with 16-bit fractional value 
 *          and subtract result from 32-bit fractional value. Doesn't perform 
 *          saturation during accumulation.
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac32 L_msu_ls (register Frac32 lsrc1, register Frac32 lsrc2, 
                               register Frac16 ssrc1)
{
  return lsrc1-L_mul_ls(lsrc2,ssrc1);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values and subtract it from 
 *          32-bit fractional value. Doesn't perform saturation during 
 *          accumulation.  
 *
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   lsrc3    - Input 32-bit fractional value.         
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/  
static inline Frac32 L_msu (register Frac32 lsrc1, register Frac32 lsrc2, 
                            register Frac32 lsrc3)
{
  return lsrc1-L_mul(lsrc2,lsrc3);
}

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values and subtract it from 64-bit 
 *          fractional value. Doesn't perform saturation during accumulation.  
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac64 LL_msu_ll (register Frac64 llsrc1, register Frac32 lsrc1, 
                                register Frac32 lsrc2)
{
  return llsrc1-LL_mul_ll(lsrc1,lsrc2);
}

/****************************************************************************//*!
 *
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          subtract it from 64-bit fractional value. Doesn't perform saturation 
 *          during accumulation.  
 *
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/   
static inline Frac64 LL_msu_lll (register Frac64 llsrc1, register Frac64 llsrc2, 
                                 register Frac32 lsrc1)
{
  return llsrc1-LL_mul_lll(llsrc2,lsrc1);
}

/****************************************************************************//*!
 *
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-na)
 *
 *          Internal accumulations don't saturate. The IIR filter output is 
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff..
 *
 * @param   x       - Input fractional value represented in 16-bit fractional 
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit 
 *                    fractional format "x(n-1)".
 * @param   *py     - Pointer to previous output values represented in 16-bit 
 *                    fractional format "y(n-1)".
 * @param   *pcoef  - Pointer to filter constants represented in 16-bit 
 *                    fractional format "b(1), b(2) and a(2)".
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_iir_1ord_ss (register Frac16 x, register Frac16 *px, 
                                    register Frac32 *py, const Frac16 *pcoef)
{ 
  register Frac32 tmp;
  
  tmp = L_msu_ls(L_mac_ss(L_mul_ss(x,*pcoef),*px,*(pcoef+1)),*py,*(pcoef+2)); 
  *px = x;
    
  return *py = tmp;
}

/****************************************************************************//*!
 *
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(1)*y(n-1)
 *
 *          Internal accumulations don't saturate. The IIR filter output is 
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x       - Input fractional value represented in 16-bit fractional 
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit 
 *                    fractional format "x(n-1)".
 * @param   *py     - Pointer to previous output values represented in 32-bit 
 *                    fractional format "y(n-1)".
 * @param   *pcoef  - Pointer to filter constants represented in 32-bit 
 *                    fractional format "b(1), b(2) and a(2)".
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_iir_1ord_sl (register Frac16 x, register Frac16 *px, 
                                    register Frac32 *py, const Frac32 *pcoef)
{ 
  register Frac32 tmp;
  
  tmp = L_msu(L_mac_ls(L_mul_ls(*pcoef,x),*(pcoef+1),*px),*py,*(pcoef+2)); 
  *px = x;
    
  return *py = tmp;
}

/****************************************************************************//*!
 *
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *
 *          Internal accumulations don't saturate. The FIR filter output is 
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x        - Input fractional value represented in 32-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 32-bit 
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit 
 *                     fractional format "b(1), b(2) and a(2)".
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac32 L_iir_1ord (register Frac32 x, register Frac32 *px, 
                                 register Frac32 *py, const Frac32 *pcoef)
{ 
  register Frac32 tmp;
  
  tmp = L_msu(L_mac(L_mul(x,*pcoef),*px,*(pcoef+1)),*py,*(pcoef+2)); 
  *px = x;
    
  return *py = tmp;
}

/****************************************************************************//*!
 *
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *
 *          Internal accumulations don't saturate. The IIR filter output is within  
 *          64-bit fractional range from 0x800000000000000 to 0x7fffffffffffff.
 *
 * @param   x        - Input fractional value represented in 32-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 64-bit 
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional 
 *                     format "b(1), b(2) and a(2)".
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac64 LL_iir_1ord_ll (register Frac32 x, register Frac32 *px, 
                                     register Frac64 *py, const Frac32 *pcoef)
{ 
  register Frac64 tmp;
  
  tmp = LL_sub(LL_mac_ll(LL_mul_ll(x,*pcoef),*px,*(pcoef+1)),LL_mul_lll(*py,*(pcoef+2))); 
  *px = x;
    
  return *py = tmp;
}

/****************************************************************************//*!
 *
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *
 *          Internal accumulations don't saturate. The IIR filter output is 
 *          within 64-bit fractional range from 0x800000000000000 to 
 *          0x7fffffffffffff.
 *
 * @param   x        - Input fractional value represented in 64-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 64-bit 
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit 
 *                     fractional format "b(1), b(2) and a(2)".
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
static inline Frac64 LL_iir_1ord (register Frac64 x, register Frac64 *px, 
                                  register Frac64 *py, const Frac32 *pcoef)
{ 
  register Frac64 tmp;
  
  tmp = LL_sub(LL_mac_lll(LL_mul_lll(x,*pcoef),*px,*(pcoef+1)),LL_mul_lll(*py,*(pcoef+2))); 
  *px = x;
    
  return *py = tmp;
}
#endif /* __FRACLIB_INLINES_H */ 