/*! 
*  \file cmd_queue.h
*  \brief 串口指令队列
*  \version 1.0
*  \date 2012-2018
*  \copyright 广州大彩光电科技有限公司
*/

#ifndef _COM_QUEUE_H_
#define _COM_QUEUE_H_

#include "stdint.h"
#include "Queue.h"
#include "config.h"

typedef unsigned char qdata;
typedef unsigned short qsize;

/*! 
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
qsize ComQueue_FindCmd(qdata *ascii_buffer, qdata *hex_buffer, qsize buf_len, QUEUE *que);

extern qdata comQueueBuffer[];
extern QUEUE comQueue; 

extern uint8_t comCmdBufferHex[]; 
extern uint8_t comCmdBufferAscii[];

#endif
