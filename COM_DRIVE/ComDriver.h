/*! 
*  \file hmi_driver.h
*  \brief �����������ļ�
*  \version 1.0
*  \date 2012-2018
*  \copyright ���ݴ�ʹ��Ƽ����޹�˾
*/

#ifndef _COM_DRIVER_
#define _COM_DRIVER_


#include "ComUart.h"
#include "miscellaneous.h"
#include "config.h"

extern uint8_t comTxBufferHex[COM_CMD_MAX_SIZE/2];
extern uint8_t comTxBufferAscii[COM_CMD_MAX_SIZE];
extern uint8_t comTxBufferIndex;

#define COM_TX_8(P1) COM_SEND_DATA((P1)&0xFF)                    //���͵����ֽ�
#define COM_TX_8N(P,N) Com_SendNU8((uint8 *)P,N)                 //����N���ֽ�
#define COM_TX_16(P1) COM_TX_8((P1)>>8);COM_TX_8(P1)                 //����16λ����
#define COM_TX_16N(P,N) Com_SendNU16((uint16 *)P,N)              //����N��16λ����
#define COM_TX_32(P1) COM_TX_16((P1)>>16);COM_TX_16((P1)&0xFFFF)     //����32λ����
#define COM_TX_FLOAT(P1) COM_TX_32(*(uint32*)(&P1))     //���͸�����

void COM_SEND_DATA(uint8 c);
void COM_BEGIN_CMD(void);
void COM_END_CMD(void);
void Com_SendStringsRaw(uchar *str);
void Com_SendStrings(uchar *str);
void Com_SendNU8(uint8 *pData,uint16 nDataLen);
void Com_SendNU16(uint16 *pData,uint16 nDataLen);


//����ָ��========================================
void Com_Ack(void);
void Com_NAck(void);

void Com_SetSoftwareVersion(uint8_t *str);
void Com_SetHardwareVersion(uint8_t *str);
void Com_SetProductDate(uint8_t *str);
void Com_SetSerialNumber(uint8_t *str);
void Com_SetModel(uint8_t *str);
void Com_SetSyncTime(uint32_t time);
void Com_SetTotalUsedTime(uint32_t time);
void Com_SetInput(uint8_t inputNumber, uint8_t level);
void Com_SetOutput(uint8_t inputNumber, uint8_t level);
void Com_SetTick(void);
void Com_SetSystemError(uint8_t errorNum);

//�����ָ��
void Com_SetWave(uint16_t wave);
void Com_SetWave2(uint16_t wave);
void Com_SetTimeConst(uint8_t timeConstIndex);
void Com_SetRange(uint8_t range);
void Com_SetLampType(uint8_t lampType);
void Com_SetChannel(uint8_t channel);
void Com_SetLamp(uint8_t flag);
void Com_SetHome(uint8_t flag);
void Com_SetAuUploadFreq(uint8_t factor);
void Com_SetAu(int32_t au, int32_t au2);
void Com_SetWaveRefSam(uint32_t ref, uint32_t sam);
void Com_SetWave2RefSam(uint32_t ref, uint32_t sam);
void Com_SetScanCMD(uint8_t cmd);
void Com_SetScanAu(uint16_t wave, int32_t au);
void Com_SetScanPara(uint16_t waveStart, uint16_t waveStop, uint8_t speed);
void Com_SetLampUsedTime(uint32_t time);
void Com_SetStartStop(uint8_t flag);

//ϵͳ��չ=============================================
void Com_SystemExt_SetRandomCode(uint32_t code);
void Com_SystemExt_SetSuperPassword(uint8_t *str);
void Com_SystemExt_SetSaveAsDefault(void);
void Com_SystemExt_SetRestoreDefault(void);
void Com_SystemExt_SetSoftwareVersion(uint8_t *str);//
void Com_SystemExt_SetHardwareVersion(uint8_t *str);//
void Com_SystemExt_SetProductDate(uint8_t *str);//
void Com_SystemExt_SetSerialNumber(uint8_t *str);//
void Com_SystemExt_SetModel(uint8_t model);
void Com_SystemExt_SetModelString(uint8_t *str);//
void Com_SystemExt_SetTotalUsedTime(uint32_t time);
void Com_SystemExt_SetSaveAllData(void);

//����չָ��
void Com_SystemExt_SetCoil(uint8_t flag);
void Com_SystemExt_SetTimeIntegral(uint16_t timeInte);
void Com_SystemExt_SetRangeIntegral(uint8_t rangeInte);
void Com_SystemExt_SetAuFactor(float factor);
void Com_SystemExt_SetHomeOffset(int32 offset);
void Com_SystemExt_SetWaveOffset(uint32 offset);
void Com_SystemExt_SetWorkMode(uint8_t workMode);
void Com_SystemExt_SetWaveCalibDataCMD(uint8 cmd);
void Com_SystemExt_SetWaveCalibData(float wave, float realWave);
void Com_SystemExt_SetLampUsedTime(uint32_t time);
void Com_SystemExt_SetLampOnOffTimes(uint16_t times);
void Com_SystemExt_SetLampCount(uint8_t count);
void Com_SystemExt_SetDACZeroVoltage(float voltage);
void Com_SystemExt_SetUploadObject(uint8_t object);

#endif      //_HMI_DRIVER_
