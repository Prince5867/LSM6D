/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
//#include "lsm.h"


int main(void)
{
/*    Lsm_Init_spi();
    rt_thread_mdelay(10);
    rt_kprintf("init suc1111\n");
    for(int i=0 ; i<10000 ; i++)
    {
        Lsm_Get_RawAcc();
    }*/
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


