#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "misc.h"

#include "../User/CPrintf.h"
#include "../UILogic/pageCommon.h"

#include "device_driver.h"

void Detector_ProcessMessage( PDEVICE_CTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//指令类型

	uint8 control_type = msg->control_type;//控件类型
	
	uint16 screen_id = PTR2U16(&msg->screen_id_high);//画面ID
	uint16 control_id = PTR2U16(&msg->control_id_high);//控件ID
	uint32 value = PTR2U32(msg->param);//数值
    uint32 value_u16 = PTR2U16(msg->param);
    
	uint8_t *p = (uint8_t*)msg;
	p[size-4] = '\0';
	//cDebug("cmd_type = %d\r\n", cmd_type);
	
	switch(cmd_type)
	{
//	case NOTIFY_DISTEXT://文本显示
//	{
//		DisText(PTR2U16(p+3), PTR2U16(p+5), p[7], p[8], p+9);
//	}
//	break;
	case NOTIFY_CONTROL:
		{
			if(control_type == 0x00)
			{
				man.detectorReady = 1;
			}	
			else if(control_type == 0x10)
			{
				if(screen_id == RUNPAGE_INDEX)
				{
					switch(control_id)
					{
						case DETECTOR_RUNPAGE_WAVE_EDIT: //
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_WAVE_EDIT, msg->param);
						break;
						case DETECTOR_RUNPAGE_AU_EDIT: //
              SetTextValue(RUNPAGE_INDEX, RUNPAGE_AU_EDIT, msg->param);
						break;
						case DETECTOR_RUNPAGE_LAMP_BUTTON: //
              SetButtonValue(RUNPAGE_INDEX, RUNPAGE_LAMP_BUTTON, msg->param[0]);
						break;
						case DETECTOR_RUNPAGE_CLEAR_BUTTON: //
						break;
						case DETECTOR_RUNPAGE_STATUS_EDIT:
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_STATUS_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
				else if(screen_id == DEVICEINFOPAGE_INDEX)
				{
					switch(control_id)
					{
						case DETECTOR_DEVICEINFOPAGE_MODEL_EDIT: //
							SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_D_MODEL_EDIT, msg->param);
						break;
						case DETECTOR_DEVICEINFOPAGE_REF_EDIT: //
              SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_D_REF_EDIT, msg->param);
						break;
						case DETECTOR_DEVICEINFOPAGE_SAM_EDIT: //
              SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_D_SAM_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
				else if(screen_id == ABOUTPAGE_INDEX)
				{
					switch(control_id)
					{
						case DEVICE_ABOUTPAGE_SOFTVERSION_EDIT: //
							SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_D_SOFTVERSION_EDIT, msg->param);
						break;
						case DEVICE_ABOUTPAGE_HARDVERSION_EDIT: //
              SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_D_HARDVERSION_EDIT, msg->param);
						break;
						case DEVICE_ABOUTPAGE_PRODUCTDATA_EDIT: //
              SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_PRODUCTDATA_EDIT, msg->param);
						break;
						case DEVICE_ABOUTPAGE_SERIES_EDIT: //
							SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_SERIES_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
				else if(screen_id == LOGOPAGE_INDEX)
				{
					switch(control_id)
					{
						case DEVICE_LOGOPAGE_INIT_EDIT: //
							SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_INIT_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_LAMPON_EDIT: //
              SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_LAMPON_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_WAVECALIB_EDIT: //
              SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_WAVECALIB_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_STEP1_EDIT: //
							SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_STEP1_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_STEP2_EDIT: //
							SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_STEP2_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_STEP3_EDIT: //
							SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_STEP3_EDIT, msg->param);
						break;
						case DEVICE_LOGOPAGE_STEP4_EDIT: //
							SetTextValue(LOGOPAGE_INDEX, LOGOPAGE_STEP4_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void PumpA_ProcessMessage( PDEVICE_CTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//指令类型

	uint8 control_type = msg->control_type;//控件类型
	
	uint16 screen_id = PTR2U16(&msg->screen_id_high);//画面ID
	uint16 control_id = PTR2U16(&msg->control_id_high);//控件ID
	uint32 value = PTR2U32(msg->param);//数值
    uint32 value_u16 = PTR2U16(msg->param);
    
	uint8_t *p = (uint8_t*)msg;
	p[size-4] = '\0';
	//cDebug("cmd_type = %d\r\n", cmd_type);
	
	switch(cmd_type)
	{
//	case NOTIFY_DISTEXT://文本显示
//	{
//		DisText(PTR2U16(p+3), PTR2U16(p+5), p[7], p[8], p+9);
//		cDebug("size = %d\r\n", size);
//		//cDebug("DisText %d, %d, %d, %d, %s\r\n", PTR2U16(p), PTR2U16(p+2), p[4], p[5], p+6);
//	}
	case NOTIFY_CONTROL:
		{
			if(control_type == 0x00)
			{
				man.pumpaReady = 1;
			}	
			else if(control_type == 0x10)
			{
				if(screen_id == RUNPAGE_INDEX)
				{
					switch(control_id)
					{
						case PUMP_RUNPAGE_FLOW_EDIT: //
						{
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_FLOWA_EDIT, msg->param);
						}
						break;
						case PUMP_RUNPAGE_PRESS_EDIT: //
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_PRESS_EDIT, msg->param);
						
							PumpB_SetTextValue(RUNPAGE_INDEX, PUMP_RUNPAGE_PRESS_EDIT, msg->param);//往B泵转发压力
						break;
						case PUMP_RUNPAGE_STATUS_EDIT:
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_STATUS_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
				else if(screen_id == DEVICEINFOPAGE_INDEX)
				{
					switch(control_id)
					{
						case PUMP_DEVICEINFOPAGE_MODEL_EDIT: //
							SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_P_MODEL_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
				else if(screen_id == ABOUTPAGE_INDEX)
				{
					switch(control_id)
					{
						case DEVICE_ABOUTPAGE_SOFTVERSION_EDIT: //
							SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_P_SOFTVERSION_EDIT, msg->param);
						break;
						case DEVICE_ABOUTPAGE_HARDVERSION_EDIT: //
              SetTextValue(ABOUTPAGE_INDEX, ABOUTPAGE_P_HARDVERSION_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void PumpB_ProcessMessage( PDEVICE_CTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//指令类型

	uint8 control_type = msg->control_type;//控件类型
	
	uint16 screen_id = PTR2U16(&msg->screen_id_high);//画面ID
	uint16 control_id = PTR2U16(&msg->control_id_high);//控件ID
	uint32 value = PTR2U32(msg->param);//数值
    uint32 value_u16 = PTR2U16(msg->param);
    
	uint8_t *p = (uint8_t*)msg;
	p[size-4] = '\0';
	//cDebug("cmd_type = %d\r\n", cmd_type);
	
	switch(cmd_type)
	{
//	case NOTIFY_DISTEXT://文本显示
//		DisText(PTR2U16(p+3), PTR2U16(p+5), p[7], p[8], p+9);
//	break;
	case NOTIFY_CONTROL:
		{
			if(control_type == 0x00)
			{
				man.pumpbReady = 1;
			}	
			else if(control_type == 0x10)
			{
				if(screen_id == RUNPAGE_INDEX)
				{
					switch(control_id)
					{
						case PUMP_RUNPAGE_FLOW_EDIT: //
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_FLOWB_EDIT, msg->param);
						break;
						case PUMP_RUNPAGE_STATUS_EDIT:
							SetTextValue(RUNPAGE_INDEX, RUNPAGE_STATUS_EDIT, msg->param);
						break;
						default:
							break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}