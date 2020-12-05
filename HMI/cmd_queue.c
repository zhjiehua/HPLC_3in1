#include "cmd_queue.h"
#include "hmi_user_uart.h"
#include "stdint.h"
#include "hmi_driver.h"

#ifdef __cplusplus
extern "C" {
#endif 
    
//队列实际缓冲区
qdata hmi_que_buffer[HMI_QUEUE_MAX_SIZE];
qdata detector_que_buffer[DEVICE_QUEUE_MAX_SIZE];
qdata pumpa_que_buffer[DEVICE_QUEUE_MAX_SIZE];
qdata pumpb_que_buffer[DEVICE_QUEUE_MAX_SIZE];

//指令队列	
QUEUE hmi_que;  
QUEUE detector_que;
QUEUE pumpa_que;
QUEUE pumpb_que;

uint8_t hmi_cmd_buffer[HMI_CMD_MAX_SIZE];
uint8_t detector_cmd_buffer[DEVICE_CMD_MAX_SIZE];
uint8_t pumpa_cmd_buffer[DEVICE_CMD_MAX_SIZE];
uint8_t pumpb_cmd_buffer[DEVICE_CMD_MAX_SIZE];

static qsize hmi_queue_find_cmd(QUEUE *que, qdata *buffer, qsize buf_len)
{
	static uint32_t cmd_state = 0;
	qsize cmd_size = 0;
	qdata _data = 0;
	while(queue_size(que)>0)
	{
		//取一个数据
		queue_pop(que, &_data);

		if(que->_cmd_pos==0&&_data!=HMI_CMD_HEAD)//指令第一个字节必须是帧头，否则跳过
			continue;

		if(que->_cmd_pos<buf_len)//防止缓冲区溢出
			buffer[que->_cmd_pos++] = _data;

		cmd_state = ((cmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数

		//最后4个字节与帧尾匹配，得到完整帧
		if(cmd_state==HMI_CMD_TAIL)
		{
			cmd_size = que->_cmd_pos; //指令字节长度
			cmd_state = 0;  //重新检测帧尾巴
			que->_cmd_pos = 0; //复位指令指针

#if(HMI_CRC16_ENABLE)
			//去掉指令头尾EE，尾FFFCFFFF共计5个字节，只计算数据部分CRC
			if(!CheckCRC16(buffer+1,cmd_size-5))//CRC校验
				return 0;

			cmd_size -= 2;//去掉CRC16（2字节）
#endif

			return cmd_size;
		}
	}
	return 0; //没有形成完整的一帧
}

qsize queue_find_cmd(QUEUE *que, qdata *buffer, qsize buf_len)
{
	if(que == &hmi_que)
	{
	return hmi_queue_find_cmd(que, buffer, buf_len);
	}
	else if(que == &detector_que)//
	{
	return hmi_queue_find_cmd(que, buffer, buf_len);
	}
	else if(que == &pumpa_que)//
	{
	return hmi_queue_find_cmd(que, buffer, buf_len);
	}
	else if(que == &pumpb_que)//
	{
	return hmi_queue_find_cmd(que, buffer, buf_len);
	}
	else 
	{
		return 0;
	}
}

#ifdef __cplusplus
}
#endif
