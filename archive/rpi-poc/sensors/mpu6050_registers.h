#ifndef __MPU6050_REGISTERS_H_
#define __MPU6050_REGISTERS_H_

#define MPU6050_ADDRESS                                 0x68

#define MPU6050_SELF_TEST_X                             0x0D
#define MPU6050_SELF_TEST_Y                             0x0E
#define MPU6050_SELF_TEST_Z                             0x0F
#define MPU6050_SELF_TEST_A                             0x10
#define MPU6050_SMPLRT_DIV                              0x19
#define MPU6050_CONFIG                                  0x1A

// start gyro config
#define MPU6050_GYRO_CONFIG                             0x1B

#define MPU6050_GYRO_FS_250                             0X00
#define MPU6050_GYRO_FS_500                             0x01
#define MPU6050_GYRO_FS_1000                            0X02
#define MPU6050_GYRO_FS_2000                            0X03

#define MPU6050_GYRO_FS_SEL_BIT                         4
#define MPU6050_GYRO_FS_SEL_LENGTH                      2
// end gyro config

// start accel config
#define MPU6050_ACCEL_CONFIG                            0x1C

#define MPU6050_ACCEL_CONFIG_AFS_SEL_BIT                4
#define MPU6050_ACCEL_CONFIG_AFS_SEL_LENGTH             2

#define MPU6050_ACCEL_FS_2                              0X00
#define MPU6050_ACCEL_FS_4                              0X01
#define MPU6050_ACCEL_FS_8                              0X02
#define MPU6050_ACCEL_FS_16                             0X03
// ends accel config

#define MPU6050_FIFO_EN                                 0x23
#define MPU6050_I2C_MST_CTRL                            0x24
#define MPU6050_I2C_SLV0_ADDR                           0x25
#define MPU6050_I2C_SLV0_REG                            0x26
#define MPU6050_I2C_SLV0_CTRL                           0x27
#define MPU6050_I2C_SLV1_ADDR                           0x28
#define MPU6050_I2C_SLV1_REG                            0x29
#define MPU6050_I2C_SLV1_CTRL                           0x2A
#define MPU6050_I2C_SLV2_ADDR                           0x2B
#define MPU6050_I2C_SLV2_REG                            0x2C
#define MPU6050_I2C_SLV2_CTRL                           0x2D
#define MPU6050_I2C_SLV3_ADDR                           0x2E
#define MPU6050_I2C_SLV3_REG                            0x2F
#define MPU6050_I2C_SLV3_CTRL                           0x30
#define MPU6050_I2C_SLV4_ADDR                           0x31
#define MPU6050_I2C_SLV4_REG                            0x32
#define MPU6050_I2C_SLV4_DO                             0x33
#define MPU6050_I2C_SLV4_CTRL                           0x34
#define MPU6050_I2C_SLV4_DI                             0x35
#define MPU6050_I2C_MST_STATUS                          0x36

// start int pin cfg
#define MPU6050_INT_PIN_CFG                             0x37

#define MPU6050_INT_PIN_CFG_I2C_BYPASS_EN_BIT           1
// ends int pin cfg

#define MPU6050_INT_ENABLE                              0x38
#define MPU6050_INT_STATUS                              0x3A

#define MPU6050_ACCEL_XOUT_H                            0x3B
#define MPU6050_ACCEL_XOUT_L                            0x3C
#define MPU6050_ACCEL_YOUT_H                            0x3D
#define MPU6050_ACCEL_YOUT_L                            0x3E
#define MPU6050_ACCEL_ZOUT_H                            0x3F
#define MPU6050_ACCEL_ZOUT_L                            0x40

#define MPU6050_TEMP_OUT_H                              0x41
#define MPU6050_TEMP_OUT_L                              0x42

#define MPU6050_GYRO_XOUT_H                             0x43
#define MPU6050_GYRO_XOUT_L                             0x44
#define MPU6050_GYRO_YOUT_H                             0x45
#define MPU6050_GYRO_YOUT_L                             0x46
#define MPU6050_GYRO_ZOUT_H                             0x47
#define MPU6050_GYRO_ZOUT_L                             0x48

#define MPU6050_EXT_SENS_DATA_00                        0x49
#define MPU6050_EXT_SENS_DATA_01                        0x4A
#define MPU6050_EXT_SENS_DATA_02                        0x4B
#define MPU6050_EXT_SENS_DATA_03                        0x4C
#define MPU6050_EXT_SENS_DATA_04                        0x4D
#define MPU6050_EXT_SENS_DATA_05                        0x4E
#define MPU6050_EXT_SENS_DATA_06                        0x4F
#define MPU6050_EXT_SENS_DATA_07                        0x50
#define MPU6050_EXT_SENS_DATA_08                        0x51
#define MPU6050_EXT_SENS_DATA_09                        0x52
#define MPU6050_EXT_SENS_DATA_10                        0x53
#define MPU6050_EXT_SENS_DATA_11                        0x54
#define MPU6050_EXT_SENS_DATA_12                        0x55
#define MPU6050_EXT_SENS_DATA_13                        0x56
#define MPU6050_EXT_SENS_DATA_14                        0x57
#define MPU6050_EXT_SENS_DATA_15                        0x58
#define MPU6050_EXT_SENS_DATA_16                        0x59
#define MPU6050_EXT_SENS_DATA_17                        0x5A
#define MPU6050_EXT_SENS_DATA_18                        0x5B
#define MPU6050_EXT_SENS_DATA_19                        0x5C
#define MPU6050_EXT_SENS_DATA_20                        0x5D
#define MPU6050_EXT_SENS_DATA_21                        0x5E
#define MPU6050_EXT_SENS_DATA_22                        0x5F
#define MPU6050_EXT_SENS_DATA_23                        0x60
#define MPU6050_I2C_SLV0_DO                             0x63
#define MPU6050_I2C_SLV1_DO                             0x64
#define MPU6050_I2C_SLV2_DO                             0x65
#define MPU6050_I2C_SLV3_DO                             0x66
#define MPU6050_I2C_MST_DELAY_CTRL                      0x67
#define MPU6050_SIGNAL_PATH_RESET                       0x68

// start user ctrl
#define MPU6050_USER_CTRL                               0x6A

#define MPU6050_USER_CTRL_I2C_MST_EN_BIT                5
// ends user ctrl

// start power managment 1
#define MPU6050_PWR_MGMT_1                              0x6B

#define MPU6050_PWR_MGMT_1_RESET_BIT                    7
#define MPU6050_PWR_MGMT_1_SLEEP_BIT                    6
#define MPU6050_PWR_MGMT_1_CLK_SEL_BIT                  2

#define MPU6050_PWR_MGMT_1_CLK_SEL_LENGTH               3

#define MPU6050_CLOCK_PLL_XGYRO                         0x01
#define MPU6050_CLOCK_PLL_ZGYRO                         0X03
// ends power managment 1

#define MPU6050_PWR_MGMT_2                              0x6C
#define MPU6050_FIFO_COUNTH                             0x72
#define MPU6050_FIFO_COUNTL                             0x73
#define MPU6050_FIFO_R_W                                0x74
#define MPU6050_WHO_AM_I                                0x75

#endif /* MPU6050_REGISTERS_H_ */
