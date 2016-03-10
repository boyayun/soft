/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      crt0.s
 * @version   1.0.1.0
 * @date      May-18-2012   
 * @brief     startup function (low level part).
 ******************************************************************************/
    PUBLIC  __iar_program_start
    EXTERN  start    
    SECTION .text:CODE
__iar_program_start
    EORS    r0,r0                           ; Initialize the GPRs
    EORS    r1,r1                           ; Initialize the GPRs
    EORS    r2,r2                           ; Initialize the GPRs
    EORS    r3,r3                           ; Initialize the GPRs
    EORS    r4,r4                           ; Initialize the GPRs
    EORS    r5,r5                           ; Initialize the GPRs
    EORS    r6,r6                           ; Initialize the GPRs
    EORS    r7,r7                           ; Initialize the GPRs
    CPSID   i                               ; disable interrupts
    BL      start                           ; call the C code
__done
    B       __done
    END
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
