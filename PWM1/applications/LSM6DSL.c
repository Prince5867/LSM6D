/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-27     admin       the first version
 */
#include "sensor_st_lsm6dsl.h"
#include "list.h"
#include "Pin.h"
List * Idle_list,*Pre_list,*Finlish_list;
int lsm6dsl_port(void)
{
    struct rt_sensor_config cfg;
    cfg.intf.dev_name = "i2c2";
    cfg.intf.user_data = (void *)LSM6DSL_ADDR_DEFAULT;
    cfg.irq_pin.pin = RT_PIN_NONE;
    rt_hw_lsm6dsl_init("lsm6dsl", &cfg);
    return 0;
}
INIT_APP_EXPORT(lsm6dsl_port);
static void sensor_sample(int argc, char **argv)
{
    pwm_control();
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data data;
    rt_size_t res, i;
    Idle_list=createList();
    /* 查找系统中的传感器设备 */
    dev = rt_device_find(argv[1]);
    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)6400);

    if (dev == RT_NULL)
    {
        rt_kprintf("Can't find device:%s\n", argv[1]);
        return;
    }

    /* 以轮询模式打开传感器设备 */
    if (rt_device_open(dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return;
    }

    for (i = 0; i < 40000; i++)
    {
        /* 从传感器读取一个数据 */
        res = rt_device_read(dev, 0, &data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed!size is %d", res);
        }
        else
        {


            Add_data(Idle_list, &data);

        }

    }
    /* 关闭传感器设备 */
    rt_device_close(dev);
    //sensor_show_data(i, (rt_sensor_t)dev, &data);
    show_data(Idle_list);
    FreeList(Idle_list);
    rt_free(Idle_list);

}
MSH_CMD_EXPORT(sensor_sample, sensor device sample);
