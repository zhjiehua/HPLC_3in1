#include "ComProcess.h"
#include "IO.h"
#include "delay.h"
#include "ComDriver.h"

#include "StepMotor.h"
#include "DDC112.h"
#include "ADS8866.h"
#include "AD5683R.h"
#include "24cxx.h"

#include "management.h"
#include "CPrintf.h"
#include "CheckActions.h"

#include "cmd_process.h"
#include "hmi_driver.h"
#include "pageCommon.h"

#include "TMC5160_Driver.h"
#include "tmc/Board.h"

#include "DAC1220.h"

#include "adc.h"

void ComProcessFrame( ComFrame_TypeDef *msg, uint16 size )
{
 
	uint8_t address = msg->address;
	uint8_t pfc = msg->pfc;
	uint8_t *data = msg->data;
    uint8_t write = pfc&0x80;
    pfc &= 0x7F;
    
	if(address == man.machineAddress)
	{
        if(!man.isConnect)
        {
            SetButtonValue(RUNPAGE_INDEX, RUNPAGE_CONNECT_BUTTON, 1);
            cDebug("Connected!\r\n");
        }
        man.isConnect = 1;
        man.tickLostCnt = 0;
        
		switch(pfc)
		{  
				//通用功能码*********************************************************
			case PFC_GLOBAL_DEVICE_ADDRESS://设备地址
                Com_NAck();
			break;                                                 
			case PFC_GLOBAL_SOFTWARE_VERSION://软件版本               
				if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetSoftwareVersion((uint8_t*)SOFTWARE_VERSION);
                    cDebug("Read : SOFTWARE_VERSION = %s\r\n", (uint8_t*)SOFTWARE_VERSION);
                }
			break;
			case PFC_GLOBAL_HARDWARE_VERSION://硬件版本                                                                    
				if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetHardwareVersion((uint8_t*)HARDWARE_VERSION);
                    cDebug("Read : HARDWARE_VERSION = %s\r\n", (uint8_t*)HARDWARE_VERSION);
                }
			break;   
			case PFC_GLOBAL_PRODUCT_DATE://出厂日期                                                                   
				if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetProductDate(man.productDate);
                    cDebug("Read : man.productDate = %s\r\n", man.productDate);
                }
			break;   
			case PFC_GLOBAL_SERIAL_NUMBER://序列号                                                                      
				if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetSerialNumber(man.serialNumber);
                    cDebug("Read : man.serialNumber = %s\r\n", man.serialNumber);
                }
			break;                 
			case PFC_GLOBAL_MODEL://型号，该命令只能读，返回的为字符串                
				if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetModel(man.modelString);
                    cDebug("Read : man.modelString = %s\r\n", man.modelString);
                }
			break; 
            case PFC_GLOBAL_SYNC_TIME://同步时间                                                    
                Com_Ack();
                if(write)
                {
                    uint32_t time = (uint32_t)PTR2U32(data);//必须有中间变量
                    man.checkStartSecond = time;
                    cDebug("Write : man.checkStartSecond = %d\r\n", man.checkStartSecond);
                }
                else
                {
                    Com_SetSyncTime(man.checkStartSecond);
                    cDebug("Read : man.checkStartSecond = %d\r\n", man.checkStartSecond);
                }
			break;  
			case PFC_GLOBAL_USED_TIME://总使用时间                                                     
                if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetTotalUsedTime(man.deviceTotalUsedTime);
                    cDebug("Read : man.deviceTotalUsedTime = %d\r\n", man.deviceTotalUsedTime);
                }
			break;   
			 
			case PFC_GLOBAL_INPUT://输入点                                                     
                if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetInput(0, man.curInput&0x01);//只有一个输入点，MI_EXT
                    cDebug("Read : man.curInput = 0x%02X\r\n", man.curInput);
                }
			break; 
			case PFC_GLOBAL_OUTPUT://输出点
                Com_Ack();
                if(write)
                {
                    if(data[0] == 0)
                        MO_EXT = !!data[1];
                    cDebug("Write : Output-%d = %d\r\n", data[0], data[1]);
                }
                else
                {
                    if(data[0] == 0)
                    {
                        Com_SetOutput(0, !!MO_EXT);//只有一个输出点，MO_EXT
                        cDebug("Read : MO_EXT = %d\r\n", (uint8_t)MO_EXT);
                    }
                }
			break;
            case PFC_GLOBAL_TICK://心跳包                                                    
                if(write)
                {
                    //Com_Ack();
                    if(!man.isConnect)
                    {
                        SetButtonValue(RUNPAGE_INDEX, RUNPAGE_CONNECT_BUTTON, 1);
                        cDebug("Connected!");
                    }
                    man.isConnect = 1;
//                    man.tickLostCnt = 0;
                    
                }
                else
                    Com_NAck();
			break; 
			case PFC_GLOBAL_ERROR://系统错误                                                    
                if(write)
                {
                    Com_NAck();
                    //ComProcessFrame_SystemError(data[0]);
                }
                else
                {
                    Com_Ack();
                    cDebug("Read : PFC_GLOBAL_ERROR\r\n");
                }
			break;
            
            case PFC_GLOBAL_EXT_SYSTEM://通用系统参数
                if (write)
                    ComProcessFrame_WriteGlobalExternSystem(msg, size);
                else
                    ComProcessFrame_ReadGlobalExternSystem(msg, size);
                break;
            case PFC_GLOBAL_EXT_USER://通用用户自定义参数
                break;
            
            
            

			//检测器功能码**********************************************************
			case PFC_CHECK_WAVE://波长1
                Com_Ack();               
                if(write)
                {
                    uint16_t wave = (uint16_t)PTR2U16(data);
                    
                    if(wave < WAVE_MIN)
                        wave = WAVE_MIN;
                    else if(wave > WAVE_MAX)
                        wave = WAVE_MAX;
            
                    if(man.wave != wave)
                    {
                        man.wave = wave;
                        AT24CXX_WriteLenByte(EEPROM_ADDR_WAVE, man.wave, 2); 
                        
                        man.ddc112ReadCount = 0;
                    }
                    
                    man.waveStep = Check_WaveToStep((float)man.wave);
                    man.waveLocateFinish = 0;

                    cDebug("Write : man.wave = %d\r\n", man.wave);
                    
                    sprintf((char*)man.strLcd, "%d", man.wave);
                    SetTextValue(RUNPAGE_INDEX, RUNPAGE_WAVE_EDIT, man.strLcd);
                }
                else
                {
                    Com_SetWave(man.wave);
                    cDebug("Read : man.wave = %d\r\n", man.wave);
                }
			break;                                                 
			case PFC_CHECK_WAVE2://波长2
            {
                Com_Ack();              
                if(write)
                {
                    uint16_t wave = (uint16_t)PTR2U16(data);
                    
                    if(wave < WAVE_MIN)
                        wave = WAVE_MIN;
                    else if(wave > WAVE_MAX)
                        wave = WAVE_MAX;
                    
                    if(man.wave2 != wave)
                    {
                        man.wave2 = wave;
                        AT24CXX_WriteLenByte(EEPROM_ADDR_WAVE2, man.wave2, 2);
                        
                        man.ddc112ReadCount2 = 0;
                    }
                    
                    man.wave2Step = Check_WaveToStep((float)man.wave2);
                    man.waveLocateFinish = 0;
                    
                    cDebug("Write : man.wave2 = %d\r\n", man.wave2);
                    
                    sprintf((char*)man.strLcd, "%d", man.wave2);
                    SetTextValue(RUNPAGE_INDEX, RUNPAGE_WAVE2_EDIT, man.strLcd);
                }
                else
                {
                    Com_SetWave2(man.wave2);
                    cDebug("Read : man.wave2 = %d\r\n", man.wave2);
                }
            }
			break; 
			case PFC_CHECK_TIMECONST://时间常数
                Com_Ack();
                if(write)
                {
                    if(data[0] != man.timeConst)
                    {
                        uint8_t i;
                        
                        if(data[0] > man.timeConst)
                        {
                            for(i=timeConstant[man.timeConst];i<timeConstant[data[0]];i++)
                            {
                                man.refValueOrderFilterBuffer[i] = 0;
                                man.samValueOrderFilterBuffer[i] = 0;
                                man.auValueOrderFilterBuffer[i] = 0;
                            }
                            
                            if(man.channel == 1)//双通道
                            {
                                for(i=timeConstant[man.timeConst];i<timeConstant[data[0]];i++)
                                {
                                    man.refValue2OrderFilterBuffer[i] = 0;
                                    man.samValue2OrderFilterBuffer[i] = 0;
                                    man.auValue2OrderFilterBuffer[i] = 0;
                                }
                            }
                        }
                        else
                        {
                            uint8_t offset = timeConstant[man.timeConst] - timeConstant[data[0]];
                            for(i=0;i<timeConstant[data[0]];i++)
                            {
                                man.refValueOrderFilterBuffer[i] = man.refValueOrderFilterBuffer[offset+i];
                                man.samValueOrderFilterBuffer[i] = man.samValueOrderFilterBuffer[offset+i];
                                man.auValueOrderFilterBuffer[i] = man.auValueOrderFilterBuffer[offset+i];
                            }
                            
                            if(man.channel == 1)//双通道
                            {
                                for(i=0;i<timeConstant[data[0]];i++)
                                {
                                    man.refValue2OrderFilterBuffer[i] = man.refValue2OrderFilterBuffer[offset+i];
                                    man.samValue2OrderFilterBuffer[i] = man.samValue2OrderFilterBuffer[offset+i];
                                    man.auValue2OrderFilterBuffer[i] = man.auValue2OrderFilterBuffer[offset+i];
                                }
                            }
                        }
                        
                        man.timeConst = data[0];
                        AT24CXX_WriteOneByte(EEPROM_ADDR_TIMECONST, man.timeConst);
                        
                        sprintf((char*)man.strLcd, "%s", TimeConst[man.timeConst]);
                        SetTextValue(SETUPPAGE_INDEX, SETUPPAGE_TIMECONST_EDIT, man.strLcd);
                    }

                    cDebug("Write : man.timeConst = %d\r\n", man.timeConst);
                }
                else
                {
                    Com_SetTimeConst(man.timeConst);
                    cDebug("Read : man.timeConst = %d\r\n", man.timeConst);
                }
			break; 
			case PFC_CHECK_RANGE://量程
				Com_Ack();
                if(write)
                {
                    if(man.range != data[0])
                    {
                        man.range = data[0];
                        AT24CXX_WriteOneByte(EEPROM_ADDR_RANGE, man.range);
                        
                        sprintf((char*)man.strLcd, "%s", Range[man.range]);
                        SetTextValue(SETUPPAGE_INDEX, SETUPPAGE_RANGE_EDIT, man.strLcd);
                    }
                    
                    cDebug("Write : man.range = %d\r\n", man.range); 
                }
                else
                {
                    Com_SetRange(man.range);
                    cDebug("Read : man.range = %d\r\n", man.range);
                }                
			break;
            case PFC_CHECK_LAMP_TYPE://灯类型 
				Com_Ack();
                if(write)
                {
                    if(man.lampType != data[0])
                    {
                        man.lampType = !!data[0];
                        AT24CXX_WriteOneByte(EEPROM_ADDR_LAMPTYPE, man.lampType);
                        
                        if(man.language)
                            sprintf((char*)man.strLcd, "%s", LampType[man.lampType]);
                        else
                            sprintf((char*)man.strLcd, "%s", LampType_En[man.lampType]);
                        SetTextValue(SETUPPAGE_INDEX, SETUPPAGE_LAMPTYPE_EDIT, man.strLcd);
                    }
                    
                    cDebug("Write : man.lampType = %d\r\n", man.lampType); 
                }
                else
                {
                    Com_SetLampType(man.lampType);
                    cDebug("Read : man.lampType = %d\r\n", man.lampType);
                }  
			break;
            case PFC_CHECK_CHANNEL://通道               
                Com_Ack();
                if(write)
                {
                    if(man.channel != data[0])
                    {
                        man.channel = !!data[0];
                        AT24CXX_WriteOneByte(EEPROM_ADDR_CHANNEL, man.channel);

                        if(man.language)
                            sprintf((char*)man.strLcd, "%s", Channel[man.channel]);
                        else
                            sprintf((char*)man.strLcd, "%s", Channel_En[man.channel]);
                        SetTextValue(SETUPPAGE_INDEX, SETUPPAGE_CHANNEL_EDIT, man.strLcd);
                        
                        if(man.channel == 0)//单通道
                        {
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_EDIT, 0);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_EDIT, 0);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_REF2_EDIT, 0);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_SAM2_EDIT, 0);
                            
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_TEXT, 0);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2UNIT_TEXT, 0);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_TEXT, 0);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2UNIT_TEXT, 0);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_REF2_TEXT, 0);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_SAM2_TEXT, 0);
                        }
                        else
                        {
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_EDIT, 1);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_EDIT, 1);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_REF2_EDIT, 1);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_SAM2_EDIT, 1);
                            
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_TEXT, 1);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2UNIT_TEXT, 1);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_TEXT, 1);
                            SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2UNIT_TEXT, 1);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_REF2_TEXT, 1);
                            SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_SAM2_TEXT, 1);
                        }
                    }
                    
                    cDebug("Write : man.channel = %d\r\n", man.channel); 
                }
                else
                {
                    Com_SetChannel(man.channel);
                    cDebug("Read : man.channel = %d\r\n", man.channel);
                }  
			break;                
			case PFC_CHECK_LAMP://灯状态
                Com_Ack();
                if(write)
                {
                    if(man.lampState != data[0])
                    {
                        man.lampState = data[0];
                        if(man.lampState)//开灯
                        {
                            MO_LAMP = 0;//电平拉高后要等待大约10s才会点亮氘灯
                            man.lampTotalOnOffTimes++;
                            AT24CXX_Write(EEPROM_ADDR_LAMPONOFFTIMES, (uint8_t*)&man.lampTotalOnOffTimes, 2);
                        }
                        else
                        {
                            MO_LAMP = 1;
                            man.lampOnSecondCnt = 0;
                            man.isLampOn = 0;
                        }
                    }
                    cDebug("Write : man.lampState = %d\r\n", man.lampState); 
                    
                    SetButtonValue(RUNPAGE_INDEX, RUNPAGE_LAMP_BUTTON, !man.lampState);
                }
                else
                {
                    Com_SetLamp(man.isLampOn);
                    cDebug("Read : man.isLampOn = %d\r\n", man.isLampOn);
                }  
			break; 
			case PFC_CHECK_ZERO://基线归零                
                if(write)
                {
                    uint8_t i;
                    
                    Com_Ack();//回复
                    
                    man.auZeroFactor = (float)(man.samValue)/(float)(man.refValue);
                    
                    for(i=0;i<timeConstant[man.timeConst];i++)
                        man.auValueOrderFilterBuffer[i] = 0;
        
                    cDebug("Write : man.auZeroFactor = %f\r\n", man.auZeroFactor);
                    
                    if(man.channel == 1)
                    {
                        man.auZeroFactor2 = (float)(man.samValue2)/(float)(man.refValue2);
                        
                        for(i=0;i<timeConstant[man.timeConst];i++)
                            man.auValue2OrderFilterBuffer[i] = 0;
                        
                        cDebug("Write : man.auZeroFactor2 = %f\r\n", man.auZeroFactor2);
                    }
                }
                else
                {
                    Com_NAck();
                }  
			break; 
			case PFC_CHECK_HOME://回零点
                Com_Ack();
                if(write)
                {
                    if(data[0])
                    {
                        man.homeFlag = 1;
                        cDebug("\r\n Home \r\n");
                    }
                    else
                    {
                        cDebug("Stop Home\r\n");
                    }
                    
                    cDebug("Write : man.homeFlag = %d\r\n", man.homeFlag);
                }
                else
                {
                    Com_SetHome(man.homeFlag);
                    cDebug("Read : man.homeFlag = %d\r\n", man.homeFlag);
                }  
			break;                                                 
			case PFC_CHECK_AU_UPLOAD_FREQ://AU值上传频率
                Com_Ack();                
                if(write)
                {
                    //if(data[0] < 10)
                    {
                        man.auUploadFreq = data[0];
                        AT24CXX_WriteOneByte(EEPROM_ADDR_AUUPLOADFREQ, man.auUploadFreq);
                        cDebug("Write : man.auUploadFreq = %d\r\n", man.auUploadFreq);
                    }
                }
                else
                {
                    Com_SetAuUploadFreq(man.auUploadFreq);
                    cDebug("Read : man.auUploadFreq = %d\r\n", man.auUploadFreq);
                }
			break; 
			case PFC_CHECK_AU://AU值
                if(write)
                    Com_NAck();
                else
                {
                    Com_Ack();
                    Com_SetAu(man.sendPCAuValue, man.sendPCAuValue2);
                    cDebug("Read : man.sendPCAuValue = %d, man.sendPCAuValue2 = %d\r\n", man.sendPCAuValue, man.sendPCAuValue2);
                }
			break; 
			case PFC_CHECK_WAVE_REF_SAM://波长1参比-样本值
                if(write)
                    Com_NAck();
                else
                {
                    Com_Ack();
                    Com_SetWaveRefSam(man.refValue/1044479.0*1000000, man.samValue/1044479.0*1000000);
                    cDebug("Read : man.refValue = %d, man.samValue = %d\r\n", man.refValue, man.samValue);
                }
			break;                                                 
			case PFC_CHECK_WAVE2_REF_SAM://波长2参比-样本值
                if(write)
                    Com_NAck();
                else
                {
                    Com_Ack();
                    Com_SetWave2RefSam(man.refValue2/1044479.0*1000000, man.samValue2/1044479.0*1000000);
                    cDebug("Read : man.refValue2 = %d, man.samValue2 = %d\r\n", man.refValue2, man.samValue2);
                }
			break; 
            case PFC_CHECK_SCAN_CMD://波长扫描使能
                Com_Ack();
                if(write)
                {
                    man.isScanning = data[0];
                    if(man.isScanning)//启动扫描
                    {
                        man.scanTimeCnt = 0;
                    
                        man.channel = 0;//强制单通道
                        sprintf((char*)man.strLcd, "%s", Channel[man.channel]);
                        SetTextValue(SETUPPAGE_INDEX, SETUPPAGE_CHANNEL_EDIT, man.strLcd);
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_EDIT, 0);
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_EDIT, 0);
                        SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_REF2_EDIT, 0);
                        SetControlVisiable(DEVICEINFOPAGE_INDEX, DEVICEINFOPAGE_SAM2_EDIT, 0);
                    
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2_TEXT, 0);
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_WAVE2UNIT_TEXT, 0);
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2_TEXT, 0);
                        SetControlVisiable(RUNPAGE_INDEX, RUNPAGE_AU2UNIT_TEXT, 0);
                        
                        sprintf((char*)man.strLcd, "%d", man.scanWaveStart);
                        SetTextValue(SCANPAGE_INDEX, SCANPAGE_WAVESTART_EDIT, man.strLcd);
                        sprintf((char*)man.strLcd, "%d", man.scanWaveEnd);
                        SetTextValue(SCANPAGE_INDEX, SCANPAGE_WAVEEND_EDIT, man.strLcd);
                        sprintf((char*)man.strLcd, "%d", man.scanWaveSpeed);
                        SetTextValue(SCANPAGE_INDEX, SCANPAGE_SCANSPEED_EDIT, man.strLcd);
                        SetButtonValue(SCANPAGE_INDEX, SCANPAGE_START_BUTTON, 1);
                        SetScreen(SCANPAGE_INDEX);
                        
                        //设置起始波长
                        man.currentScanWave = man.scanWaveStart;
                        man.isScanChangeWave = 1;
                        
                        man.curPage = SCANPAGE_INDEX;
                        
                        cDebug("WaveScan Start\r\n");
                    }
                    else
                    {
                        SetButtonValue(SCANPAGE_INDEX, SCANPAGE_START_BUTTON, 0);
                        cDebug("WaveScan Stop\r\n");
                    }
                    
                    cDebug("Write : man.isScanning = %d\r\n", man.isScanning);
                }
                else
                {
                    Com_SetScanCMD(man.isScanning);
                    cDebug("Read : man.isScanning = %d\r\n", man.isScanning);
                }
            break;
            case PFC_CHECK_SCAN_PARA://波长扫描参数
                Com_Ack();
                if(write)
                {
                    man.scanWaveStart = (uint16_t)PTR2U16(data);
                    man.scanWaveEnd = (uint16_t)PTR2U16(data+2);
                    man.scanWaveSpeed = data[4];
                    
                    AT24CXX_Write(EEPROM_ADDR_SCANWAVESTART, (uint8_t*)&man.scanWaveStart, 2);
                    AT24CXX_Write(EEPROM_ADDR_SCANWAVEEND, (uint8_t*)&man.scanWaveEnd, 2);
                    AT24CXX_WriteOneByte(EEPROM_ADDR_SCANWAVESPEED, man.scanWaveSpeed);
                    
                    cDebug("Write : man.scanWaveStart = %d\r\n", man.scanWaveStart);
                    cDebug("Write : man.scanWaveEnd = %d\r\n", man.scanWaveEnd);
                    cDebug("Write : man.scanWaveSpeed = %d\r\n", man.scanWaveSpeed);
                }
                else
                {
                    Com_SetScanPara(man.scanWaveStart, man.scanWaveEnd, man.scanWaveSpeed);
                    cDebug("Read : man.scanWaveStart = %d\r\n", man.scanWaveStart);
                    cDebug("Read : man.scanWaveEnd = %d\r\n", man.scanWaveEnd);
                    cDebug("Read : man.scanWaveSpeed = %d\r\n", man.scanWaveSpeed);
                }
            break;
            case PFC_CHECK_LAMP_USED_TIME://灯源使用时间
                if(write)
                {
                    Com_NAck();
                }
                else
                {
                    Com_Ack();
                    Com_SetLampUsedTime(man.lampTotalUsedTime/3600);
                    cDebug("Read : man.lampTotalUsedTime = %d\r\n", man.lampTotalUsedTime);
                    
                }
                break;
            case PFC_CHECK_START_STOP:
                Com_Ack();
                if(write)
                {
                    man.checkLED = data[0];
                    cDebug("Write : man.checkLED = %d\r\n", man.checkLED);
                }
                else
                {
                    Com_SetStartStop(man.checkLED);
                    cDebug("Read : man.checkLED = %d\r\n", man.checkLED);
                }
            break;
                
            case PFC_CHECK_EXT_SYSTEM://检测器系统参数
                if (write)
                    ComProcessFrame_WriteCheckExternSystem(msg, size);
                else
                    ComProcessFrame_ReadCheckExternSystem(msg, size);
                break;
            case PFC_CHECK_EXT_USER://检测器用户自定义参数
                break;
    
			default:
                Com_NAck();
				cDebug("\r\n '%02x' Error Function Code !!\r\n",pfc);//cDebug("\r\n Function Code Error !!\r\n");
			break;   
		}       
	}
	else
    {        
        Com_NAck();
		cDebug("\r\n '%02x' Error address code !!\r\n",address);//cDebug("\r\n Address code error !!\r\n"); 
    }        

}



