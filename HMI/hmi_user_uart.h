#ifndef _HMI_USER_UART_H_
#define _HMI_USER_UART_H_

#include "Types.h"

/****************************************************************************
* 名    称： HMIUartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void HMIUart_Init(uint32 Baudrate);

/*****************************************************************
* 名    称： HMISendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
 *****************************************************************/
void HMISendChar(uchar t);


#endif
