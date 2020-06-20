/**
 * Based on https://www.i2cdevlib.com/
 * 
 */

#include <stdbool.h>

#include "mpu6050.h"
#include "mpu6050_registers.h"

/**
 * Set clock source setting.
 * 
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU6050_PWR_MGMT_1
 * @see MPU6050_PWR_MGMT_1_CLK_SEL_BIT
 * @see MPU6050_PWR_MGMT_1_CLK_SEL_LENGTH
 */
void mpu6050_set_clock_source(uint8_t source)
{
    write_bits(
        MPU6050_ADDRESS,
        MPU6050_PWR_MGMT_1,
        MPU6050_PWR_MGMT_1_CLK_SEL_BIT,
        MPU6050_PWR_MGMT_1_CLK_SEL_LENGTH, 
        source
    );
}

/**
 * Set full-scale gyroscope range.
 * 
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_GYRO_CONFIG
 * @see MPU6050_GYRO_FS_SEL_BIT
 * @see MPU6050_GYRO_FS_SEL_LENGTH
 */
void mpu6050_set_full_scale_gyro_range(uint8_t range)
{
    write_bits(
        MPU6050_ADDRESS,
        MPU6050_GYRO_CONFIG,
        MPU6050_GYRO_FS_SEL_BIT,
        MPU6050_GYRO_FS_SEL_LENGTH,
        range
    );
}

/**
 * Set full-scale accelerometer range.
 * 
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
void mpu6050_set_full_scale_accel_range(uint8_t range)
{
    write_bits(
        MPU6050_ADDRESS,
        MPU6050_ACCEL_CONFIG,
        MPU6050_ACCEL_CONFIG_AFS_SEL_BIT,
        MPU6050_ACCEL_CONFIG_AFS_SEL_LENGTH,
        range
    );
}

/**
 * Set sleep mode status.
 * 
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
void mpu6050_set_sleep_enabled(bool enabled)
{
    write_bit(
        MPU6050_ADDRESS,
        MPU6050_PWR_MGMT_1,
        MPU6050_PWR_MGMT_1_SLEEP_BIT,
        enabled
    );
}

/**
 * Power on and prepare for general usage.
 * 
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void mpu6050_initialize()
{
    mpu6050_set_clock_source(MPU6050_CLOCK_PLL_XGYRO);
    mpu6050_set_full_scale_gyro_range(MPU6050_GYRO_FS_250);
    mpu6050_set_full_scale_accel_range(MPU6050_ACCEL_FS_2);
    mpu6050_set_sleep_enabled(false); // thanks to Jack Elston for pointing this one out!
}