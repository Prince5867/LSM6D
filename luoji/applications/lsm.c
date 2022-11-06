/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-06     admin       the first version
 */
#include <rtdevice.h>
#include <board.h>
#include "drv_common.h"
#include "stm32h7xx_hal_i2c.h"
#include "lsm.h"
#include "stm32h7xx_hal_i2c_ex.h"
I2C_HandleTypeDef hi2c3;
#define ACC_GYRO_ADDRESS     0xD6
rt_err_t i2c3_hw_init(void)
{

      hi2c3.Instance = I2C3;
      hi2c3.Init.Timing = 0x0050174F; //1000k
      //hi2c3.Init.Timing = 0x00B03FDB; //400k
      //hi2c3.Init.Timing = 0x307075B1; //100k
      hi2c3.Init.OwnAddress1 = 0;
      hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      hi2c3.Init.OwnAddress2 = 0;
      hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
      hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
      if (HAL_I2C_Init(&hi2c3) != HAL_OK)
      {
        Error_Handler();
      }
      /** Configure Analogue filter
      */
      if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
      {
        Error_Handler();
      }
      /** Configure Digital filter
      */
      if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
      {
        Error_Handler();
      }
    return RT_EOK;
}
void LSM_WriteOneByte(rt_uint8_t Add,rt_uint8_t WriteAddr,rt_uint8_t DataToWrite)
{
  HAL_I2C_Mem_Write(&hi2c3,Add,WriteAddr, I2C_MEMADD_SIZE_8BIT,(uint8_t *)&(DataToWrite),1,100);
}

rt_uint8_t LSM_ReadOneByte(rt_uint8_t Add,rt_uint8_t ReadAddr)
{
    rt_uint8_t temp=0;
    HAL_I2C_Mem_Read(&hi2c3,Add,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&(temp),1,100);
    return temp;
}
void Lsm_Init_iic(void)
{
  /* while(LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_WHO_AM_I)!=0x6A)
    {
       rt_kprintf("cant find\n");
    }*/
    i2c3_hw_init();
    //LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_CTRL2_G,0X1C);                                          //陀螺仪
    //LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_CTRL10_C,0x38);
    LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_CTRL1_XL,0xA0);
    //LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_TAP_CFG,0x90);
    LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_WAKE_UP_DUR,0x00);
    LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_WAKE_UP_THS,0x02);
    //LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_MD1_CFG,0x20);
    LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_TAP_THS_6D,0x40);
    LSM_WriteOneByte(ACC_GYRO_ADDRESS,LSM6DSL_CTRL8_XL,0x01);
}

void Lsm_Get_Rawacc(int AccData[],int size)
{
    short acc[3],i;
    rt_uint8_t buf[6];
    if((LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_STATUS_REG)&0x01)!=0)
    {
        buf[0]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTX_H_XL);
        buf[1]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTX_L_XL);
        buf[2]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTY_H_XL);
        buf[3]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTY_L_XL);
        buf[4]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTZ_H_XL);
        buf[5]= LSM_ReadOneByte(ACC_GYRO_ADDRESS,LSM6DSL_OUTZ_L_XL);
        acc[0]=(buf[0]<<8)|buf[1];
        acc[1]=(buf[2]<<8)|buf[3];
        acc[2]=(buf[4]<<8)|buf[5];
        for(i=0;i<size;i++)
        {
            AccData[i]=(int32_t)((float)((float)acc[i]* 0.122f));
        }
    }
}
