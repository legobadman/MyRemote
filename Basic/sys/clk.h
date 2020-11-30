/**************************************************************************//**
 * @file     clk.h
 * @version  V3.0
 * $Revision: 16 $
 * $Date: 15/07/02 11:21a $
 * @brief    NUC123 Series Clock Control Driver Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2014~2015 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __CLK_H__
#define __CLK_H__

#ifdef __cplusplus
extern "C"
{
#endif

__STATIC_INLINE void CLK_SysTickDelay(uint32_t us)
{
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  = (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    
    /* Disable SysTick counter */
    SysTick->CTRL = 0;    
}

#ifdef __cplusplus
}
#endif

#endif //__CLK_H__



/*** (C) COPYRIGHT 2014~2015 Nuvoton Technology Corp. ***/
