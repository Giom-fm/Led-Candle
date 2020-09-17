/* 
 * File:   mpu6050.h
 * Author: Guillaume Fournier-Mayer
 *
 * Created on 25. Juli 2020, 15:13
 * 
 * A module to communicate with an MPU6050 Chip by TWI.
 * This module operates in a blocking way!
 */

#ifndef MPU6050_H
#define	MPU6050_H

#include <stdint.h>

// ## CALIBRATION OFFSETS ######################################################

// 0.035939
#define A_X_OFFSET -0.035939
// -0.010786
#define A_Y_OFFSET 0.010786
// 0.812593
#define A_Z_OFFSET 0.187407

// ## ADDRESSES ################################################################

#define MPU_WRITE_ADDRESS 0xD0
#define MPU_READ_ADDRESS 0xD1

// ## REGISTERS ################################################################

#define PWR_MGMT_1 0x6B
#define SMPRT_DIV 0x19
#define PWR_MGMT_2 0x6C
#define MPU_ACCEL_X 0x3B
#define MPU_ACCEL_Y 0x3D
#define MPU_ACCEL_Z 0x3F
#define MPU_CONFIG 0x1A
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE 0x38

// ## CONFIGS ##################################################################

#define TEMP_DIS 3
#define LP_WAKE_CTRL_L 6
#define LP_WAKE_CTRL_H 7
#define STBY_ZG 0
#define STBY_YG 1
#define STBY_XG 2
#define DLPF_CFG_2 2
#define DLPF_CFG_1 1
#define DLPF_CFG_0 0
#define DATA_RDY_EN 0

// ## SCALE FACTORS ############################################################

#define GYRO_SCALE_FACTOR 131.0
#define ACCEL_SCALE_FACTOR 16384.0

#define CALIBRATE_SAMPLES 100

/**
 * Initializes aand configures the MPU6050 Sensor
 */
void mpu6050_init();

/**
 * Gets the acceleration data for each axis and writes them to the passed
 * variables
 * @param a_x The X Axis pointer
 * @param a_y The Y Axis pointer
 * @param a_z The Z Axis pointer
 */
void mpu6050_get_accel_data(float *a_x, float *a_y, float *a_z);

/**
 * Gets an amount of acceleration data gets the avarage and prints the result
 * to USART
 */
void mpu6050_calibrate();


#endif	/* MPU6050_H */

