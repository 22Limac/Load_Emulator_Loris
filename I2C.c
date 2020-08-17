/*
 * I2C.c
 *
 *  Created on: Feb 9, 2020
 *      Author: LiamMacDonald
 */
#define GLOBAL_I2C
#include "I2C.h"

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Queue.h"
#include "Utilities.h"
#include "InterruptType.h"
#include "UART.h"
#include "HoldingBuffer.h"

// LTC2942 REGISTER ADDRESSES
#define CC_CONTROL_R 0b00000001

// LTC2942 REGISTER VALUES TO SET
#define CC_CONTROL_DEFAULT_V 0x3C


static interruptType Data = {I2C,NUL};
volatile int readback;
volatile int interruptVector;
volatile int src;




void I2C0_Init(void)
{
    static int wait =0;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    I2CSlaveInit(I2C0_BASE, SLAVE_ADDR);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);


    wait=0;

    I2C0_SCSR_R = 0x01;

    I2CSlaveIntEnableEx(I2C0_BASE, I2C_SLAVE_INT_DATA);
    I2CSlaveIntEnableEx(I2C0_BASE, I2C_SLAVE_INT_STOP);
    I2CSlaveIntClearEx(I2C0_BASE,I2C_SLAVE_INT_STOP);
    wait =0;
    disable();
    wait=0;
    IntEnable(INT_I2C0);
}

void processI2C(char* serializedI2C)
{
    i2cDeserialize received;

    received.input = serializedI2C;

    switch(received.command->registerToSet)
    {

    case CC_CONTROL_R:

        printString("CONTROL REGISTER\n\r");
        switch(received.command->value)
        {
        case CC_CONTROL_DEFAULT_V:
            printString("DEFAULT\n\r");
            break;
        default:
            printString("INVALID LTC2942 R VALUE\n\r");
        }
        break;
     default:
         printString("INVALID LTC2942 REGISTER ADDRESSS\n\r");
    }
}

void I2C0_IntHandler(void)
{

        switch(I2CSlaveIntStatusEx(I2C0_BASE,true)) //Returns integer according to the interrupt triggered
        {
        case I2C_SLAVE_INT_DATA:
            if(I2C0_SCSR_R & I2C_SCSR_RREQ)
            {
                Data.data = I2C0_SDR_R;
                add(Data.data);
            }
            else
            {
                if(dequeue(OUTPUT_I2C, &Data))
                {
                    I2C0_SDR_R = Data.data;
                }
            }
            I2CSlaveIntClear(I2C0_BASE);
        break;
        case I2C_SLAVE_INT_STOP:
            enqueue(INPUT, Data);
            I2CSlaveIntClearEx(I2C0_BASE,I2C_SLAVE_INT_STOP);
        default:
        break;
        }

}


//interruptVector = I2C0_SMIS_R;
//    src = I2C0_SCSR_R;
//
//    if(interruptVector & I2C_SMIS_DATAMIS)
//    {
//        I2C0_SICR_R |= I2C_SICR_DATAIC;
//
//        readback = I2C0_SICR_R;
//
//        if(src & I2C_SCSR_RREQ)
//        {
//            Data.data = (char)I2C0_SDR_R;
////            Data.data = I2CSlaveDataGet(INT_I2C0);
//            add(Data.data);
//        }
//        else if(dequeue(OUTPUT_I2C, &Data))
//        {
//            I2C0_SDR_R = Data.data;
//        }
//
//    }
//    else if(interruptVector & I2C_SMIS_STOPMIS)
//    {
//        I2C0_SICR_R |= I2C_SICR_STOPIC;
//        readback = I2C0_SICR_R;
//
//        enqueue(INPUT, Data);
//
//    }



