#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define MPU6050_ADDRESS 0xD0

void MPU6050_Init(void);
uint8_t MPU6050_Read_Data(void);
float MPU6050_Get_Ax(void);
float MPU6050_Get_Ay(void);
float MPU6050_Get_Az(void);
float MPU6050_Get_Gx(void);
float MPU6050_Get_Gy(void);
float MPU6050_Get_Gz(void);
float MPU6050_Get_Temperature(void);

#endif
