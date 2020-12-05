#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "hmi_user_uart.h"
#include "cmd_process.h"
#include "pageCommon.h"
#include "management.h"
#include "miscellaneous.h"
#include "CPrintf.h"
#include "math.h"
#include "IO.h"
#include "wdg.h"
#include "DeviceUsart.h"

#define HPLC_TEST 0 // 0:��������   1::����������㣬������

int main(void)
{ 
    uint8_t loopCnt = 0;
    uint32_t count = 0;
        
    uint8_t homeStopFlag = 0;
	qsize size = 0;

    //SCB->VTOR = FLASH_BASE | 0x3000; /* Vector Table Relocation in Internal FLASH. */
    //IWDG_Init(IWDG_Prescaler_128,625);    //���Ƶ��Ϊ128,����ֵΪ625,���ʱ��Ϊ2s
    IWDG_Feed();
  
    memset(&man, 0, sizeof(man));
    
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	
    cDebug("Hello HPLC Check!\r\n");
    
    queue_init(&hmi_que, hmi_que_buffer, HMI_QUEUE_MAX_SIZE);
		queue_init(&detector_que, detector_que_buffer, DEVICE_QUEUE_MAX_SIZE);
		queue_init(&pumpa_que, pumpa_que_buffer, DEVICE_QUEUE_MAX_SIZE);
		queue_init(&pumpb_que, pumpb_que_buffer, DEVICE_QUEUE_MAX_SIZE);
	
    HMIUart_Init(115200);
		detector_uart_init(115200);
		pumpa_uart_init(115200);
		pumpb_uart_init(115200);
		
//#if !HPLC_TEST
#if 1    
    cDebug("LCD_Check()...\r\n");
    //ResetDevice();
    SetHandShake();
    man.lcdReady = 0;
    while(!man.lcdReady)
    {
        size = queue_find_cmd(&hmi_que, hmi_cmd_buffer, HMI_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
        if(size > 0)  //���յ�ָ�� 
        {                                                                           
          ProcessMessage((PCTRL_MSG)hmi_cmd_buffer, size);                    //ָ��� 
        }
        
        if(count++ > 60000*10)
        {
            count = 0;
            //ResetDevice();
            SetHandShake();
            
            cDebug("-----RESET the LCD!\r\n");
        }
        
        IWDG_Feed();
    }
    cDebug("LCD_Check()...Finish\r\n");
#endif
    SetScreen(LOGOPAGE_INDEX);
		
//    delay_ms(1000);IWDG_Feed();
//    delay_ms(1000);IWDG_Feed();
//    delay_ms(1000);IWDG_Feed();
//    delay_ms(1000);IWDG_Feed();
    
    //DisText(100, 100, 0, 6, (uint8_t*)"Initializing... \r\n");   
		//SetTextValue(LOGOPAGE_INDEX, 4, (uint8_t*)"Initializing... \r\n");
		//SetTextValue(LOGOPAGE_INDEX, 5, (uint8_t*)"Lamp on...");
		
    //��ʼ����������Ҫ��EEPROM������
    InitMan();

    //��ʱ����AUֵ����λ��
    //TIM2_Int_Init(499, 7199); //10KHzʱ�ӣ���ʱ50ms������λ������Auֵ
    //TIM2_Int_Init(999, 7199); //100ms

		while(!man.detectorReady)
		{
			size = queue_find_cmd(&hmi_que, hmi_cmd_buffer, HMI_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��              
			if(size > 0)  //���յ�ָ��  
			{                                                                           
				ProcessMessage((PCTRL_MSG)hmi_cmd_buffer, size);                    //ָ��� 
			}
			
			size = queue_find_cmd(&detector_que, detector_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
			if(size > 0)  //���յ�ָ�� 
			{                                                                           
				Detector_ProcessMessage((PDEVICE_CTRL_MSG)detector_cmd_buffer, size);                    //ָ��� 
			}
			
			size = queue_find_cmd(&pumpa_que, pumpa_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
			if(size > 0)  //���յ�ָ�� 
			{                                                                           
				PumpA_ProcessMessage((PDEVICE_CTRL_MSG)pumpa_cmd_buffer, size);                    //ָ��� 
			}
			
			size = queue_find_cmd(&pumpb_que, pumpb_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
			if(size > 0)  //���յ�ָ�� 
			{                                                                           
				PumpB_ProcessMessage((PDEVICE_CTRL_MSG)pumpb_cmd_buffer, size);                    //ָ��� 
			}
		
			IWDG_Feed();
		}
		
		SetScreen(RUNPAGE_INDEX);
		
	while(1)
	{
    IWDG_Feed();

		size = queue_find_cmd(&hmi_que, hmi_cmd_buffer, HMI_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
		if(size > 0)  //���յ�ָ��  
		{                                                                           
			ProcessMessage((PCTRL_MSG)hmi_cmd_buffer, size);                    //ָ��� 
		}
		
		size = queue_find_cmd(&detector_que, detector_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
		if(size > 0)  //���յ�ָ�� 
		{                                                                           
			Detector_ProcessMessage((PDEVICE_CTRL_MSG)detector_cmd_buffer, size);                    //ָ��� 
		}
		
		size = queue_find_cmd(&pumpa_que, pumpa_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
		if(size > 0)  //���յ�ָ�� 
		{                                                                           
			PumpA_ProcessMessage((PDEVICE_CTRL_MSG)pumpa_cmd_buffer, size);                    //ָ��� 
		}
		
		size = queue_find_cmd(&pumpb_que, pumpb_cmd_buffer, DEVICE_CMD_MAX_SIZE);     //�ӻ������л�ȡһ��ָ��         
		if(size > 0)  //���յ�ָ�� 
		{                                                                           
			PumpB_ProcessMessage((PDEVICE_CTRL_MSG)pumpb_cmd_buffer, size);                    //ָ��� 
		}
	}
}
