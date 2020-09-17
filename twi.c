#include "twi.h"
#include <util/twi.h>
#include "debug.h"

#define DEBUG 0

void twi_start(uint8_t address) {

    while (1) {

        debug_print("TWI Start\r\n");
        // Clear Interrupt, Set START and Enable TWI
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        // Wait until interrupt-flag has been set
        while (!(TWCR & (1 << TWINT)));

        // check Status - if Fail -> Retry
        if ((TWSR & TW_NO_INFO) != TW_START) {
            debug_print("TWI Start-Error: Startcondition not set 0x%x\r\n", TWCR & TW_NO_INFO);
            continue;
        }

        // Set Slave address
        TWDR = address;
        // Clear TWI interrupt flag, Enable TWI
        TWCR = (1 << TWINT) | (1 << TWEN);

        // Wait until interrupt-flag has been set
        while (!(TWCR & (1 << TWINT)));
        // Check Status - If Fail Stop and retry
        if ((TWSR & TW_NO_INFO) != TW_MT_SLA_ACK) {
            debug_print("TWI Start-Error: ACK not received 0x%x\r\n", TWCR & TW_NO_INFO);
            twi_stop();
            continue;
        }
        break;
    }
}

TWI_STATUS twi_repeated_start(uint8_t address) {

    // Clear Interrupt, Set START and Enable TWI
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // Wait until interrupt-flag has been set
    while (!(TWCR & (1 << TWINT)));
    // check Status
    if ((TWSR & TW_NO_INFO) != TW_REP_START)
        return TWI_ERROR;

    // Set Slave address
    TWDR = address;
    // Clear TWI interrupt flag, Enable TWI
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait until interrupt-flag has been set
    while (!(TWCR & (1 << TWINT)));
    // Check Status
    uint8_t status = TWSR & TW_NO_INFO;
    if (status == TW_MR_SLA_ACK)
        return TWI_ACK;
    else if (status == TW_MT_SLA_NACK)
        return TWI_NACK;
    else
        return TWI_ERROR;
}

void twi_stop() {
    debug_print("TWI Stop\r\n");
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

char twi_read_ack() {

    // Clear TWI interrupt flag, Enable TWI, Enable Acknowledge
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    // Wait until interrupt-flag has been set
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

char twi_read_nack() {
    // Clear TWI interrupt flag, Enable TWI
    TWCR = (1 << TWEN) | (1 << TWINT);
    // Wait until interrupt-flag has been set
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

TWI_STATUS twi_write(char data) {

    // Set Data
    TWDR = data;
    // Clear TWI interrupt flag, Enable TWI
    TWCR = (1 << TWINT) | (1 << TWEN);
    // Wait until interrupt-flag has been set
    while (!(TWCR & (1 << TWINT)));
    uint8_t status = TWSR & TW_NO_INFO;
    // Check Status
    if (status == TW_MT_DATA_ACK) {
        debug_print("TWI Write-Status ACK\r\n");
        return TWI_ACK;
    } else if (status == TW_MT_DATA_NACK) {
        debug_print("TWI Write-Status: NACK\r\n");
        return TWI_NACK;
    } else {
        debug_print("TWI Write-Error: No ack or Nack received 0x%x\r\n", status);
        return TWI_ERROR;
    }
}

