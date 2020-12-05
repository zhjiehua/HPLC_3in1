/************************************版权申明********************************************
**                             广州大彩光电科技有限公司
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   cmd_queue.c
** 修改时间:   2018-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
使用必读
cmd_queue.c中共5个函数：清空指令数据queue_reset()、从串口添加指令数据queue_push()、
从队列中取一个数据queue_pop().获取队列中有效数据个数queue_size()、从指令队列中取出一条完整的指令queue_find_cmd（）
若移植到其他平台，需要修改底层寄存器设置,但禁止修改函数名称，否则无法与HMI驱动库(hmi_driver.c)匹配。
--------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------*/
#include "Queue.h"
#include "ComQueue.h"
#include "miscellaneous.h"
#include "CPrintf.h"
#include "ComProcess.h"


qdata comQueueBuffer[COM_QUEUE_MAX_SIZE];
QUEUE comQueue; 

uint8_t comCmdBufferHex[COM_CMD_MAX_SIZE/2];                                           //hex指令缓存
uint8_t comCmdBufferAscii[COM_CMD_MAX_SIZE];                                              //ascii指令缓存

/*! 
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
qsize ComQueue_FindCmd(qdata *ascii_buffer, qdata *hex_buffer, qsize buf_len, QUEUE *que)
{
	qsize cmd_size = 0;
	qdata _data = 0;
	while(queue_size(que)>0)
	{
		//取一个数据
		queue_pop(que, &_data);
        
		if(_data == SYSTEM_COMMUNICATE_HEAD)
		{
			que->_cmd_pos = 0;
		}
	
		if(que->_cmd_pos == 0 && _data != SYSTEM_COMMUNICATE_HEAD) //指令第一个字节必须是帧头，否则跳过
		{
            Com_NAck();
			//cDebug("\r\n $ \r\n"); 
			continue;
		}

		if(que->_cmd_pos < buf_len) //防止缓冲区溢出
			ascii_buffer[que->_cmd_pos++] = _data;  //注意：++操作符比*操作符优先级更高
        
		if(_data == SYSTEM_COMMUNICATE_TAIL)
		{
			//char i;
			cmd_size = que->_cmd_pos/2-1 ;                              //指令字节长度

			if(que->_cmd_pos%2 == 0)
			{
                if(que->_cmd_pos < COM_CMD_MAX_SIZE-20)
                {
                    Ascii2Hex(hex_buffer, &ascii_buffer[1], cmd_size);
                    
//                    cDebug("The cmd is: ");
//                    for(i = 0; i < cmd_size; i++)
//                        cDebug("%02x ", hex_buffer[i]);
//                    cDebug("\r\n");
                }
                else //指令太长
				{
                    Com_NAck();
                    USER_ErrLog("The CMD is too long!"); 								  
                    que->_cmd_pos = 0; 
                    return 0;
                }
			}
			else //指令长度不是双数
			{
                Com_NAck();
				USER_ErrLog("The len of CMD is singular!"); 								  
				que->_cmd_pos = 0; 
				return 0;
			}					

			que->_cmd_pos = 0;                                              //复位指令指针
#if(CRC16_ENABLE)
			//去掉指令头尾，尾共计2个字节，只计算数据部分CRC
			if(!CheckCRC16(hex_buffer, cmd_size))                      //CRC校验
			{
                Com_NAck();
                USER_ErrLog("The CRC check is wrong!"); 
				return 0;
			}
			cmd_size -= 2;                                            //去掉CRC16（2字节）
#endif
			return cmd_size;
		}
	}
	return 0;                                                         //没有形成完整的一帧
}
