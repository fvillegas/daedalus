#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "icaro/timer/timer.h"
#include "icaro/twi/twi.h"
#include "icaro/twi/i2cdevlib.h"
#include "icaro/float.h"
#include "icaro/imu.h"

#include "eeprom/eeprom.h"
#include "sensors/mpu6050.h"
#include "sensors/mpu6050_registers.h"
#include "sensors/hcm5883l.h"
#include "mahony/mahony.h"

#ifdef DEBUG
#include "icaro/uart/uart.h"
char DEBUG_BUFFER[150] = {0};
#endif

//#define STATUS_LED PB5
#define STATUS_LED PB0
union Float f;

uint8_t REGISTER[IMU_REGISTER_LENGTH] = {0};
int16_t gx, gy, gz, ax, ay, az, mx, my, mz;
long last = 0L;
long now = 0L;

void calculate_roll_pitch_yaw()
{
    mpu6050_get_motion_6(&ax, &ay, &az, &gx, &gy, &gz);
    hcm5883l_get_heading(&mx, &my, &mz);
    
    mahony_update(
    gx * 0.001,
    gy * 0.001,
    gz * 0.001,
    ax * 0.001,
    ay * 0.001,
    az * 0.001,
    mx * 0.001,
    my * 0.001,
    mz * 0.001);

    f.m_float = getRoll();
    REGISTER[IMU_ROLL_ADDRESS] = f.m_bytes[0];
    REGISTER[IMU_ROLL_ADDRESS + 1] = f.m_bytes[1];
    REGISTER[IMU_ROLL_ADDRESS + 2] = f.m_bytes[2];
    REGISTER[IMU_ROLL_ADDRESS + 3] = f.m_bytes[3];
    
    f.m_float = getPitch();
    REGISTER[IMU_PITCH_ADDRESS] = f.m_bytes[0];
    REGISTER[IMU_PITCH_ADDRESS + 1] = f.m_bytes[1];
    REGISTER[IMU_PITCH_ADDRESS + 2] = f.m_bytes[2];
    REGISTER[IMU_PITCH_ADDRESS + 3] = f.m_bytes[3];
    
    f.m_float = getYaw();
    REGISTER[IMU_YAW_ADDRESS] = f.m_bytes[0];
    REGISTER[IMU_YAW_ADDRESS + 1] = f.m_bytes[1];
    REGISTER[IMU_YAW_ADDRESS + 2] = f.m_bytes[2];
    REGISTER[IMU_YAW_ADDRESS + 3] = f.m_bytes[3];
}

void calibrate_gyro_accel(void)
{
    int16_t values[6] = {0};
    
    #if DEBUG
    uart_puts("mpu6050 running calibration\n");
    #endif
    
    mpu6050_run_calibration(values);
    mpu6050_save_cal_values(values);
    
    #if DEBUG
    uart_puts("mpu6050 calibration done\n");
    #endif
    
    mpu6050_read_cal_values(values);
    mpu6050_set_x_accel_offset(values[0]);
    mpu6050_set_y_accel_offset(values[1]);
    mpu6050_set_z_accel_offset(values[2]);
    
    mpu6050_set_x_gyro_offset(values[3]);
    mpu6050_set_y_gyro_offset(values[4]);
    mpu6050_set_z_gyro_offset(values[5]);
}

void setup(void)
{
    DDRB |= (1 << STATUS_LED);
    
    init_millis(F_CPU);
    twi_init();
    
    sei();
    
    #ifdef DEBUG
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart_puts("setup finish\n");
    #endif
}

void setup_sensors(void)
{
    mpu6050_init();
    hcm5883l_init();
    
    int16_t values[6] = {0};
        
    mpu6050_read_cal_values(values);
        
    mpu6050_set_x_accel_offset(values[0]);
    mpu6050_set_y_accel_offset(values[1]);
    mpu6050_set_z_accel_offset(values[2]);
        
    mpu6050_set_x_gyro_offset(values[3]);
    mpu6050_set_y_gyro_offset(values[4]);
    mpu6050_set_z_gyro_offset(values[5]);
}

int main(void)
{
    setup();
   
    setup_sensors();
    //calibrate_gyro_accel();
    mahony_init();
    
    while (1)
    {
        now = millis();
        
        calculate_roll_pitch_yaw();
        
        if ((now - last) > 100) {
            PORTB ^= (1 << STATUS_LED);
            #ifdef DEBUG
            sprintf(DEBUG_BUFFER, "rpy %f\t%f\t%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                getRoll(),
                getPitch(),
                getYaw(),
                gx, gy, gz,
                ax, ay, az,
                mx, my, mz);
            uart_puts(DEBUG_BUFFER);
            #endif
            last = now;
        }
    }
}

