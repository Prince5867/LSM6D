/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-21     admin       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

    float k=(6000./65536.);
    float F;
#define ADC_DEV_NAME        "adc1"      /* ADC �豸���� */
#define ADC_DEV_CHANNEL     5          /* ADC ͨ�� */
#define REFER_VOLTAGE       330         /* �ο���ѹ 3.3V,���ݾ��ȳ���100����2λС��*/
#define CONVERT_BITS        (1 << 16)   /* ת��λ��Ϊ16λ */
rt_thread_t adc_thread;   //ADC�߳̾��
rt_adc_device_t adc_dev;  //ADC�豸���

rt_sem_t sem1; //����Ҫ�õ��ź���
rt_sem_t sem2;
/*ѹ����С��ֵ*/
const rt_uint32_t Pre_Pressure =1000;
const rt_uint32_t Finlish_Pressure =2000;

static void adc_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t value;

    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc device can't find��\n");
    }
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    if(ret!=RT_EOK)
    {
        rt_kprintf("adc start failed��\n");
    }

    while(1)
    {

        value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
        F=(6000.-value*k);
       // Add_adc_data(adc_list, F);
        //rt_kprintf("the value is : %d\n",(rt_uint32_t)F);
/*        if((value>=Pre_Pressure)&&(value<=Finlish_Pressure))
        {
            rt_sem_release(sem1);
        }
        if(value>=Finlish_Pressure)
        {
            rt_sem_release(sem2);
        }*/
        //vol = value * REFER_VOLTAGE / CONVERT_BITS;
        //rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);
        rt_thread_mdelay(100);
    }
}
static int adc_init()
{
    adc_thread = rt_thread_create("adc_thread",adc_thread_entry,RT_NULL,512,10,10);
  if (adc_thread != RT_NULL)
    {
        rt_thread_startup(adc_thread);
    }
    sem1=rt_sem_create("sem1", 0, RT_IPC_FLAG_FIFO);
    sem2=rt_sem_create("sem2", 0, RT_IPC_FLAG_FIFO);
    return RT_EOK;
}
INIT_APP_EXPORT(adc_init);


