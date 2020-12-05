/************************************版权申明********************************************
**                             广州大彩光电科技有限公司
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   hmi_user_uart.c
** 修改时间:   2011-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
                                  使用必读
   hmi_user_uart.c中的串口发送接收函数共3个函数：串口初始化HMIUartinit()、发送1个字节HMISendChar()、
   发送字符串SendStrings().若移植到其他平台，需要修改底层寄
   存器设置,但禁止修改函数名称，否则无法与HMI驱动库(hmi_driver.c)匹配。
--------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------
                          1. 基于STM32平台串口驱动
----------------------------------------------------------------------------------------*/

#include "usart.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"

#include "CPrintf.h"

/****************************************************************************
* 名    称： HMIUartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void HMIUart_Init(uint32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    cDebug("HMIUart_Init()...\r\n");
    
    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    
    USART_DeInit(UART5);                                           //复位USART1
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

    /* USART configuration */
    USART_Init(UART5, &USART_InitStructure);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //接收中断使能
    //USART_ITConfig(UART5, USART_IT_RXNE|USART_IT_ERR, ENABLE);//开启串口接受中断，开启错误中断
    //USART_ClearITPendingBit(UART5, USART_IT_RXNE);
  
    /* Enable USART */
    USART_Cmd(UART5, ENABLE);
    
    cDebug("HMIUart_Init()...Finish\r\n");
}

/*****************************************************************
* 名    称： HMISendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
 *****************************************************************/
void HMISendChar(uchar t)
{
	//while((UART5->SR&0X40)==0);
    USART_SendData(UART5, t);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET));//等待串口发送完毕
    
//    USART_SendData(USART3,t);
//    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//    while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

void UART5_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(UART5);//(USART3->DR);	//读取接收到的数据
	
		queue_push(&hmi_que, Res);
    }

//    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
//    {
//      USART_ClearITPendingBit(USART2, USART_IT_ORE);
//    }    
}
