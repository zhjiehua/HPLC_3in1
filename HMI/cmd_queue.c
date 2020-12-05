#include "cmd_queue.h"
#include "hmi_user_uart.h"
#include "stdint.h"
#include "hmi_driver.h"

#ifdef __cplusplus
extern "C" {
#endif 
    
//����ʵ�ʻ�����
qdata hmi_que_buffer[HMI_QUEUE_MAX_SIZE];
qdata detector_que_buffer[DEVICE_QUEUE_MAX_SIZE];
qdata pumpa_que_buffer[DEVICE_QUEUE_MAX_SIZE];
qdata pumpb_que_buffer[DEVICE_QUEUE_MAX_SIZE];

//ָ�����	
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
		//ȡһ������
		queue_pop(que, &_data);

		if(que->_cmd_pos==0&&_data!=HMI_CMD_HEAD)//ָ���һ���ֽڱ�����֡ͷ����������
			continue;

		if(que->_cmd_pos<buf_len)//��ֹ���������
			buffer[que->_cmd_pos++] = _data;

		cmd_state = ((cmd_state<<8)|_data);//ƴ�����4���ֽڣ����һ��32λ����

		//���4���ֽ���֡βƥ�䣬�õ�����֡
		if(cmd_state==HMI_CMD_TAIL)
		{
			cmd_size = que->_cmd_pos; //ָ���ֽڳ���
			cmd_state = 0;  //���¼��֡β��
			que->_cmd_pos = 0; //��λָ��ָ��

#if(HMI_CRC16_ENABLE)
			//ȥ��ָ��ͷβEE��βFFFCFFFF����5���ֽڣ�ֻ�������ݲ���CRC
			if(!CheckCRC16(buffer+1,cmd_size-5))//CRCУ��
				return 0;

			cmd_size -= 2;//ȥ��CRC16��2�ֽڣ�
#endif

			return cmd_size;
		}
	}
	return 0; //û���γ�������һ֡
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
