#ifndef __EEPROM_H_
#define __EEPROM_H_

#define AX_OFFSET_ADDRESS 0     // two bytes
#define AY_OFFSET_ADDRESS 2     // two bytes
#define AZ_OFFSET_ADDRESS 4     // two bytes
#define GX_OFFSET_ADDRESS 6     // two bytes
#define GY_OFFSET_ADDRESS 8     // two bytes
#define GZ_OFFSET_ADDRESS 10    // two bytes

void mpu6050_read_cal_values(int16_t *values);
void mpu6050_save_cal_values(int16_t *values);

#endif