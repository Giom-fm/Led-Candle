#include "debug.h"

#include <stdbool.h>
#include "mpu6050.h"
#include "twi.h"

#define DEBUG 0

void mpu6050_init_sample_rate()
{

  TWI_STATUS status;

  // Configure Smaplerate
  debug_print("MPU Init: Configure Samplerate\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(SMPRT_DIV);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Samplerate address\r\n");
  status = twi_write(249);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Samplerate value\r\n");
  twi_stop();
}

void mpu6050_init_accel()
{
  TWI_STATUS status;
  // Configure Accelerometer to +-2g
  debug_print("MPU Init: Configure Accelerometer\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(ACCEL_CONFIG);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set accelconfig address\r\n");
  status = twi_write(0x00);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set accelconfig value\r\n");
  twi_stop();
}

void mpu6050_init_interrupt()
{

  TWI_STATUS status;

  // Set Data-Ready-Interrupt
  debug_print("MPU-Init: Init interrupt\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(INT_ENABLE);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Int-Enable Address\r\n");
  status = twi_write((1 << DATA_RDY_EN));
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Int-Enable value\r\n");
  twi_stop();
}

void mpu6050_init_dlpf()
{

  TWI_STATUS status;

  // Set Digital Lowpass Filter
  debug_print("MPU Init: Configure Lowpass filter\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(MPU_CONFIG);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Low-Pass Address\r\n");
  status = twi_write((1 << DLPF_CFG_2) | (1 << DLPF_CFG_1));
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set Low-Pass value\r\n");
  twi_stop();
}

void mpu6050_init_power()
{
  
  TWI_STATUS status;

  // Set Cycle and disable temperature sensor
  debug_print("MPU Init: Configure Powermanagment 1\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(PWR_MGMT_1);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set powermanagment 1 address\r\n");
  status = twi_write((1 << TEMP_DIS));
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set powermanagment 1 value\r\n");
  twi_stop();

  // Disable gyro
  debug_print("MPU Init: Configure Powermanagment 2\r\n");
  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(PWR_MGMT_2);
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set powermanagment 2 address\r\n");
  status = twi_write((1 << STBY_XG) | (1 << STBY_YG) | (1 << STBY_ZG));
  if (status != TWI_ACK)
    debug_print("MPU Init-Error: Set powermanagment 2 value\r\n");
  twi_stop();
}

void mpu6050_init()
{
  //_delay_ms(150);
  mpu6050_init_power();
  mpu6050_init_sample_rate();
  mpu6050_init_accel();
  mpu6050_init_dlpf();
  mpu6050_init_interrupt();
}

int16_t mpu6050_get_data(uint8_t reg)
{

  debug_print("MPU Getdata\r\n");
  TWI_STATUS status;

  twi_start(MPU_WRITE_ADDRESS);
  status = twi_write(reg);
  if (status != TWI_ACK)
    debug_print("MPU Getdata-Error: Set getdata address\r\n");
  status = twi_repeated_start(MPU_READ_ADDRESS);
  if (status != TWI_ACK)
    debug_print("MPU Getdata-Error: Set repeated start address\r\n");

  char data_msb = twi_read_ack();
  char data_lsb = twi_read_nack();
  twi_stop();

  return ((int16_t)data_msb << 8) | data_lsb;
}

void mpu6050_get_accel_data(float *a_x, float *a_y, float *a_z)
{
  *a_x = mpu6050_get_data(MPU_ACCEL_X) / ACCEL_SCALE_FACTOR + A_X_OFFSET;
  *a_y = mpu6050_get_data(MPU_ACCEL_Y) / ACCEL_SCALE_FACTOR + A_Y_OFFSET;
  *a_z = mpu6050_get_data(MPU_ACCEL_Z) / ACCEL_SCALE_FACTOR + A_Z_OFFSET;
  debug_print("a_x:\t%.2f\ta_y:\t%.2f\ta_z:\t%.2f\r\n", *a_x, *a_y, *a_z);
}

void mpu6050_calibrate()
{

  float a_x_raw = 0, a_x_cali = 0;
  float a_y_raw = 0, a_y_cali = 0;
  float a_z_raw = 0, a_z_cali = 0;

  //Take some samples to get an avarage
  for (uint8_t i = 0; i < CALIBRATE_SAMPLES; i++)
  {
    a_x_raw += mpu6050_get_data(MPU_ACCEL_X) / ACCEL_SCALE_FACTOR;
    a_y_raw += mpu6050_get_data(MPU_ACCEL_Y) / ACCEL_SCALE_FACTOR;
    a_z_raw += mpu6050_get_data(MPU_ACCEL_Z) / ACCEL_SCALE_FACTOR;
  }

  a_x_cali = a_x_raw / CALIBRATE_SAMPLES;
  a_y_cali = a_y_raw / CALIBRATE_SAMPLES;
  a_z_cali = a_z_raw / CALIBRATE_SAMPLES;

  debug_print("a_x:\t%.6f\ta_y:\t%.6f\ta_z:\t%.6f\r\n", a_x_cali, a_y_cali,
              a_z_cali);
}
