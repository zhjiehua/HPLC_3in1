/*! 
 *  \file cmd_queue.h
 *  \brief 串口指令队列
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright 广州大彩光电科技有限公司
 */

#ifndef _CMD_QUEUE_H_
#define _CMD_QUEUE_H_

#include "Queue.h"
#include "stdint.h"

/*! 
 *  \brief  从指令队列中取出一条完整的指令
 *  \param  cmd 指令接收缓存区
 *  \param  buf_len 指令接收缓存区大小
 *  \return  指令长度，0表示队列中无完整指令
 */
qsize queue_find_cmd(QUEUE *que, qdata *cmd,qsize buf_len);

//=================用户数据======================

#define HMI_QUEUE_MAX_SIZE 1024 //128  /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/
#define DEVICE_QUEUE_MAX_SIZE 1024 //128  /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/

extern qdata hmi_que_buffer[];
extern qdata detector_que_buffer[];
extern qdata pumpa_que_buffer[];
extern qdata pumpb_que_buffer[];

extern QUEUE hmi_que;
extern QUEUE detector_que;
extern QUEUE pumpa_que;
extern QUEUE pumpb_que;

#define HMI_CMD_MAX_SIZE 100 //200        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define DEVICE_CMD_MAX_SIZE 100 //64        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
	
extern uint8_t hmi_cmd_buffer[];
extern uint8_t detector_cmd_buffer[];		
extern uint8_t pumpa_cmd_buffer[];	
extern uint8_t pumpb_cmd_buffer[];	

#endif
