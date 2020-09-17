/* 
 * File:   twi.h
 * Author: Guillaume Fournier-Mayer
 *
 * Created on 25. Juli 2020, 14:44
 * 
 * A module to send so data over an TWI Bus.
 * This modules operates in a blocking way!
 */

#ifndef TWI_H
#define	TWI_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Describes the state of some TWI operations.
 */

typedef enum {
    TWI_ERROR = -1,
    TWI_ACK = 0,
    TWI_NACK = 1
} TWI_STATUS;

/**
 * Sets the start condition.
 * Blocks until master mode has been reached.
 * 
 * @param address The slave address to read or write to.
 */
void twi_start(uint8_t address);

/**
 * Sets the repeated start condition.
 * 
 * @param address The address to write or read to.
 * @return The state of the operation.
 */
TWI_STATUS twi_repeated_start(uint8_t address);

/**
 * Sets the stop condition.
 * Blocks until condition ist met.
 */
void twi_stop();

/**
 * Writes one byte of data to the bus.
 * 
 * @param data The data to write.
 * @return The state of the write operation.
 */
TWI_STATUS twi_write(char data);

/**
 * Reads a byte of data and sends a ACK.
 * @return The Data.
 */
char twi_read_ack();

/**
 * Reads a byte of data and sends a NACK.
 * @return The Data.
 */
char twi_read_nack();

#endif	/* TWI_H */

