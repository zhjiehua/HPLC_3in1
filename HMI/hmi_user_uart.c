/************************************��Ȩ����********************************************
**                             ���ݴ�ʹ��Ƽ����޹�˾
**                             http://www.gz-dc.com
**-----------------------------------�ļ���Ϣ--------------------------------------------
** �ļ�����:   hmi_user_uart.c
** �޸�ʱ��:   2011-05-18
** �ļ�˵��:   �û�MCU��������������
** ����֧�֣�  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
                                  ʹ�ñض�
   hmi_user_uart.c�еĴ��ڷ��ͽ��պ�����3�����������ڳ�ʼ��HMIUartinit()������1���ֽ�HMISendChar()��
   �����ַ���SendStrings().����ֲ������ƽ̨����Ҫ�޸ĵײ��
   ��������,����ֹ�޸ĺ������ƣ������޷���HMI������(hmi_driver.c)ƥ�䡣
--------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------
                          1. ����STM32ƽ̨��������
----------------------------------------------------------------------------------------*/

#include "usart.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"

#include "CPrintf.h"

/****************************************************************************
* ��    �ƣ� HMIUartInit()
* ��    �ܣ� ���ڳ�ʼ��
* ��ڲ����� ��
* ���ڲ����� ��
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
    
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    USART_DeInit(UART5);                                           //��λUSART1
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

    /* USART configuration */
    USART_Init(UART5, &USART_InitStructure);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //�����ж�ʹ��
    //USART_ITConfig(UART5, USART_IT_RXNE|USART_IT_ERR, ENABLE);//�������ڽ����жϣ����������ж�
    //USART_ClearITPendingBit(UART5, USART_IT_RXNE);
  
    /* Enable USART */
    USART_Cmd(UART5, ENABLE);
    
    cDebug("HMIUart_Init()...Finish\r\n");
}

/*****************************************************************
* ��    �ƣ� HMISendChar()
* ��    �ܣ� ����1���ֽ�
* ��ڲ����� t  ���͵��ֽ�
* ���ڲ����� ��
 *****************************************************************/
void HMISendChar(uchar t)
{
	//while((UART5->SR&0X40)==0);
    USART_SendData(UART5, t);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
    
//    USART_SendData(USART3,t);
//    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//    while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}

void UART5_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(UART5);//(USART3->DR);	//��ȡ���յ�������
	
		queue_push(&hmi_que, Res);
    }

//    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
//    {
//      USART_ClearITPendingBit(USART2, USART_IT_ORE);
//    }    
}
