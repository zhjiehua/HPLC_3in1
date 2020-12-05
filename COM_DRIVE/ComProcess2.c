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
#include "wdg.h"

void ComProcessFrame_WriteGlobalExternSystem( ComFrame_TypeDef *msg, uint16 size )
{
    uint8_t pfce = msg->data[0];
	uint8_t *data = msg->data + 1;
    
    switch (pfce)
	{
        case PFC_GES_PRIORITY:
//        {
//            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
//            float flow = *(float*)(&temp);
//        }
//            if(data[0] == 0x55)
//            {
//                Com_Ack();
//                man.priority = data[1];
//            }
//            else
//                Com_NAck();
            break;
		case PFC_GES_RESET:
            Com_Ack();
            SoftReset();
            cDebug("Write : Reset the machine!\r\n");
			break;
		case PFC_GES_RESTORE_DEFAULT://�ָ���������
            Com_Ack();
        {
            if(data[0] == 0)
            {
                uint16_t i;
                uint8_t eepromData;
								
								cDebug("Write : Restore default...\r\n");
							
                for(i=0;i<EEPROM_ADDR_FACTORYDATA;i++)
                {
                    eepromData = AT24CXX_ReadOneByte(EEPROM_ADDR_FACTORYDATA_REAL+i);
                    AT24CXX_WriteOneByte(i, eepromData);
										IWDG_Feed();
                }
                
                //���
                for(i=0;i<EEPROM_ADDR_FACTORYDATA;i++)
                {
                    eepromData = AT24CXX_ReadOneByte(EEPROM_ADDR_FACTORYDATA_REAL+i);
                    if(eepromData != AT24CXX_ReadOneByte(i))
                        AT24CXX_WriteOneByte(i, eepromData);
										IWDG_Feed();
                }
                
                Com_SystemExt_SetRestoreDefault();
                
                cDebug("Write : Restore default...Finish\r\n");
            }
            else
            {
                AT24CXX_WriteOneByte(EEPROM_ADDR_DEFAULT, (uint8_t)(~EEPROM_DEFAULT_DATA));
                cDebug("Write : Restore default-First Power On!\r\n");
            }
        }
			break;
		case PFC_GES_SAVE_AS_DEFAULT://����Ϊ�������� 
            Com_Ack();
        {
            uint16_t i;
            uint8_t eepromData;
					
						cDebug("Write : Save as default...\r\n");
					
            for(i=0;i<EEPROM_ADDR_FACTORYDATA;i++)
            {
                eepromData = AT24CXX_ReadOneByte(i);
                AT24CXX_WriteOneByte(EEPROM_ADDR_FACTORYDATA_REAL+i, eepromData);
								IWDG_Feed();
            }
            
            //���
            for(i=0;i<EEPROM_ADDR_FACTORYDATA;i++)
            {
                eepromData = AT24CXX_ReadOneByte(i);
                if(eepromData != AT24CXX_ReadOneByte(EEPROM_ADDR_FACTORYDATA_REAL+i))
                    AT24CXX_WriteOneByte(EEPROM_ADDR_FACTORYDATA_REAL+i, eepromData);
								IWDG_Feed();
            }
            
            Com_SystemExt_SetSaveAsDefault();
            
            cDebug("Write : Save as default...Finish\r\n");
        }
			break;
		case PFC_GES_SUPERPASSWORD:       
            Com_Ack();
        {
        
            strcpy((char*)man.superPassword, (char*)data);
            AT24CXX_Write(EEPROM_ADDR_SUPERPASSWORD, (uint8_t*)&man.superPassword, strlen((const char*)man.superPassword)+1);
            cDebug("Write : man.superPassword = %s\r\n", man.superPassword);
			break;
        }
        case PFC_GES_SOFTWARE_VERSION:
            Com_NAck();
            break;
        case PFC_GES_HARDWARE_VERSION:
            Com_NAck();
            break;
        case PFC_GES_PRODUCT_DATE:
            Com_Ack();
            strcpy((char*)man.productDate, (char*)data);
            AT24CXX_Write(EEPROM_ADDR_PRODUCTDATE, (uint8_t*)&man.productDate, strlen((const char*)man.productDate)+1);
            cDebug("Write : man.productDate = %s\r\n", man.productDate);
            break;
        case PFC_GES_SERIAL_NUMBER:
            Com_Ack();
            strcpy((char*)man.serialNumber, (char*)data);
            AT24CXX_Write(EEPROM_ADDR_SERIALNUMBER, (uint8_t*)&man.serialNumber, strlen((const char*)man.serialNumber)+1);
            cDebug("Write : man.serialNumber = %s\r\n", man.serialNumber);
            break;
        case PFC_GES_MODEL:
            Com_Ack();
            man.model = data[0];
            AT24CXX_WriteOneByte(EEPROM_ADDR_MODEL, man.model);
            cDebug("Write : man.model = %d\r\n", man.model);
            break;
        case PFC_GES_MODEL_STRING:
            Com_Ack();
            strcpy((char*)man.modelString, (char*)data);
            AT24CXX_Write(EEPROM_ADDR_MODELSTRING, (uint8_t*)&man.modelString, strlen((const char*)man.modelString)+1);
            cDebug("Write : man.modelString = %s\r\n", man.modelString);
            break;
        case PFC_GES_USED_TIME:
            Com_Ack();
            man.deviceTotalUsedTime = (uint32_t)PTR2U32(data);
            AT24CXX_Write(EEPROM_ADDR_DEVICETOTALUSEDTIME, (uint8_t*)&man.deviceTotalUsedTime, 4);
            cDebug("Write : man.deviceTotalUsedTime = %d\r\n", man.deviceTotalUsedTime);
            break;

		default:
            Com_NAck();
            cDebug("\r\n Write : '%02x' Error Function Code Extern!!\r\n", pfce);
			break;
	}
}

void ComProcessFrame_ReadGlobalExternSystem( ComFrame_TypeDef *msg, uint16 size )
{
    uint8_t pfce = msg->data[0];
	//uint8_t *data = msg->data + 1;

	switch (pfce)
	{
    case PFC_GES_PRIORITY:
        Com_Ack();
        srand(TIM2->CNT);
        man.randomCode = rand();
        Com_SystemExt_SetRandomCode(man.randomCode);
        cDebug("Read : man.randomCode = %d\r\n", man.randomCode);
        break;
		case PFC_GES_RESET:
        Com_NAck();
			break;
		case PFC_GES_RESTORE_DEFAULT:
        Com_NAck();
			break;
		case PFC_GES_SAVE_AS_DEFAULT:
        Com_NAck();
			break;
		case PFC_GES_SUPERPASSWORD://����Ա����     
        Com_Ack();
        Com_SystemExt_SetSuperPassword(man.superPassword);
        cDebug("Read : man.superPassword = %s\r\n", man.superPassword);
    break;
    case PFC_GES_SOFTWARE_VERSION:
        Com_Ack();
        Com_SystemExt_SetSoftwareVersion((uint8_t*)SOFTWARE_VERSION);
        cDebug("Read : SOFTWARE_VERSION = %s\r\n", (uint8_t*)SOFTWARE_VERSION);
        break;
    case PFC_GES_HARDWARE_VERSION:
        Com_Ack();
        Com_SystemExt_SetHardwareVersion((uint8_t*)HARDWARE_VERSION);
        cDebug("Read : HARDWARE_VERSION = %s\r\n", (uint8_t*)HARDWARE_VERSION);
        break;
    case PFC_GES_PRODUCT_DATE:
        Com_Ack();
        Com_SystemExt_SetProductDate(man.productDate);
        cDebug("Read : man.productDate = %s\r\n", man.productDate);
        break;
    case PFC_GES_SERIAL_NUMBER:
        Com_Ack();
        Com_SystemExt_SetSerialNumber(man.serialNumber);
        cDebug("Read : man.serialNumber = %s\r\n", man.serialNumber);
        break;
    case PFC_GES_MODEL:
        Com_Ack();
        Com_SystemExt_SetModel(man.model);
        cDebug("Read : man.model = %d\r\n", man.model);
        break;
    case PFC_GES_MODEL_STRING:
        Com_Ack();
        Com_SystemExt_SetModelString(man.modelString);
        cDebug("Read : man.modelString = %s\r\n", man.modelString);
        break;
    case PFC_GES_USED_TIME:
        Com_Ack();
        Com_SystemExt_SetTotalUsedTime(man.deviceTotalUsedTime);
        cDebug("Read : man.deviceTotalUsedTime = %d\r\n", man.deviceTotalUsedTime);
        break;
    case PFC_GES_ALL_DATA://��ȡȫ������
        Com_Ack();
        UploadAllData();
    break;
		default:
            Com_NAck();
            cDebug("\r\n Read : '%02x' Error Function Code Extern!!\r\n", pfce);
			break;
	}
}

void ComProcessFrame_WriteCheckExternSystem( ComFrame_TypeDef *msg, uint16 size )
{
    uint8_t pfce = msg->data[0];
	uint8_t *data = msg->data + 1;
    
    switch (pfce)
	{
        case PFC_CES_COIL://���������/�ɿ�
            Com_Ack();
            if(data[0])//������ŷ�
            {
                MO_COIL = 0;
                cDebug("Write : Close the COIL!\r\n");
            }
            else
            {
                MO_COIL = 1;
                cDebug("Write : Open the COIL!\r\n");
            }
			break;
		case PFC_CES_TIME_INTEGRAL://����ʱ��
            Com_Ack();
        {
            uint16_t time = PTR2U16(data);
            if(man.timeIntegral != time && time >=500 && time <= 4000)
            {
                man.timeIntegral = time;
                AT24CXX_Write(EEPROM_ADDR_TIMEINTEGRAL, (uint8_t*)&man.timeIntegral, 2);
                
                DDC112_SetIntegralTime(man.timeIntegral);
            }
            else
                cDebug("Write : man.timeIntegral  time = %d\r\n", time);
            
            cDebug("Write : man.timeIntegral = %d\r\n", man.timeIntegral);
        }
			break;
		case PFC_CES_RANGE_INTEGRAL://��������
            Com_Ack();
        {
            if(man.rangeIntegral != data[0])
            {
                man.rangeIntegral = data[0];
                AT24CXX_WriteOneByte(EEPROM_ADDR_RANGEINTEGRAL, man.rangeIntegral);

                DDC112_RANGE0 = !!(man.rangeIntegral & 0x01);
                DDC112_RANGE1 = !!(man.rangeIntegral & 0x02);
                DDC112_RANGE2 = !!(man.rangeIntegral & 0x04);
            }
            
            cDebug("Write : man.rangeIntegral = %d\r\n", man.rangeIntegral);
        }
			break;
		case PFC_CES_AU_FACTOR://�궨ϵ��
            Com_Ack();
        {
            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
            float factor = *(float*)(&temp);
            if(man.auFactor != factor)
            {
                man.auFactor = factor;
                AT24CXX_Write(EEPROM_ADDR_AUFACTOR, (uint8_t*)&man.auFactor, 4);
            }
            
            cDebug("Write : man.auFactor = %f\r\n", man.auFactor);
            
//            //����DAC
//            uint32_t value = factor/5.0*(float)0x000FFFFF;
//            DAC1220_WriteOutput(value);
//            cDebug("Write : man.auFactor = %f, value = 0x%X\r\n", factor, value);
        }
			break;
		case PFC_CES_RASTER_STEP://��դת��ָ������
            Com_Ack();
        {
#if 0 //��ͨ��������                    
            Direction_TypeDef dir = (Direction_TypeDef)data[0];
            uint32_t step = (uint32_t)PTR2U32(&data[1]);//�������м����

            cDebug("Write : dir = %d, step = %d\r\n", dir, step);
            
            StepMotor_RunPosition(0, dir, 0.01, step);
            while(!StepMotor_IsStop(0));
            
            cDebug("PFC_CES_RASTER_STEP-StepMotor is finish\r\n");
#else                    
            uint32_t d;
            int32_t xTarget;
            
            d = (uint32_t)PTR2U32(&data[1]);
            if(data[0]) //CCW����������
            {                       
                xTarget = (int32_t)d;               
                Evalboards.ch1.moveBy(0, &xTarget);
            }
            else //CW����������
            {
                xTarget = (int32_t)d;                        
                xTarget = 0 - xTarget;
                Evalboards.ch1.moveBy(0, &xTarget);
                
            }
            
            cDebug("Write : dir = %d, xTarget = %d\r\n", data[0], xTarget);
#endif
        }
			break;
		case PFC_CES_HOME_OFFSET://ÿ�ι�������Ҫƫ�Ʋ���
            Com_Ack();
        {
            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
            if(man.homeOffset != temp)
            {
                man.homeOffset = temp;
                AT24CXX_WriteLenByte(EEPROM_ADDR_HOMEOFFSET, man.homeOffset, 4);
            }
            
            cDebug("Write : man.homeOffset = %d\r\n", man.homeOffset); 
        }
			break;
		case PFC_CES_WAVE_OFFSET://������λ�ã�ÿ�θı䷽��ʱ����դ��Ҫ�߶�waveOffset����
            Com_Ack();
        {
            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
            if(man.waveOffset != temp)
            {
                man.waveOffset = temp;
                AT24CXX_WriteLenByte(EEPROM_ADDR_WAVEOFFSET, man.waveOffset, 4);
            }
            cDebug("Write : man.waveOffset = %d\r\n", man.waveOffset); 
        }
			break;
		case PFC_CES_WORKMODE://����ģʽ
            Com_Ack();
        {
            if(data[1])
                man.workMode |= data[0]; 
            else
                man.workMode &= ~data[0];
            
            cDebug("Write : man.workMode = %d\r\n", man.workMode);  
        }
			break;
		case PFC_CES_WAVECALIB_DATA://���ͼ첨��У׼����
            Com_Ack();
        {
            if(man.recieveWaveCalibDataFlag && man.waveCalibTableSize < WAVECALIBTABLE_SIZE)
            {
                uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
                float wave = *(float*)(&temp);
                
                if(man.waveCalibTableSize && (wave == man.waveCalibTable[man.waveCalibTableSize-1].wave))//���ܻ��ط��ϴ�����
                    break;
                
                man.waveCalibTable[man.waveCalibTableSize].wave = wave;
                temp = (uint32_t)PTR2U32(data+4);
                wave = *(float*)(&temp);
                man.waveCalibTable[man.waveCalibTableSize].realWave = wave;
                man.waveCalibTableSize++;
            }
        }
			break;
		case PFC_CES_WAVECALIB_DATA_CMD://��ʼ/�������ͼ첨��У׼����
            Com_Ack();
        {
            if(data[0])
            {
                man.waveCalibTableSize = 0;
                man.recieveWaveCalibDataFlag = 1;
                
                cDebug("Write : Start transmit the WaveCalibTable\r\n");
            }
            else
            {
                uint8_t i;
                man.recieveWaveCalibDataFlag = 0;
                
                //��������
                AT24CXX_Write(EEPROM_ADDR_WAVECALIBTABLE, (uint8_t*)(man.waveCalibTable), 2*4*man.waveCalibTableSize);
                AT24CXX_WriteOneByte(EEPROM_ADDR_WAVECALIBTABLESIZE, man.waveCalibTableSize);
                
                cDebug("The WaveCalibTable is:\r\n");
                for(i=0;i<man.waveCalibTableSize;i++)
                {
                    cDebug("%d = [%f, %f]\r\n", i, man.waveCalibTable[i].wave, man.waveCalibTable[i].realWave);
                }
            }
        }
			break;
        case PFC_CES_LAMP_USED_TIME://��Դʹ��ʱ��
            Com_Ack();
        {
            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
            man.lampTotalUsedTime = temp;
            AT24CXX_Write(EEPROM_ADDR_LAMPTOTALUSEDTIME, (uint8_t*)&man.lampTotalUsedTime, 4);
            cDebug("Write : man.lampTotalUsedTime = %d\r\n", man.lampTotalUsedTime);
        }
            break;
        case PFC_CES_LAMP_ONOFF_TIMES://��Դ���ػ�����
            Com_Ack();
        {
            uint16_t temp = (uint32_t)PTR2U16(data);//�������м����
            man.lampTotalOnOffTimes = temp;
            AT24CXX_Write(EEPROM_ADDR_LAMPONOFFTIMES, (uint8_t*)&man.lampTotalOnOffTimes, 2);
            cDebug("Write : man.lampTotalOnOffTimes = %d\r\n", man.lampTotalOnOffTimes);
        }
            break;
        case PFC_CES_LAMP_COUNT://��Դ�������ڼ�ջ��
            Com_Ack();
        {
            man.lampCount = data[0];
            AT24CXX_Write(EEPROM_ADDR_LAMPCOUNT, (uint8_t*)&man.lampCount, 1);
            cDebug("Write : man.lampCount = %d\r\n", man.lampCount);
        }
            break;
        case PFC_CES_DAC_ZEROVOLTAGE://DAC����ѹ
            Com_Ack();
        {
            uint32_t temp = (uint32_t)PTR2U32(data);//�������м����
            man.dacZeroVoltage = *(float*)(&temp);
            AT24CXX_Write(EEPROM_ADDR_DACZEROVOLAGE, (uint8_t*)&man.dacZeroVoltage, 4);
            cDebug("Write : man.dacZeroVoltage = %f\r\n", man.dacZeroVoltage);
        }
            break;
        case PFC_CES_UPLOADOBJECT://�ϴ�����
            Com_Ack();
            man.uploadObject = data[0];
            AT24CXX_WriteOneByte(EEPROM_ADDR_UPLOADOBJECT, man.uploadObject);
            cDebug("Write : man.uploadObject = %d\r\n", man.uploadObject);
            break;
        default:
            Com_NAck();
            cDebug("\r\n Write : '%02x' Error Function Code Extern!!\r\n", pfce);
            break;
    }
}
            
void ComProcessFrame_ReadCheckExternSystem( ComFrame_TypeDef *msg, uint16 size )
{
    uint8_t pfce = msg->data[0];
	//uint8_t *data = msg->data + 1;
    
    switch (pfce)
	{
        case PFC_CES_COIL:
            Com_Ack();
            Com_SystemExt_SetCoil(!!MO_COIL);
            cDebug("Read : MO_COIL = %d\r\n", (uint8_t)!!MO_COIL);
			break;
		case PFC_CES_TIME_INTEGRAL:
            Com_Ack();
            Com_SystemExt_SetTimeIntegral(man.timeIntegral);
            cDebug("Read : man.timeIntegral = %d\r\n", man.timeIntegral);
			break;
		case PFC_CES_RANGE_INTEGRAL:
            Com_Ack();
            Com_SystemExt_SetRangeIntegral(man.rangeIntegral);
            cDebug("Read : man.rangeIntegral = %d\r\n", man.rangeIntegral);
			break;
		case PFC_CES_AU_FACTOR:
            Com_Ack();
            Com_SystemExt_SetAuFactor(man.auFactor);
            cDebug("Read : man.auFactor = %f\r\n", man.auFactor);
			break;
		case PFC_CES_RASTER_STEP:
            Com_NAck();
			break;
		case PFC_CES_HOME_OFFSET://ÿ�ι�������Ҫƫ�Ʋ���            
            Com_Ack();
            Com_SystemExt_SetHomeOffset(man.homeOffset);
            cDebug("Read : man.homeOffset = %d\r\n", man.homeOffset);
			break;
		case PFC_CES_WAVE_OFFSET://������λ�ã�ÿ�θı䷽��ʱ����դ��Ҫ�߶�waveOffset����
            Com_Ack();
            Com_SystemExt_SetWaveOffset(man.waveOffset);
            cDebug("Read : man.waveOffset = %d\r\n", man.waveOffset);
			break;
		case PFC_CES_WORKMODE://����ģʽ
            Com_Ack();
            Com_SystemExt_SetWorkMode(man.workMode);
            cDebug("Read : man.workMode = %d\r\n", man.workMode);
			break;
		case PFC_CES_WAVECALIB_DATA://���ͼ첨��У׼����
            Com_Ack();
        {
            uint8_t i;
            Com_SystemExt_SetWaveCalibDataCMD(1);
            for(i=0;i<man.waveCalibTableSize;i++)
                Com_SystemExt_SetWaveCalibData(man.waveCalibTable[i].wave, man.waveCalibTable[i].realWave);
            Com_SystemExt_SetWaveCalibDataCMD(0);
            
            cDebug("Read : man.waveCalibTable\r\n");
        }
			break;
		case PFC_CES_WAVECALIB_DATA_CMD://��ʼ/�������ͼ첨��У׼����
            Com_NAck();
			break;
        case PFC_CES_LAMP_USED_TIME://��Դʹ��ʱ��
            Com_Ack();
            Com_SystemExt_SetLampUsedTime(man.lampTotalUsedTime);
            cDebug("Read : man.lampTotalUsedTime = %d\r\n", man.lampTotalUsedTime);
            break;
        case PFC_CES_LAMP_ONOFF_TIMES://��Դ���ػ�����
            Com_Ack();
            Com_SystemExt_SetLampOnOffTimes(man.lampTotalOnOffTimes);
            cDebug("Read : man.lampTotalOnOffTimes = %d\r\n", man.lampTotalOnOffTimes);
            break;
        case PFC_CES_LAMP_COUNT://��Դ�������ڼ�ջ��
            Com_Ack();
            Com_SystemExt_SetLampCount(man.lampCount);
            cDebug("Read : man.lampCount = %d\r\n", man.lampCount);
            break;
        case PFC_CES_DAC_ZEROVOLTAGE://DAC����ѹ
            Com_Ack();
            Com_SystemExt_SetDACZeroVoltage(man.dacZeroVoltage);
            cDebug("Read : man.dacZeroVoltage = %f\r\n", man.dacZeroVoltage);
            break;
        case PFC_CES_UPLOADOBJECT://�ϴ�����
            Com_Ack();
            Com_SystemExt_SetUploadObject(man.uploadObject);
            cDebug("Read : man.uploadObject = %d\r\n", man.uploadObject);
            break;
        default:
            Com_NAck();
            cDebug("\r\n Read : '%02x' Error Function Code Extern!!\r\n", pfce);
            break;
    }
}    

