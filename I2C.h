/*
 * I2C.h
 *
 *  Created on: Feb 12, 2020
 *      Author: LiamMacDonald
 */
#pragma once

typedef struct i2c_command_
{
    unsigned registerToSet : 8;
    unsigned value: 8;
}i2cCommand;

typedef union i2c_deserialize_command_
{
    char* input;
    i2cCommand * command;
}i2cDeserialize;

#ifndef GLOBAL_I2C
#define GLOBAL_I2C

extern void I2C0_Init(void);
extern void processI2C(char*);

#else
#define SLAVE_ADDR 0x64
#define I2C0_SCSR_R             (*((volatile uint32_t *)0x40020804))
#define I2C0_SDR_R              (*((volatile uint32_t *)0x40020808))
#define I2C0_SMIS_R             (*((volatile uint32_t *)0x40020814))  // I2C Slave Masked Interrupt
#define I2C0_SICR_R             (*((volatile uint32_t *)0x40020818))

#endif// GLOBAL_I2C
