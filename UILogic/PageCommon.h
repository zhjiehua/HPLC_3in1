#ifndef _PAGE_COMMON_H_
#define _PAGE_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "sys.h"
#include "string.h"
#include "hmi_user_uart.h"
#include "management.h"
    
/************************************************************************/
/* 页面ID定义                                                           */
/************************************************************************/
#define LOGOPAGE_INDEX					0
#define CHECKINGPAGE_INDEX			    1
#define RUNPAGE_INDEX					2
#define SETUPPAGE_INDEX				    3
#define MENUPAGE_INDEX			        4
#define SCANPAGE_INDEX			        5
#define SYSSETUPPAGE_INDEX			    6
#define CALIBPAGE_INDEX			        7
#define DEVICEINFOPAGE_INDEX			8
#define ABOUTPAGE_INDEX			        9
#define HELPPAGE_INDEX			        10
#define SCANCURVEPAGE_INDEX			    11
#define WAVECALIBPAGE_INDEX			    12
#define SUPERPASSWORDPAGE_INDEX			13
#define TIPS2PAGE_INDEX			        14
#define TIPS1PAGE_INDEX			        15
#define TIPS0PAGE_INDEX			        16
#define GLPPAGE_INDEX			        17

#define FACTORYMODEPAGE_INDEX			18

#define PURGEPAGE_INDEX			    11

/************************************************************************/
/* LOGO页面控件ID                                                       */
/************************************************************************/
#define LOGOPAGE_ANIMATION					1
#define LOGOPAGE_SKIP_BUTTON				2
#define LOGOPAGE_INIT_EDIT					4
#define LOGOPAGE_LAMPON_EDIT				5
#define LOGOPAGE_WAVECALIB_EDIT			6
#define LOGOPAGE_STEP1_EDIT					7
#define LOGOPAGE_STEP2_EDIT					8
#define LOGOPAGE_STEP3_EDIT					9
#define LOGOPAGE_STEP4_EDIT					10

/************************************************************************/
/* 运行页面控件ID                                                         */
/************************************************************************/
#define RUNPAGE_WAVE_EDIT				1
#define RUNPAGE_WAVE2_EDIT				2
#define RUNPAGE_AU_EDIT				    3
#define RUNPAGE_AU2_EDIT				4
#define RUNPAGE_LAMP_BUTTON			    5
#define RUNPAGE_CLEAR_BUTTON			6

#define RUNPAGE_FLOWA_EDIT				7
#define RUNPAGE_PERCENTA_EDIT				8
#define RUNPAGE_FLOWB_EDIT				9
#define RUNPAGE_PERCENTB_EDIT				10
#define RUNPAGE_PRESS_EDIT				11
#define RUNPAGE_START_BUTTON			12
#define RUNPAGE_STOP_BUTTON				13
#define RUNPAGE_PURGEA_BUTTON			14
#define RUNPAGE_PURGEB_BUTTON			15

#define RUNPAGE_STATUS_EDIT 			40

#define RUNPAGE_ABOUT_BUTTON			30

#define RUNPAGE_WAVE2_TEXT				52
#define RUNPAGE_WAVE2UNIT_TEXT			53
#define RUNPAGE_AU2_TEXT				56
#define RUNPAGE_AU2UNIT_TEXT			57

/************************************************************************/
/* 设置页面控件ID                                                       */
/************************************************************************/
#define SETUPPAGE_RANGE_EDIT			1
#define SETUPPAGE_RANGE_BUTTON			2
#define SETUPPAGE_RANGE_MENU			3
#define SETUPPAGE_TIMECONST_EDIT		4
#define SETUPPAGE_TIMECONST_BUTTON		5
#define SETUPPAGE_TIMECONST_MENU		6
#define SETUPPAGE_CHANNEL_EDIT			7
#define SETUPPAGE_CHANNEL_BUTTON		8
#define SETUPPAGE_CHANNEL_MENU			9
#define SETUPPAGE_LAMPTYPE_EDIT			10
#define SETUPPAGE_LAMPTYPE_BUTTON		11
#define SETUPPAGE_LAMPTYPE_MENU			12

#define SETUPPAGE_RETURN_BUTTON			30

/************************************************************************/
/* 菜单页面控件ID                                                       */
/************************************************************************/
#define MENUPAGE_SCAN_BUTTON		    1
#define MENUPAGE_SYSSETUP_BUTTON		2
#define MENUPAGE_CALIB_BUTTON		    3
#define MENUPAGE_DEVICEINFO_BUTTON		4
#define MENUPAGE_ABOUT_BUTTON		    5
#define MENUPAGE_GLP_BUTTON		        6
#define MENUPAGE_HELP_BUTTON		    7

#define MENUPAGE_RETURN_BUTTON			30

/************************************************************************/
/* 扫描页面控件ID                                                       */
/************************************************************************/
#define SCANPAGE_WAVESTART_EDIT		    1
#define SCANPAGE_WAVEEND_EDIT   		2
#define SCANPAGE_SCANSPEED_EDIT		    3
#define SCANPAGE_START_BUTTON		    4
#define SCANPAGE_CURVE_BUTTON		    5

#define SCANPAGE_CURWAVE_EDIT	        7
#define SCANPAGE_CURAU_EDIT	            8

#define SCANPAGE_RETURN_BUTTON			30

/************************************************************************/
/* 系统设置页面控件ID                                                    */
/************************************************************************/
#define SYSSETUPPAGE_COMPORT_EDIT		1
#define SYSSETUPPAGE_COMPORT_BUTTON		3
#define SYSSETUPPAGE_COMPORT_MENU		2
#define SYSSETUPPAGE_ADDRESS_EDIT		4
#define SYSSETUPPAGE_LOCALIP1_EDIT		5
#define SYSSETUPPAGE_LOCALIP2_EDIT		6
#define SYSSETUPPAGE_LOCALIP3_EDIT		7
#define SYSSETUPPAGE_LOCALIP4_EDIT		8
#define SYSSETUPPAGE_LOCALPORT_EDIT		9
#define SYSSETUPPAGE_REMOTEIP1_EDIT		10
#define SYSSETUPPAGE_REMOTEIP2_EDIT		11
#define SYSSETUPPAGE_REMOTEIP3_EDIT		12
#define SYSSETUPPAGE_REMOTEIP4_EDIT		13
#define SYSSETUPPAGE_REMOTEPORT_EDIT	14
#define SYSSETUPPAGE_BRIGHTNESS_EDIT    15
#define SYSSETUPPAGE_ALARMSOUND_BUTTON  16
#define SYSSETUPPAGE_LANGUAGE_EDIT		17
#define SYSSETUPPAGE_LANGUAGE_BUTTON	18
#define SYSSETUPPAGE_LANGUAGE_MENU		19

#define SYSSETUPPAGE_RETURN_BUTTON		30

/************************************************************************/
/* 校正页面控件ID                                                       */
/************************************************************************/
#define CALIBPAGE_TIMEINTEGRAL_EDIT		1
#define CALIBPAGE_RANGEINTEGRAL_EDIT	2
#define CALIBPAGE_RANGEINTEGRAL_BUTTON	3
#define CALIBPAGE_RANGEINTEGRAL_MENU	4
#define CALIBPAGE_AUFACTOR_EDIT 		5
#define CALIBPAGE_WAVECALIB_BUTTON		6
#define CALIBPAGE_RESET_BUTTON		    7

#define CALIBPAGE_SUPERPASSWORD_BUTTON	8

#define CALIBPAGE_RETURN_BUTTON		    30

/************************************************************************/
/* 设备信息页面控件ID                                                       */
/************************************************************************/
#define DEVICEINFOPAGE_MODEL_EDIT		1
#define DEVICEINFOPAGE_REF_EDIT		    2
#define DEVICEINFOPAGE_SAM_EDIT	        3
#define DEVICEINFOPAGE_REF2_EDIT		4
#define DEVICEINFOPAGE_SAM2_EDIT	    5

#define DEVICEINFOPAGE_RETURN_BUTTON	30

#define DEVICEINFOPAGE_REF2_TEXT	    53
#define DEVICEINFOPAGE_SAM2_TEXT	    54

/************************************************************************/
/* 关于页面控件ID                                                       */
/************************************************************************/
#define ABOUTPAGE_D_SOFTVERSION_EDIT		1
#define ABOUTPAGE_D_HARDVERSION_EDIT		2
#define ABOUTPAGE_P_SOFTVERSION_EDIT		3
#define ABOUTPAGE_P_HARDVERSION_EDIT		4
#define ABOUTPAGE_SERIES_EDIT	        	5
#define ABOUTPAGE_PRODUCTDATA_EDIT	    6
#define ABOUTPAGE_D_MODEL_EDIT					7
#define ABOUTPAGE_P_MODEL_EDIT					8
#define ABOUTPAGE_D_REF_EDIT						9
#define ABOUTPAGE_D_SAM_EDIT						10

#define ABOUTPAGE_RETURN_BUTTON	        30

/************************************************************************/
/* 帮助页面控件ID                                                       */
/************************************************************************/
#define HELPPAGE_RETURN_BUTTON	        30

/************************************************************************/
/* 扫描曲线页面控件ID                                                    */
/************************************************************************/
#define SCANCURVEPAGE_CURVE_GRAPH	    1
#define SCANCURVEPAGE_CREST_EDIT	    2

#define SCANCURVEPAGE_RETURN_BUTTON	    30

/************************************************************************/
/* 波长校正页面控件ID                                                    */
/************************************************************************/
#define WAVECALIBPAGE_WAVECALIBTABLE_RECORD        1

#define WAVECALIBPAGE_WAVECALIBWAVE_EDIT           2
#define WAVECALIBPAGE_WAVECALIBREALWAVE_EDIT       3
#define WAVECALIBPAGE_WAVECALIBMODE_BUTTON         4

#define WAVECALIBPAGE_RETURN_BUTTON	               30

/************************************************************************/
/* 超级密码页面控件ID                                                    */
/************************************************************************/
#define SUPERPASSWORDPAGE_PASSWORD_EDIT    1
#define SUPERPASSWORDPAGE_STATUS_EDIT      2
#define SUPERPASSWORDPAGE_ENTER_BUTTON	   3
#define SUPERPASSWORDPAGE_RETURN_BUTTON	   4

/************************************************************************/
/* TIPS页面控件ID                                                        */
/************************************************************************/
#define TIPS2PAGE_TIPS_EDIT                 1
#define TIPS2PAGE_ENTER_BUTTON              3
#define TIPS2PAGE_CANCEL_BUTTON             4

#define TIPS1PAGE_TIPS_EDIT                 1
#define TIPS1PAGE_ENTER_BUTTON              3

#define TIPS0PAGE_TIPS_EDIT                 1

/************************************************************************/
/* GLP页面控件ID                                                        */
/************************************************************************/
#define GLPPAGE_DEVICEUSEDTIME_EDIT	        1
#define GLPPAGE_LAMPUSEDTIME_EDIT	        2
#define GLPPAGE_LAMPONOFFTIMES_EDIT	        3
#define GLPPAGE_LAMPCOUNT_EDIT	            4
#define GLPPAGE_LAMPCHANGED_BUTTON	        5

#define GLPPAGE_RETURN_BUTTON	            30

/************************************************************************/
/* 菜单选择控件使用                                                     */
/************************************************************************/
#define MENU_DISABLE					0x00
#define MENU_ENABLE						0x01

/************************************************************************/
/* 软件版本                                                             */
/************************************************************************/
#define PANEL_VERSION					"9.1.7"
#define VERSION							"0.1.0"

//#define cDebug printf

/*
 * 下拉列表字符串声明
 */

/*
 * 函数声明
 */

void RunPageButtonProcess(uint16 control_id, uint8  state);
void RunPageEditProcess(uint16 control_id, uint8 *str);

void SetupPageButtonProcess(uint16 control_id, uint8  state);
void SetupPageEditProcess(uint16 control_id, uint8 *str);
void SetupPageMenuProcess(uint16 control_id, uint8 item, uint8 state);

void MenuPageButtonProcess(uint16 control_id, uint8  state);

void ScanPageButtonProcess(uint16 control_id, uint8  state);
void ScanPageEditProcess(uint16 control_id, uint8 *str);

void SysSetupPageButtonProcess(uint16 control_id, uint8  state);
void SysSetupPageEditProcess(uint16 control_id, uint8 *str);
void SysSetupPageMenuProcess(uint16 control_id, uint8 item, uint8 state);

void CalibPageButtonProcess(uint16 control_id, uint8  state);
void CalibPageEditProcess(uint16 control_id, uint8 *str);
void CalibPageMenuProcess(uint16 control_id, uint8 item, uint8 state);

void DeviceInfoPageButtonProcess(uint16 control_id, uint8  state);

void AboutPageButtonProcess(uint16 control_id, uint8  state);
void AboutPageEditProcess(uint16 control_id, uint8 *str);

void HelpPageButtonProcess(uint16 control_id, uint8  state);

void ScanCurvePageButtonProcess(uint16 control_id, uint8  state);

void WaveCalibPageButtonProcess(uint16 control_id, uint8  state);
void WaveCalibPageEditProcess(uint16 control_id, uint8 *str);
void WaveCalibPageRecordProcess(uint16 control_id, uint16 value);

void SuperPasswordPageButtonProcess(uint16 control_id, uint8 state);
void SuperPasswordPageEditProcess(uint16 control_id, uint8 *str);

void Tips2PageButtonProcess(uint16 control_id, uint8 state);
void Tips1PageButtonProcess(uint16 control_id, uint8 state);

void GLPPageButtonProcess(uint16 control_id, uint8  state);

/************************************************************************/
/* LOGO页面控件ID                                                       */
/************************************************************************/
#define DEVICE_LOGOPAGE_ANIMATION					1
#define DEVICE_LOGOPAGE_SKIP_BUTTON				2
#define DEVICE_LOGOPAGE_INIT_EDIT					4
#define DEVICE_LOGOPAGE_LAMPON_EDIT				5
#define DEVICE_LOGOPAGE_WAVECALIB_EDIT			6
#define DEVICE_LOGOPAGE_STEP1_EDIT					7
#define DEVICE_LOGOPAGE_STEP2_EDIT					8
#define DEVICE_LOGOPAGE_STEP3_EDIT					9
#define DEVICE_LOGOPAGE_STEP4_EDIT					10

/************************************************************************/
/* 关于页面控件ID                                                       */
/************************************************************************/
#define DEVICE_ABOUTPAGE_SOFTVERSION_EDIT		1
#define DEVICE_ABOUTPAGE_HARDVERSION_EDIT		2
#define DEVICE_ABOUTPAGE_PRODUCTDATA_EDIT	  3
#define DEVICE_ABOUTPAGE_SERIES_EDIT	      6

#define DEVICE_ABOUTPAGE_RETURN_BUTTON	    30

/************************************************************************/
/* 泵运行页面控件ID                                                     */
/************************************************************************/
#define PUMP_RUNPAGE_FLOW_EDIT						1
#define PUMP_RUNPAGE_FLOW_PERCENT_EDIT		2
#define PUMP_RUNPAGE_PRESS_EDIT			    	3
#define PUMP_RUNPAGE_TIME_EDIT   					4
#define PUMP_RUNPAGE_START_BUTTON    			5
#define PUMP_RUNPAGE_STOP_BUTTON    		  6
#define PUMP_RUNPAGE_PURGE_BUTTON    			7

#define PUMP_RUNPAGE_STATUS_EDIT 					40

#define PUMP_RUNPAGE_SETUP_BUTTON					30
#define PUMP_RUNPAGE_MENU_BUTTON			    31
#define PUMP_RUNPAGE_CONNECT_BUTTON				32

//清洗页面
#define PUMP_PURGEPAGE_PURGE_BUTTON			    3

/************************************************************************/
/* 泵设备信息页面控件ID                                                   */
/************************************************************************/
#define PUMP_DEVICEINFOPAGE_MODEL_EDIT		1
#define PUMP_DEVICEINFOPAGE_FLOWMAX_EDIT		2
#define PUMP_DEVICEINFOPAGE_PRESSMAX_EDIT	3
#define PUMP_DEVICEINFOPAGE_PRESSMIN_EDIT	4

#define PUMP_DEVICEINFOPAGE_RETURN_BUTTON	30

/************************************************************************/
/* 检运行页面控件ID                                                     */
/************************************************************************/
#define DETECTOR_RUNPAGE_WAVE_EDIT				1
#define DETECTOR_RUNPAGE_WAVE2_EDIT				2
#define DETECTOR_RUNPAGE_AU_EDIT				  3
#define DETECTOR_RUNPAGE_AU2_EDIT					4
#define DETECTOR_RUNPAGE_TIME_EDIT   			5
#define DETECTOR_RUNPAGE_LAMP_BUTTON			6
#define DETECTOR_RUNPAGE_CLEAR_BUTTON			7

#define DETECTOR_RUNPAGE_STATUS_EDIT 			40

#define DETECTOR_RUNPAGE_SETUP_BUTTON			30
#define DETECTOR_RUNPAGE_MENU_BUTTON			31
#define DETECTOR_RUNPAGE_CONNECT_BUTTON		32

#define DETECTOR_RUNPAGE_WAVE2_TEXT				52
#define DETECTOR_RUNPAGE_WAVE2UNIT_TEXT		53
#define DETECTOR_RUNPAGE_AU2_TEXT					56
#define DETECTOR_RUNPAGE_AU2UNIT_TEXT			57

/************************************************************************/
/* 检设备信息页面控件ID                                                 */
/************************************************************************/
#define DETECTOR_DEVICEINFOPAGE_MODEL_EDIT		1
#define DETECTOR_DEVICEINFOPAGE_REF_EDIT		  2
#define DETECTOR_DEVICEINFOPAGE_SAM_EDIT	    3

#define DETECTOR_DEVICEINFOPAGE_RETURN_BUTTON	30

#ifdef __cplusplus
}
#endif

#endif
