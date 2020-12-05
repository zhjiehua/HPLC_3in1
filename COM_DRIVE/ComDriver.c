/************************************版权申明********************************************
**                             广州大彩光电科技有限公司
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   hmi_driver.c
** 修改时间:   2018-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------*/
#include "ComDriver.h"
#include "ComUart.h"
#include "ComProcess.h"
#include "management.h"
#include "miscellaneous.h"
#include "ComLAN.h"

uint8_t comTxBufferHex[COM_CMD_MAX_SIZE/2];
uint8_t comTxBufferAscii[COM_CMD_MAX_SIZE];
uint8_t comTxBufferIndex = 0;

/*! 
*  \brief  发送一个字节
*  \param  c 
*/
void COM_SEND_DATA(uint8 c)
{
    comTxBufferHex[comTxBufferIndex++] = c;   
}
/*! 
*  \brief  帧头
*/
void COM_BEGIN_CMD(void)
{
    comTxBufferIndex = 0;
}
/*! 
*  \brief  帧尾
*/
void COM_END_CMD(void)
{
#if(CRC16_ENABLE)
    uint16 crc = 0xFFFF;
	AddCRC16(comTxBufferHex, comTxBufferIndex, &crc);
    comTxBufferHex[comTxBufferIndex++] = (uint8)(crc >> 8);
    comTxBufferHex[comTxBufferIndex++] = (uint8)crc;
#endif

    Hex2Ascii(comTxBufferAscii+1, comTxBufferHex, comTxBufferIndex);
    comTxBufferIndex *= 2;
    
    comTxBufferAscii[0] = SYSTEM_COMMUNICATE_HEAD;
    comTxBufferIndex++;
    comTxBufferAscii[comTxBufferIndex++] = SYSTEM_COMMUNICATE_TAIL;
    
    if(man.comType == 0) //串口
    {
        uint8 i;
        for(i=0;i<comTxBufferIndex;i++)
            ComUart_SendChar(comTxBufferAscii[i]);
    }
    else
    {
        if(S0_State&S_CONN)//socket已连接
            Write_SOCK_Data_Buffer(0, comTxBufferAscii, comTxBufferIndex);
    }
    
    comTxBufferIndex = 0;
}

void Com_SendStringsRaw(uchar *str)
{
    //COM_BEGIN_CMD();
    while(*str)
    {
        COM_TX_8(*str);
        str++;
    }
    COM_TX_8(*str);//发送字符串结尾符
    //COM_END_CMD();
}

/*! 
*  \brief  串口发送送字符串
*  \param  字符串
*/
void Com_SendStrings(uchar *str)
{
    COM_BEGIN_CMD();
    while(*str)
    {
        COM_TX_8(*str);
        str++;
    }
    //COM_TX_8(*str);//发送字符串结尾符
    COM_END_CMD();
}
/*! 
*  \brief  串口发送送N个字节
*  \param  个数
*/
void Com_SendNU8(uint8 *pData,uint16 nDataLen)
{
    uint16 i = 0;
    COM_BEGIN_CMD();
    for (;i<nDataLen;++i)
    {
        COM_TX_8(pData[i]);
    }
    COM_END_CMD();
}
/*! 
*  \brief  串口发送送N个16位的数据
*  \param  个数
*/
void Com_SendNU16(uint16 *pData,uint16 nDataLen)
{
    uint16 i = 0;
    COM_BEGIN_CMD();
    for (;i<nDataLen;++i)
    {
        COM_TX_16(pData[i]);
    }
    COM_END_CMD();
}


//回复指令======================================================
void Com_Ack(void)
{
    comTxBufferHex[0] = SYSTEM_COMMUNICATE_ACK;
    
    if(man.comType == 0) //串口
    {
        ComUart_SendChar(comTxBufferHex[0]);
    }
    else
    {
        Write_SOCK_Data_Buffer(0, comTxBufferHex, 1);
    }
    
    comTxBufferIndex = 0;
}

void Com_NAck(void)
{
    comTxBufferHex[0] = SYSTEM_COMMUNICATE_NACK;
    
    if(man.comType == 0) //串口
    {
        ComUart_SendChar(comTxBufferHex[0]);
    }
    else
    {
        Write_SOCK_Data_Buffer(0, comTxBufferHex, 1);
    }
    
    comTxBufferIndex = 0;
}


//基本指令======================================================
//通用基本协议====================================

void Com_SetSoftwareVersion(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_SOFTWARE_VERSION|0x80);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SetHardwareVersion(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_HARDWARE_VERSION|0x80);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SetProductDate(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_PRODUCT_DATE|0x80);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SetSerialNumber(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_SERIAL_NUMBER|0x80);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SetModel(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_MODEL|0x80);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SetSyncTime(uint32_t time)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_SYNC_TIME|0x80);
    COM_TX_32(time);
    COM_END_CMD();
}

void Com_SetTotalUsedTime(uint32_t time)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_USED_TIME|0x80);
    COM_TX_32(time);
    COM_END_CMD();
}

void Com_SetInput(uint8_t inputNumber, uint8_t level)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_INPUT|0x80);
    COM_TX_8(inputNumber);
    COM_TX_8(level);
    COM_END_CMD();
}

void Com_SetOutput(uint8_t outputNumber, uint8_t level)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_OUTPUT|0x80);
    COM_TX_8(outputNumber);
    COM_TX_8(level);
    COM_END_CMD();
}

void Com_SetTick(void)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_TICK|0x80);
    COM_END_CMD();
}

void Com_SetSystemError(uint8_t errorNum)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_ERROR|0x80);
    COM_TX_8(errorNum);
    COM_END_CMD();
}


//检基本协议====================================
void Com_SetWave(uint16_t wave)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_WAVE|0x80);
    COM_TX_16(wave);
    COM_END_CMD();
}

void Com_SetWave2(uint16_t wave)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_WAVE2|0x80);
    COM_TX_16(wave);
    COM_END_CMD();
}

void Com_SetTimeConst(uint8_t timeConstIndex)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_TIMECONST|0x80);
    COM_TX_8(timeConstIndex);
    COM_END_CMD();
}

void Com_SetRange(uint8_t range)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_RANGE|0x80);
    COM_TX_8(range);
    COM_END_CMD();
}

void Com_SetLampType(uint8_t lampType)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_LAMP_TYPE|0x80);
    COM_TX_8(lampType);
    COM_END_CMD();
}

void Com_SetChannel(uint8_t channel)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_CHANNEL|0x80);
    COM_TX_8(channel);
    COM_END_CMD();
}

void Com_SetLamp(uint8_t flag)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_LAMP|0x80);
    COM_TX_8(flag);
    COM_END_CMD();
}

void Com_SetHome(uint8_t flag)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_HOME|0x80);
    COM_TX_8(flag);
    COM_END_CMD();
}

void Com_SetAuUploadFreq(uint8_t factor)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_AU_UPLOAD_FREQ|0x80);
    COM_TX_8(factor);
    COM_END_CMD();
}

void Com_SetAu(int32_t au, int32_t au2)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_AU|0x80);
    COM_TX_32(au);
    COM_TX_32(au2);
    COM_END_CMD();
}

void Com_SetWaveRefSam(uint32_t ref, uint32_t sam)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_WAVE_REF_SAM|0x80);
    COM_TX_32(ref);
    COM_TX_32(sam);
    COM_END_CMD();
}

void Com_SetWave2RefSam(uint32_t ref, uint32_t sam)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_WAVE2_REF_SAM|0x80);
    COM_TX_32(ref);
    COM_TX_32(sam);
    COM_END_CMD();
}

void Com_SetScanCMD(uint8 cmd)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_SCAN_CMD|0x80);
    COM_TX_8(cmd);
    COM_END_CMD();
}
void Com_SetScanAu(uint16_t wave, int32_t au)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_SCAN_AU|0x80);
    COM_TX_16(wave);
    COM_TX_32(au);
    COM_END_CMD();
}

void Com_SetScanPara(uint16_t waveStart, uint16_t waveStop, uint8_t speed)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_SCAN_PARA|0x80);
    COM_TX_16(waveStart);
    COM_TX_16(waveStop);
    COM_TX_8(speed);
    COM_END_CMD();
}

void Com_SetLampUsedTime(uint32_t time)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_LAMP_USED_TIME|0x80);
    COM_TX_32(time);
    COM_END_CMD();
}

void Com_SetStartStop(uint8_t flag)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_START_STOP|0x80);
    COM_TX_32(flag);
    COM_END_CMD();
}
