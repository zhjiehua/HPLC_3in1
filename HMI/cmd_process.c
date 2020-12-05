#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "misc.h"

#include "../User/CPrintf.h"
#include "../UILogic/pageCommon.h"
#include "../Logic/management.h"
#include "miscellaneous.h"
#include "24cxx.h"

#include "device_driver.h"

#ifdef __cplusplus
extern "C" {
#endif


	
/*! 
 *  \brief  消息处理流程，此处一般不需要更改
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//指令类型
	uint8 ctrl_msg = msg->ctrl_msg;   //消息的类型
	uint8 control_type = msg->control_type;//控件类型
	uint16 screen_id = PTR2U16(&msg->screen_id_high);//画面ID
	uint16 control_id = PTR2U16(&msg->control_id_high);//控件ID
	uint32 value = PTR2U32(msg->param);//数值
    uint32 value_u16 = PTR2U16(msg->param);

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://触摸屏按下
	case NOTIFY_TOUCH_RELEASE://触摸屏松开
		//NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://写FLASH成功
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://写FLASH失败
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://读取FLASH成功
		//NotifyReadFlash(1,cmd_buffer+2,size-6);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD://读取FLASH失败
		//NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_DISTEXT://文本显示
		
		break;
	case NOTIFY_READ_RTC://读取RTC时间
		//NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7]);
		NotifyReadRTC(hmi_cmd_buffer[2],hmi_cmd_buffer[3],hmi_cmd_buffer[4],hmi_cmd_buffer[5],hmi_cmd_buffer[6],hmi_cmd_buffer[7],hmi_cmd_buffer[8]);
	
		//转发给检测器
		MsgTransmit(1, msg, size);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					//cDebug("NotifyButton(), %d, %d, %d\n", screen_id, control_id, msg->param[1]);
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://文本控件
					//cDebug("NotifyText(), %d, %d, %s\n", screen_id, control_id, msg->param);
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //进度条控件
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //滑动条控件
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //仪表控件
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://菜单控件
					//cDebug("NotifyMenu(), %d, %d, %d, %d\n", screen_id, control_id, msg->param[0], msg->param[1]);
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://选择控件
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://倒计时控件
					NotifyTimer(screen_id,control_id);
					break;
        case kCtrlRecord://数据记录
					NotifyRecord(screen_id, control_id, value_u16);
					break;
				default:
					break;
				}
			}			
		}
		break;
	case NOTIFY_RESET:
		cDebug("NOTIFY_RESET\n");
		//NotifyReset();
    
        man.lcdBackLightFlag = 1;

        ReadRTC();
    
		break;
	case NOTIFY_ONLINE:
		cDebug("NOTIFY_ONLINE\n");
		
		man.lcdBackLightFlag = 1;
		ReadRTC();
		//man.lcdReady = 1;

		if(!man.lcdReady)
				SetScreen(LOGOPAGE_INDEX);

		break;
	case NOTIFY_UNLOCK:
//		SetTouchPaneOption(1, pProjectMan->beeperEnable, 1, 1);
//		LockDeviceConfig();
		break;
	case MSG_BACKLIGHT:
		man.lcdBackLightFlag = ctrl_msg;
		//pProjectMan->lcdBackLightFlag = 1;
		cDebug("MSG_BACKLIGHT pProjectMan->lcdBackLightFlag = %d\r\n", man.lcdBackLightFlag);
	break;
	
	//用户数据
	case MSG_USERUART_DATA: 
		//User_Data_Parse((uint8*)(&msg->ctrl_msg), size-6);
		break;
	
	default:
		break;
	}
}

//文本控件显示整数值
void SetTextValueInt32(uint16 screen_id, uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%d",value);
	SetTextValue(screen_id,control_id,(uchar *)buffer);
}

//字符串转整数
int32 StringToInt32(uint8 *str)
{
	int32 v = 0;
	sscanf((const char *)str,"%d",&v);
	return v;
}

//文本控件显示浮点数
void SetTextValueFloat(uint16 screen_id, uint16 control_id, float value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%.1f",value);
	SetTextValue(screen_id,control_id,(uchar *)buffer);
}

//字符串转整数
float StringToFloat(uint8 *str)
{
	double v = 0;
	sscanf((const char *)str,"%lf",&v);
	return v;
}

/*! 
 *  \brief  触摸屏复位通知
 *  \details  触摸屏上电复位或者被动复位后，执行此函数
 */
void NotifyReset(void)
{
	//TODO: 添加用户代码
	SetHandShake();//发送握手命令
}

/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16 screen_id)
{
	//TODO: 添加用户代码
    
//    man.curPage = screen_id;
//                
//    if(man.curPage == FACTORYMODEPAGE_INDEX)
//        man.factoryMode = 1;
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8 press,uint16 x,uint16 y)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8 state)
{
	//TODO: 添加用户代码
	switch(screen_id)
	{
		case LOGOPAGE_INDEX:
			//man.homeFlag = 0;//跳过开机回零点
			Detector_SetButtonValue(LOGOPAGE_INDEX, DEVICE_LOGOPAGE_SKIP_BUTTON, state);
		break;
		case RUNPAGE_INDEX:
			RunPageButtonProcess(control_id, state);
		break;
//		case SETUPPAGE_INDEX:
//			SetupPageButtonProcess(control_id, state);
//		break;
//		case MENUPAGE_INDEX:
//			MenuPageButtonProcess(control_id, state);
//		break;
//		case SCANPAGE_INDEX:
//			ScanPageButtonProcess(control_id, state);
//		break;
//		case SYSSETUPPAGE_INDEX:
//			SysSetupPageButtonProcess(control_id, state);
//		break;
//		case CALIBPAGE_INDEX:
//			CalibPageButtonProcess(control_id, state);
//		break;
//		case DEVICEINFOPAGE_INDEX:
//			//DeviceInfoPageButtonProcess(control_id, state);
//		break;
		case ABOUTPAGE_INDEX:
			AboutPageButtonProcess(control_id, state);
		break;
//		case HELPPAGE_INDEX:
//			//HelpPageButtonProcess(control_id, state);
//		break;
//		case SCANCURVEPAGE_INDEX:
//			//ScanCurvePageButtonProcess(control_id, state);
//		break;
//    case WAVECALIBPAGE_INDEX:
//			WaveCalibPageButtonProcess(control_id, state);
//		break;
//		case SUPERPASSWORDPAGE_INDEX:
//				SuperPasswordPageButtonProcess(control_id, state);
//		break;
//		case TIPS2PAGE_INDEX:
//				//Tips2PageButtonProcess(control_id, state);
//		break;
//		case TIPS1PAGE_INDEX:
//				//Tips1PageButtonProcess(control_id, state);
//		break;
//		case GLPPAGE_INDEX:
//				GLPPageButtonProcess(control_id, state);
//		break;
//		
//		case FACTORYMODEPAGE_INDEX:
//			//FactoryPageButtonProcess(control_id, state);
//		break;
		default:
			cDebug("cmd_process NotifyButton error!\n");
		break;
	}
}

/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
	//TODO: 添加用户代码
	switch(screen_id)
	{
		case RUNPAGE_INDEX:
			RunPageEditProcess(control_id, str);
		break;
//		case SETUPPAGE_INDEX:
//			SetupPageEditProcess(control_id, str);
//		break;
//		case SCANPAGE_INDEX:
//			ScanPageEditProcess(control_id, str);
//		break;
//		case SYSSETUPPAGE_INDEX:
//			SysSetupPageEditProcess(control_id, str);
//		break;
//		case CALIBPAGE_INDEX:
//			CalibPageEditProcess(control_id, str);
//		break;
//    case WAVECALIBPAGE_INDEX:
//			WaveCalibPageEditProcess(control_id, str);
//		break;
//		case SUPERPASSWORDPAGE_INDEX:
//				SuperPasswordPageEditProcess(control_id, str);            
//		break;
//		case ABOUTPAGE_INDEX:
//				AboutPageEditProcess(control_id, str);
//		break;
		default:
			cDebug("cmd_process NotifyText error!\n");
		break;
	}
}

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8  item, uint8  state)
{
	//TODO: 添加用户代码
	if(state == 1)
		return;

	switch(screen_id)
	{
//		case SETUPPAGE_INDEX:
//				SetupPageMenuProcess(control_id, item, state);
//		break;
//		case CALIBPAGE_INDEX:
//				CalibPageMenuProcess(control_id, item, state);
//		break;
//		case SYSSETUPPAGE_INDEX:
//				SysSetupPageMenuProcess(control_id, item, state);
//		break;
		default:
			cDebug("cmd_process NotifyMenu error!\n");
		break;
	}
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
	//TODO: 添加用户代码
	switch(screen_id)
	{
		default:
		break;
	}
}

/*! 
 *  \brief  数据记录通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyRecord(uint16 screen_id, uint16 control_id, uint16 value)
{
	//TODO: 添加用户代码
	switch(screen_id)
	{
//		case WAVECALIBPAGE_INDEX:
//			WaveCalibPageRecordProcess(control_id, value);
//		break;
		default:
		break;
	}
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8 status)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
 */
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second)
{
    //TODO: 添加用户代码
	sprintf((char*)man.strLcd, "20%02d/%02d/%02d %02d:%02d:%02d",
		BCDToInt(year), BCDToInt(month), BCDToInt(day),
		BCDToInt(hour), BCDToInt(minute), BCDToInt(second));	
    cDebug("rtc is %s\r\n", man.strLcd);
	
    man.lcdDate.year = BCDToInt(year);
    man.lcdDate.month = BCDToInt(month);
    man.lcdDate.day = BCDToInt(day);

    //测试用
    //man.lcdDate.day += 2;
    
    man.lcdReady = 1;
    
//    if(man.isValidatedActiveCode)
//    {
//        man.isValidatedActiveCode = 0;
//        AT24CXX_Write(EEPROM_ADDR_DATE, (uint8_t*)(&man.lcdDate), 3*4);//保存当前的日期
//    }
}

#ifdef __cplusplus
}
#endif
