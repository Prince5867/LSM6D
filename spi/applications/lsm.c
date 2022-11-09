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
#include "lsm.h"
#include "stm32h7xx_hal_spi.h"

#define ACC_GYRO_ADDRESS     0xD6
SPI_HandleTypeDef hspi2;
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void LSM6DSL_Write_Reg(rt_uint8_t add,rt_uint8_t value)
{
    rt_uint8_t pdata[2];
    pdata[0]=add;
    pdata[1]=value;
    HAL_SPI_Transmit(&hspi2, (uint8_t *)pdata, 2,100);
}

rt_uint8_t LSM6DSL_Read_Reg(rt_uint8_t add)
{
    rt_uint8_t temp[2];
    rt_uint8_t cmd;
    cmd=(add|0x80);

    HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&cmd, temp, 2, 10);
    return temp[1];
}
void Lsm_Init_spi(void)
{
        MX_SPI2_Init();
      // LSM6DSL_Write_Reg(LSM6DSL_CTRL2_G,0X1C);
        //LSM6DSL_Write_Reg(LSM6DSL_CTRL10_C,0x38);

        LSM6DSL_Write_Reg(LSM6DSL_CTRL1_XL,0xA0);
       // LSM6DSL_Write_Reg(LSM6DSL_TAP_CFG,0x90);
        LSM6DSL_Write_Reg(LSM6DSL_WAKE_UP_DUR,0x00);
        LSM6DSL_Write_Reg(LSM6DSL_WAKE_UP_THS,0x02);
        //LSM6DSL_Write_Reg(LSM6DSL_MD1_CFG,0x20);

        LSM6DSL_Write_Reg(LSM6DSL_TAP_THS_6D,0x40);
        LSM6DSL_Write_Reg(LSM6DSL_CTRL8_XL,0x01);
        rt_kprintf("init suc\n");
}

void Lsm_Get_RawAcc(int AccData[],int size)
{
    short acc[3],i;
    rt_uint8_t buf[6];
    if((LSM6DSL_Read_Reg(LSM6DSL_STATUS_REG)&0x01)!=0)
      {
        buf[0]= LSM6DSL_Read_Reg(LSM6DSL_OUTX_H_XL);
        buf[1]= LSM6DSL_Read_Reg(LSM6DSL_OUTX_L_XL);
        buf[2]= LSM6DSL_Read_Reg(LSM6DSL_OUTY_H_XL);
        buf[3]= LSM6DSL_Read_Reg(LSM6DSL_OUTY_L_XL);
        buf[4]= LSM6DSL_Read_Reg(LSM6DSL_OUTZ_H_XL);
        buf[5]= LSM6DSL_Read_Reg(LSM6DSL_OUTZ_L_XL);
        acc[0]=(buf[0]<<8)|buf[1];
        acc[1]=(buf[2]<<8)|buf[3];
        acc[2]=(buf[4]<<8)|buf[5];
        for(i=0;i<size;i++)
        {
             AccData[i]=(int32_t)((float)((float)acc[i]* 0.122f));
        }
      }
}
