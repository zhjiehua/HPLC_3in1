/*! 
 *  \file cmd_queue.h
 *  \brief ����ָ�����
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright ���ݴ�ʹ��Ƽ����޹�˾
 */

#ifndef _CMD_QUEUE_H_
#define _CMD_QUEUE_H_

#include "Queue.h"
#include "stdint.h"

/*! 
 *  \brief  ��ָ�������ȡ��һ��������ָ��
 *  \param  cmd ָ����ջ�����
 *  \param  buf_len ָ����ջ�������С
 *  \return  ָ��ȣ�0��ʾ������������ָ��
 */
qsize queue_find_cmd(QUEUE *que, qdata *cmd,qsize buf_len);

//=================�û�����======================

#define HMI_QUEUE_MAX_SIZE 1024 //128  /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/
#define DEVICE_QUEUE_MAX_SIZE 1024 //128  /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

extern qdata hmi_que_buffer[];
extern qdata detector_que_buffer[];
extern qdata pumpa_que_buffer[];
extern qdata pumpb_que_buffer[];

extern QUEUE hmi_que;
extern QUEUE detector_que;
extern QUEUE pumpa_que;
extern QUEUE pumpb_que;

#define HMI_CMD_MAX_SIZE 100 //200        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define DEVICE_CMD_MAX_SIZE 100 //64        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
	
extern uint8_t hmi_cmd_buffer[];
extern uint8_t detector_cmd_buffer[];		
extern uint8_t pumpa_cmd_buffer[];	
extern uint8_t pumpb_cmd_buffer[];	

#endif
