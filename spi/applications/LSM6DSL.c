/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-27     admin       the first version
 */
#include <rtthread.h>
#include "list.h"
#include "Pin.h"
#include "lsm.h"
extern adc_List * adc_list;
List * Idle_list,*Pre_list,*Finlish_list;
extern rt_thread_t adc_thread;

static void sensor_sample()
{
    //pwm_control();
    //rt_thread_startup(adc_thread);
    Acc_Data data;
    rt_size_t i;
    int acc_data[3];
    Idle_list=createList();
    Lsm_Init_spi();
    rt_thread_mdelay(100);
    for (i = 0; i < 200000; i++)
    {
        if(Lsm_Get_RawAcc(acc_data,3))
        {
            data.data[0]=acc_data[0];
            data.data[1]=acc_data[1];
            data.data[2]=acc_data[2];
            Add_data(Idle_list, &data);
        }
    }
    show_data(Idle_list);
   // show_adc_data(adc_list);
    //rt_thread_delete(adc_thread);
    FreeList(Idle_list);
    rt_free(Idle_list);
    //Free_adc_List(adc_list);
    //rt_free(adc_list);
}
MSH_CMD_EXPORT(sensor_sample, sensor sample);

