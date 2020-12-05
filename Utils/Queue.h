/*! 
 *  \file cmd_queue.h
 *  \brief 串口指令队列
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright 广州大彩光电科技有限公司
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Types.h"

typedef struct _QUEUE
{
	qsize _head; //队列头
	qsize _tail;  //队列尾
	
	qsize _cmd_pos; //当前指令指针位置
	
	qsize _size; //队列长度
	qdata *_data;	//队列数据缓存区指针
}QUEUE;

/*! 
 *  \brief  清空指令数据
 */
void queue_init(QUEUE *que, qdata *buffer, qsize size);

/*! 
 * \brief  添加指令数据
 * \detial 串口接收的数据，通过此函数放入指令队列 
 *  \param  _data 指令数据
 */
void queue_push(QUEUE *que, qdata _data);

qsize queue_size(QUEUE *que);

void queue_pop(QUEUE *que, qdata* _data);

#endif
