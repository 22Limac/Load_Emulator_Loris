/*
 * @file    main.c
 * @brief   Entry point of the interrupt-driven monitor.
 * @author  Liam JA MacDonald
 * @date    20-Sep-2019 (Created)
 * @date    10-Oct-2019 (Modified)
 */
#include <string.h>
#include <ctype.h>
#include "Queue.h"
#include "UART.h"
#include "I2C.h"
#include "inc/hw_ints.h"
#include "HoldingBuffer.h"
#include "Utilities.h"


/*
 * @brief   Initializes interrupts, polls interrupt queue
 *          and handles them.
 * @detail  Defines  variable, initializes and enables
 *          interrupts, polls input queue. When interrupt
 *          occurs, its type and data is dequeued and passed
 *          through interruptType inData variable.
 *
 * @detail  UART (0) interrupt:
 *          the inData data character is by default added to the holding
 *          buffer, then enqueued into the output interrupt
 *          queue to be echoed to the terminal if the holding buffer
 *          isn't full.
 *          If the data character is ENTER (0x0d) the head of
 *          the holding buffer is passed to cmd pointer and passed
 *          to the process function in the CommandTable.c module.
 *          If the data character is BACKSPACE (0x08) the last
 *          character is removed from the holding buffer and the
 *          character is cleared from the terminal
 *
 * @detail  SYSTICK (1) interrupt:
 *          updateTenths is called in the Time.c module which
 *          increments the time.
 */
void main (void)
{
    interruptType inData ={NULL};
    char * cmd;
    /* Initialize UART */
    I2C0_Init();
    UART0_Init();           // Initialize UART0
    InterruptEnable(INT_VEC_UART0);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    enable();     // Enable Master (CPU) Interrupts


    while(1)
    {
        /* Poll input queue */
        while(!(dequeue(INPUT,&inData)));

            switch(inData.type)
            {
            case UART :

                switch(inData.data)
                {

                case BS:

                    if(remove()==SUCCESS){printString(BACKSPACE);}


                    break;
                default:
                    enqueue(OUTPUT_UART, inData);
                }
                break;
            case I2C:
                processI2C(empty());
                printString(NEWLINE);
                break;
            default:
                break;
            }

    }
}




