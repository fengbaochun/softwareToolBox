#include "mpu6050.h"
//#include "delay.h"
//#include "usart.h"
#include "i2c.h"
//#include "Print.h"

mpuType imu;

/**
 * @brief 初始化MPU6050
 * @param 无
 * @return 状态 0成功 其他失败
 * @author HZ12138
 * @date 2022-08-08 14:51:59
 */
uint8_t MPU_Init(void)
{
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); //复位MPU6050
	HAL_Delay(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); //唤醒MPU6050
	MPU_Set_Gyro_Fsr(3);					 //陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					 //加速度传感器,±2g
	MPU_Set_Rate(50);						 //设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);	 //关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);	 //关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效
	MPU_Read_Byte(MPU_DEVICE_ID_REG);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); //设置CLKSEL,PLL X轴为参考
	MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); //加速度与陀螺仪都工作
	MPU_Set_Rate(200);						 //设置采样率为50Hz
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); //设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); //设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data = 0;
	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data); //设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); //设置数字低通滤波器
	return MPU_Set_LPF(rate / 2);					  //自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((uint16_t)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;
	;
}
//得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((uint16_t)buf[0] << 8) | buf[1];
		*gy = ((uint16_t)buf[2] << 8) | buf[3];
		*gz = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
}
//得到加速度值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((uint16_t)buf[0] << 8) | buf[1];
		*ay = ((uint16_t)buf[2] << 8) | buf[3];
		*az = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
	;
}


float mpu6050_get_accel_res(uint8_t accel_scale)
{
    float accel_res = 0;
    switch (accel_scale){
		case 0:	accel_res = 2.0 / 32768.0;	break;
		case 1:	accel_res = 4.0 / 32768.0;	break;
		case 2:	accel_res = 8.0 / 32768.0;	break;
		case 3:	accel_res = 16.0 / 32768.0;	break;
    }
    return (accel_res);
}

float mpu6050_get_gyro_res(uint8_t gyro_scale)
{
    float gyro_res = 0;
    switch (gyro_scale){
		case 0:	gyro_res = 250.0 / 32768.0;	break;
		case 1:	gyro_res = 500.0 / 32768.0;	break;
		case 2:	gyro_res = 1000.0 / 32768.0;break;
		case 3:	gyro_res = 2000.0 / 32768.0;break;
    }
    return (gyro_res);
}

uint8_t dmpGetGravity(VectorFloat *v, Quaternion *q) {
    v->x = 2 * (q->x*q->z - q->w*q->y);
    v->y = 2 * (q->w*q->x + q->y*q->z);
    v->z = q->w*q->w - q->x*q->x - q->y*q->y + q->z*q->z;
    return 0;
}

uint8_t dmpGetLinearAccel(VectorFloat *v, VectorFloat *vRaw, VectorFloat *gravity) {
    // get rid of the gravity component (+1g = +16384 in standard DMP FIFO packet, sensitivity is 2g)
    v->x = vRaw->x - gravity->x;//*16384;
    v->y = vRaw->y - gravity->y;//*16384;
    v->z = vRaw->z - gravity->z;//*16384;
    return 0;
}

uint8_t dmpGetLinearAccelInWord(VectorFloat *v, VectorFloat *aaReal, Quaternion *q) {
    // get rid of the gravity component (+1g = +16384 in standard DMP FIFO packet, sensitivity is 2g)
	
	float q0q1, q0q2, q1q1, q1q3, q2q2, q2q3, q3q3, q1q2, q0q3; //q0q0,

	q0q1 = q->w * q->x;
	q0q2 = q->w * q->y;
	q1q1 = q->x * q->x;
	q1q3 = q->x * q->z;
	q2q2 = q->y * q->y;
	q2q3 = q->y * q->z;
	q3q3 = q->z * q->z;
	q1q2 = q->x * q->y;
	q0q3 = q->w * q->z;
	
	//方向向量单位化
	VectorFloat x_vec, y_vec, z_vec;
	// 载体坐标下的x方向向量，单位化。
	x_vec.x = 1 - (2 * q2q2 + 2 * q3q3);
	x_vec.y = 2 * q1q2 - 2 * q0q3;
	x_vec.z = 2 * q1q3 + 2 * q0q2;

	// 载体坐标下的y方向向量，单位化。
	y_vec.x = 2 * q1q2 + 2 * q0q3;
	y_vec.y = 1 - (2 * q1q1 + 2 * q3q3);
	y_vec.z = 2 * q2q3 - 2 * q0q1;

	// 载体坐标下的z方向向量（等效重力向量、重力加速度向量），单位化。
	z_vec.x = 2 * q1q3 - 2 * q0q2;
	z_vec.y = 2 * q2q3 + 2 * q0q1;
	z_vec.z = 1 - (2 * q1q1 + 2 * q2q2);
	
	// 计算世界坐标下的运动加速度。(与姿态解算无关)
	v->x = x_vec.x * aaReal->x + x_vec.y * aaReal->y;
	v->y = y_vec.x * aaReal->x + y_vec.y * aaReal->y;
	v->z = z_vec.x * aaReal->x + z_vec.y * aaReal->y + z_vec.z * aaReal->z;

    return 0;
}
