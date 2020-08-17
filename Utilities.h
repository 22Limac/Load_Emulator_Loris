/*
 * @file    Utilities.h
 * @brief   frequently used constants
 *          Utilities.c function prototypes
 * @author  Liam JA MacDonald
 * @date    23-Sep-2019 (created)
 * @date    10-Oct-2019 (modified)
 */
#pragma     once
#define     disable()   __asm(" cpsid i") //disable interrupts
#define     enable()    __asm(" cpsie i")//enable interrupts
#define     ENTER       0x0d //ASCII Characters
#define     BS          0x08
#define     NUL         0x00

#define     INPUT       0   //For different queues
#define     OUTPUT_UART     1
#define     OUTPUT_I2C      2

#define     TRUE        1   // For using Integers as booleans
#define     FALSE       0
#define     SUCCESS     1   // To clarify returns values where possible
#define     FAILURE     0
#define     RESET       0   //reset value for all date and time values other then days
#define     HOURS       0   //time array positions
#define     MINUTES     1
#define     SECONDS     2
#define     TENTHS      3
#define     PRECISION   4   //Time precision
#define     TIME_STRING     3   //time string length including nul
#define     TENTHS_LIMIT    10     //max set value
#define     SECONDS_MINUTES_LIMIT   60
#define     HOURS_LIMIT     24
#define     TWO_DIGITS      10                          //two digit number musn't be formatted
#define     FORBIDDEN       100
#define     OUTPUT_STRING   20 //for printing date and time strings, with padding to be safe
#define     EMPTY       0       //Queue return values
#define     FULL        0

#define     UART        0       //interrupt types
#define     I2C         1
#define     NEWLINE  "\n\r"
#define     COMMAND_PROMPT  "\n\r>"     //different prompts
#define     INVALID_PROMPT  "\n\r?"
#define     BACKSPACE       "\b \b"     //to replace the backspaced char with whitespace
            //check time or date value is within boundaries
#define     BOUNDARY_CHECK(X,Y) ((RESET<=X)&&(X<Y))
            /*if value exceeds limit, reset and call function*/
#define     UPDATE(VAL,LIM,FUNC) VAL = (VAL<LIM) ? VAL+1 : RESET;\
                                             if(!(VAL)){FUNC;}

#define NVIC_EN0_R      (*((volatile unsigned long *)0xE000E100))   // Interrupt 0-31 Set Enable Register
#define NVIC_EN1_R      (*((volatile unsigned long *)0xE000E104))   // Interrupt 32-54 Set Enable Register

#ifndef     UTILITIES
#define     UTILITIES


    extern void printChar(char);
    extern void printString(char*);
    extern void spaceFilter(char*);
    extern int myAtoi(int *, char*);
    extern int parseClock(char*,int*);
    extern void printTime(const int *);
    extern void InterruptEnable(unsigned long);

#else

    void printChar(char);
    void printString(char*);
    void spaceFilter(char*);
    int myAtoi(int *, char*);
    int parseClock(char*,int*);
    void formatTime(int, char*);
    void printTime(const int *);

#endif /* UTILITIES */
