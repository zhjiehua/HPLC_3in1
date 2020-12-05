#include "device_driver.h"
#include "stdio.h"
#include "DeviceUsart.h"
#include "cmd_process.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================
//=====================================DEVICE=================================================
//===========================================================================================
void MsgTransmit(uint8_t device, void *msg, uint16 size)
{
	uint8_t i;
	uint8_t *p = (uint8_t*)msg;
	
	if(device == 0)//HMI
	{
		for(i=0;i<size;i++)
		{
			HMISendChar(p[i]);
		}
	}
	else if(device == 1)//Detector
	{
		for(i=0;i<size;i++)
		{
			DetectorSendChar(p[i]);
		}
	}
	
}

//===========================================================================================
//=====================================DETECTOR=================================================
//===========================================================================================
void DetectorSendChar(uchar t)
{
    USART_SendData(USART2, t);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

#define Detector_TX_8(P1) Detector_SEND_DATA((P1)&0xFF)  //发送单个字节
#define Detector_TX_16(P1) Detector_TX_8((P1)>>8);Detector_TX_8(P1)  //发送16位整数
#define Detector_TX_32(P1) Detector_TX_16((P1)>>16);Detector_TX_16((P1)&0xFFFF)  //发送32位整数

#define Detector_SEND_DATA(P) DetectorSendChar(P)
#define Detector_BEGIN_CMD() Detector_TX_8(0XEE)
#define Detector_END_CMD() Detector_TX_32(0XFFFCFFFF)

static void Detector_SendStrings(uchar *str)
{
	while(*str)
	{
		Detector_TX_8(*str);
		str++;
	}
}


void Detector_SetScreen(uint16 screen_id)
{
	Detector_BEGIN_CMD();
	Detector_TX_8(0xB1);
	Detector_TX_8(0x00);
	Detector_TX_16(screen_id);
	Detector_END_CMD();
}

void Detector_SetButtonValue(uint16 screen_id,uint16 control_id,uchar state)
{
	Detector_BEGIN_CMD();
	Detector_TX_8(0xB1);
	Detector_TX_8(0x10);
	Detector_TX_16(screen_id);
	Detector_TX_16(control_id);
	Detector_TX_8(kCtrlButton); //button类型
	Detector_TX_8(kCtrlButton); //插入一个字节，该字节不知有什么用
	Detector_TX_8(state);
	Detector_END_CMD();
}

void Detector_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str)
{
	Detector_BEGIN_CMD();
	Detector_TX_8(0xB1);
	Detector_TX_8(0x10);
	Detector_TX_16(screen_id);
	Detector_TX_16(control_id);
	Detector_TX_8(kCtrlText); //text类型
	Detector_SendStrings(str);
	Detector_END_CMD();
}



//===========================================================================================
//=====================================PUMPA=================================================
//===========================================================================================
void PumpASendChar(uchar t)
{
    USART_SendData(USART3, t);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

#define PumpA_TX_8(P1) PumpA_SEND_DATA((P1)&0xFF)  //发送单个字节
#define PumpA_TX_16(P1) PumpA_TX_8((P1)>>8);PumpA_TX_8(P1)  //发送16位整数
#define PumpA_TX_32(P1) PumpA_TX_16((P1)>>16);PumpA_TX_16((P1)&0xFFFF)  //发送32位整数

#define PumpA_SEND_DATA(P) PumpASendChar(P)
#define PumpA_BEGIN_CMD() PumpA_TX_8(0XEE)
#define PumpA_END_CMD() PumpA_TX_32(0XFFFCFFFF)

static void PumpA_SendStrings(uchar *str)
{
	while(*str)
	{
		PumpA_TX_8(*str);
		str++;
	}
}


void PumpA_SetScreen(uint16 screen_id)
{
	PumpA_BEGIN_CMD();
	PumpA_TX_8(0xB1);
	PumpA_TX_8(0x00);
	PumpA_TX_16(screen_id);
	PumpA_END_CMD();
}

void PumpA_SetButtonValue(uint16 screen_id,uint16 control_id,uchar state)
{
	PumpA_BEGIN_CMD();
	PumpA_TX_8(0xB1);
	PumpA_TX_8(0x10);
	PumpA_TX_16(screen_id);
	PumpA_TX_16(control_id);
	PumpA_TX_8(kCtrlButton); //button类型
	PumpA_TX_8(kCtrlButton); //插入一个字节，该字节不知有什么用
	PumpA_TX_8(state);
	PumpA_END_CMD();
}

void PumpA_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str)
{
	PumpA_BEGIN_CMD();
	PumpA_TX_8(0xB1);
	PumpA_TX_8(0x10);
	PumpA_TX_16(screen_id);
	PumpA_TX_16(control_id);
	PumpA_TX_8(kCtrlText); //text类型
	PumpA_SendStrings(str);
	PumpA_END_CMD();
}

//===========================================================================================
//=====================================PUMPB=================================================
//===========================================================================================
void PumpBSendChar(uchar t)
{
    USART_SendData(UART4, t);
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

#define PumpB_TX_8(P1) PumpB_SEND_DATA((P1)&0xFF)  //发送单个字节
#define PumpB_TX_16(P1) PumpB_TX_8((P1)>>8);PumpB_TX_8(P1)  //发送16位整数
#define PumpB_TX_32(P1) PumpB_TX_16((P1)>>16);PumpB_TX_16((P1)&0xFFFF)  //发送32位整数

#define PumpB_SEND_DATA(P) PumpBSendChar(P)
#define PumpB_BEGIN_CMD() PumpB_TX_8(0XEE)
#define PumpB_END_CMD() PumpB_TX_32(0XFFFCFFFF)

static void PumpB_SendStrings(uchar *str)
{
	while(*str)
	{
		PumpB_TX_8(*str);
		str++;
	}
}


void PumpB_SetScreen(uint16 screen_id)
{
	PumpB_BEGIN_CMD();
	PumpB_TX_8(0xB1);
	PumpB_TX_8(0x00);
	PumpB_TX_16(screen_id);
	PumpB_END_CMD();
}

void PumpB_SetButtonValue(uint16 screen_id,uint16 control_id,uchar state)
{
	PumpB_BEGIN_CMD();
	PumpB_TX_8(0xB1);
	PumpB_TX_8(0x10);
	PumpB_TX_16(screen_id);
	PumpB_TX_16(control_id);
	PumpB_TX_8(kCtrlButton); //button类型
	PumpB_TX_8(kCtrlButton); //插入一个字节，该字节不知有什么用
	PumpB_TX_8(state);
	PumpB_END_CMD();
}

void PumpB_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str)
{
	PumpB_BEGIN_CMD();
	PumpB_TX_8(0xB1);
	PumpB_TX_8(0x10);
	PumpB_TX_16(screen_id);
	PumpB_TX_16(control_id);
	PumpB_TX_8(kCtrlText); //text类型
	PumpB_SendStrings(str);
	PumpB_END_CMD();
}








#ifdef __cplusplus
}
#endif
