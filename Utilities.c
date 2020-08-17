/*
 * @file    Utilities.c
 * @brief   Contains functions frequently used
 *          over several modules used to format
 *          convert values and printing multiple
 *          characters in varying formats
 * @author  Liam JA MacDonald
 * @date    23-Sep-2019 (created)
 * @date    10-Oct-2019 (modified)
 */
#define UTILITIES
#include "Utilities.h"
#include "UART.h"
#include "Queue.h"
#include "InterruptType.h"
#include    <string.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    <ctype.h>

/*
 * @brief   Enable UART0 to interrupt
 * @param   [in] unsigned long InterruptIndex:
 *          UART0 address in interrupt table
 */
void InterruptEnable(unsigned long InterruptIndex)
{
/* Indicate to CPU which device is to interrupt */
if(InterruptIndex < 32)
    NVIC_EN0_R |= 1 << InterruptIndex;       // Enable the interrupt in the EN0 Register
else
    NVIC_EN1_R |= 1 << (InterruptIndex - 32);    // Enable the interrupt in the EN1 Register
}
/*
 * @brief   Enqueues a complete string onto the OUTPUT
 *          queue
 * @param   [in] char* string: string to be queued
 * @details enqueues one character at a time until
 *          nul character
 */
void printString(char* string)
{
    interruptType out ={UART, NUL};
    while(*string)
    {
        out.data =*(string++);
        enqueue(OUTPUT_UART,out);
    }
}



/*
 * @brief   converts ascii to decimal value
 * @param   [in] char* str: string that will be converted from ascii
 *          [out] int* num: integer passed reference to store decimal value
 * @return  int return used as a boolean value,
 *          if returns 1 string was successfully converted
 *          if returns 0 characters were not digits or the string
 *          too long
 */
int myAtoi(int * num, char* str)
{
    int total = 0;

    while(*str)
    {
        if((*str>='0')&&(*str<='9')&&(strlen(str)<TIME_STRING))
        {
          total = total*10+(*(str++)-'0') ;
        }
        else
        {return FAILURE;}
    }
    *num = total;
    return SUCCESS;
}

