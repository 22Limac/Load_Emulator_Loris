/*
 * @file    Queue.c
 * @brief   Defines interrupt queues
 *          Contains enqueue and dequeue functionality
 * @author  Liam JA MacDonald
 * @date    23-Sep-2019 (Created)
 * @date    10-Oct-2019 (Modified)
 */
#include <string.h>
#define GLOBAL_QUEUES
#include "Queue.h"
#include "Utilities.h"
#include "UART.h"

#define NUMBER_OF_QUEUES 3
/*Declare queue array for INPUT (0) and OUTPUT (1) interrupts*/
static queue interruptQueues[NUMBER_OF_QUEUES] =
    {{{RESET},RESET,RESET},{{RESET},RESET,RESET},{{RESET},RESET,RESET}};

/*
 * @brief   Adds an interrupt to an interrupt queue.
 * @param   [in] int queueType: specifies which queue to add to
 *                              INPUT (0); OUTPUT(1)
 *          [in] interruptType it: incoming interrupt
 * @return  int return Used as a boolean value,
 *          if returns 1; interrupt successfully queued.
 *          if returns 0; queue full or empty.
 * @detail  if it's for OUTPUT queue and it's empty
 *          the data is forced out to the screen
 *          interrupts are disabled before changing the
 *          write pointer to avoid race conditions
 */
int enqueue(int queueType, interruptType it)
{

    if((queueType==OUTPUT_UART)&&(interruptQueues[queueType].writePtr == interruptQueues[queueType].readPtr))//buffer is empty
    {
           forceOutput(it.data);
           return EMPTY;
    }
    disable();
    /* gives circular queue functionality*/
    unsigned int tmpPtr = (interruptQueues[queueType].writePtr+1)&(MAX_QUEUE_SIZE-1);
    if((tmpPtr == interruptQueues[queueType].readPtr)){return FULL;}
    /* put character in queue and increment write ptr */
    interruptQueues[queueType].fifo[interruptQueues[queueType].writePtr].type= it.type;
    interruptQueues[queueType].fifo[interruptQueues[queueType].writePtr].data= it.data;

    interruptQueues[queueType].writePtr =tmpPtr;
    enable();

    return SUCCESS;
}

/*
 * @brief   Removes an interrupt to an interrupt queue.
 * @param   [in] int queueType: specifies which queue to add to
 *                              INPUT (0); OUTPUT(1)
 *          [in] interruptType it: incoming interrupt
 * @return  int return Used as a boolean value,
 *          if returns 1; interrupt successfully queued.
 *          if returns 0; queue full or empty.
 * @detail  if it's for OUTPUT queue and it's empty
 *          the data is forced out to the screen
 *          interrupts are disabled before changing the
 *          write pointer to avoid race conditions
 */
int dequeue(int queueType,interruptType* it)
{
    if((interruptQueues[queueType].writePtr == interruptQueues[queueType].readPtr))//buffer is empty
    {
        return EMPTY;
    }

    it->data = interruptQueues[queueType].fifo[interruptQueues[queueType].readPtr].data;
    it->type = interruptQueues[queueType].fifo[interruptQueues[queueType].readPtr].type;
    /* gives circular queue functionality*/
    interruptQueues[queueType].readPtr=(interruptQueues[queueType].readPtr+1)&(MAX_QUEUE_SIZE-1);

    return SUCCESS;
}



