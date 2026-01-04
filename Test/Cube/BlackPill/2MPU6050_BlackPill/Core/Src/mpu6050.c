

#include <stdint.h>
#include "mpu6050.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t data_tx[2];
uint8_t data_rx[15];

void MPU6050_Init(void)
{
	data_tx[0] = 0x6B;
	data_tx[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MPU6050_ADDRESS, data_tx, 2, 100);

	data_tx[0] = 0x19;
	data_tx[1] = 0x07;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MPU6050_ADDRESS, data_tx, 2, 100);
}

uint8_t MPU6050_Read_Data(void)
{
	HAL_StatusTypeDef status;
	data_tx[0] = 0x3B;
	data_tx[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MPU6050_ADDRESS, &data_tx[0], 1, 100);
	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t)MPU6050_ADDRESS, data_rx, 14, 100);
	return (status == HAL_OK) ? 1 : 0;
}

float MPU6050_Get_Ax(void)
{
	return (float)(((int16_t)(data_rx[0]<<8 | data_rx[1]))/(float)16384);
}

float MPU6050_Get_Ay(void)
{
	return (float)(((int16_t)(data_rx[2]<<8 | data_rx[3]))/(float)16384);
}

float MPU6050_Get_Az(void)
{
	return (float)(((int16_t)(data_rx[4]<<8 | data_rx[5]))/(float)16384);
}

float MPU6050_Get_Gx(void)
{
	return (float)(((int16_t)(data_rx[10]<<8 | data_rx[11]))/(float)131);
}

float MPU6050_Get_Gy(void)
{
	return (float)(((int16_t)(data_rx[12]<<8 | data_rx[13]))/(float)131);
}

float MPU6050_Get_Gz(void)
{
	return (float)(((int16_t)(data_rx[8]<<8 | data_rx[9]))/(float)131);
}

float MPU6050_Get_Temperature(void)
{
	return (float)(((int16_t)(data_rx[6]<<8 | data_rx[7]))/(float)340 + (float)36.53);
}
