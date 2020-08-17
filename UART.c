/*
 * @file    UART.c
 * @details Contains initialization routines to set
 *          a UART interrupts for transmission and receive.
 *          Definition of the UART ISR
 * @author  Liam JA MacDonald
 * @date    23-Sep-2019 (created)
 * @date    10-Oct-2019 (modified)
 */

#define GLOBAL_UART
#include "UART.h"
#include "Queue.h"
#include "Utilities.h"
#include "InterruptType.h"

/*UART interrupt buffer */
static interruptType Data = {UART,NUL};

/*
 * @brief initialize UART0
 *        with BAUD-RATE:       115200
 *             Data Bits:       8
 *             Parity Bits:     0
 *             Stop Bits:       1
 */
void UART0_Init(void)
{
    volatile int wait;

    /* Initialize UART0 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCUART_GPIOA;   // Enable Clock Gating for UART0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCGPIO_UART0;   // Enable Clock Gating for PORTA
    wait = 0; // give time for the clocks to activate

    UART0_CTL_R &= ~UART_CTL_UARTEN;        // Disable the UART
    wait = 0;   // wait required before accessing the UART config regs

    // Setup the BAUD rate
    UART0_IBRD_R = 8;   // IBRD = int(16,000,000 / (16 * 115,200)) = 8.680555555555556
    UART0_FBRD_R = 44;  // FBRD = int(.680555555555556 * 64 + 0.5) = 44.05555555555556

    UART0_LCRH_R = (UART_LCRH_WLEN_8);  // WLEN: 8, no parity, one stop bit, without FIFOs)

    GPIO_PORTA_AFSEL_R = 0x3;        // Enable Receive and Transmit on PA1-0
    GPIO_PORTA_PCTL_R = (0x01) | ((0x01) << 4);         // Enable UART RX/TX pins on PA1-0
    GPIO_PORTA_DEN_R = EN_DIG_PA0 | EN_DIG_PA1;        // Enable Digital I/O on PA1-0

    UART0_CTL_R = UART_CTL_UARTEN;        // Enable the UART
    wait = 0; // wait; give UART time to enable itself.
}



/*
 * @brief   Enable UART0 receive and transmit interrupts in UART0
 * @param   [in] unsigned long Flags:
 *          bit mask to specify conditions for interrupt
 */
void UART0_IntEnable(unsigned long flags)
{
    /* Set specified bits for interrupt */
    UART0_IM_R |= flags;
}

/*
 * @brief   Force character into the data register
 * @param   [in] char data: character to be put into
 *          data register
 */
void forceOutput(char data)
{
        while((UART0_FR_R & UART_FR_TXFF)!=FALSE);//wait until not busy
        UART0_DR_R = data;
}

/*
 * @brief   Handles receive and transmit interrupts
 * @detail  check if receive interrupt has been set
 *          if it has load enqueue it in the input queue
 *          if the the output queue isn't empty force
 *          next available data out
 */
void UART0_IntHandler(void)
{
/*
 * Simplified UART ISR - handles receive and xmit interrupts
 * Application signalled when data received
 */
    if(UART0_MIS_R & UART_INT_RX)
    {
        /* RECV done - clear interrupt and make char available to application */
        UART0_ICR_R |= UART_INT_RX;
        Data.data = UART0_DR_R;
        enqueue(INPUT,Data);
    }
    UART0_ICR_R |= UART_INT_TX;
    if(dequeue(OUTPUT_UART, &Data))
    {
        forceOutput(Data.data);
    }

}
