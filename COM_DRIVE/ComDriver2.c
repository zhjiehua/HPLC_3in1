#include "ComDriver.h"
#include "ComUart.h"
#include "ComProcess.h"
#include "management.h"
#include "miscellaneous.h"
#include "ComLAN.h"

//系统扩展指令=======================================
void Com_SystemExt_SetRandomCode(uint32_t code)
{
    uint8 *temp;
    
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_PRIORITY);
    temp = (uint8*)(&code);
    COM_TX_8(temp[3]);
    COM_TX_8(temp[2]);
    COM_TX_8(temp[1]);
    COM_TX_8(temp[0]);
    COM_END_CMD();
}

void Com_SystemExt_SetSaveAsDefault(void)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_SAVE_AS_DEFAULT);
    COM_END_CMD();
}

void Com_SystemExt_SetRestoreDefault(void)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_RESTORE_DEFAULT);
    COM_END_CMD();
}

void Com_SystemExt_SetSuperPassword(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_SUPERPASSWORD);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SystemExt_SetSoftwareVersion(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_SOFTWARE_VERSION);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SystemExt_SetHardwareVersion(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_HARDWARE_VERSION);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SystemExt_SetProductDate(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_PRODUCT_DATE);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SystemExt_SetSerialNumber(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_SERIAL_NUMBER);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}


void Com_SystemExt_SetModel(uint8_t model)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_MODEL);
    COM_TX_8(model);
    COM_END_CMD();
}

void Com_SystemExt_SetModelString(uint8_t *str)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);  
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_MODEL_STRING);
    Com_SendStringsRaw(str);
    COM_END_CMD();
}

void Com_SystemExt_SetTotalUsedTime(uint32_t time)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_USED_TIME);
    COM_TX_32(time);
    COM_END_CMD();
}

void Com_SystemExt_SetSaveAllData(void)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_GLOBAL_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_GES_ALL_DATA);
    COM_END_CMD();
}

//检扩展指令=================================================
void Com_SystemExt_SetCoil(uint8_t flag)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_COIL);
    COM_TX_8(flag);
    COM_END_CMD();
}

void Com_SystemExt_SetTimeIntegral(uint16_t timeInte)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_TIME_INTEGRAL);
    COM_TX_16(timeInte);
    COM_END_CMD();
}

void Com_SystemExt_SetRangeIntegral(uint8_t rangeInte)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_RANGE_INTEGRAL);
    COM_TX_8(rangeInte);
    COM_END_CMD();
}

void Com_SystemExt_SetAuFactor(float factor)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_AU_FACTOR);
    COM_TX_FLOAT(factor);
    COM_END_CMD();
}

void Com_SystemExt_SetHomeOffset(int32 offset)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_HOME_OFFSET);
    COM_TX_32(offset);
    COM_END_CMD();
}
void Com_SystemExt_SetWaveOffset(uint32 offset)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_WAVE_OFFSET);
    COM_TX_32(offset);
    COM_END_CMD();
}

void Com_SystemExt_SetWorkMode(uint8_t workMode)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_WORKMODE);
    COM_TX_8(workMode);
    COM_END_CMD();
}

void Com_SystemExt_SetWaveCalibDataCMD(uint8 cmd)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_WAVECALIB_DATA_CMD);
    COM_TX_8(cmd);
    COM_END_CMD();
}
void Com_SystemExt_SetWaveCalibData(float wave, float realWave)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_WAVECALIB_DATA);
    COM_TX_FLOAT(wave);
    COM_TX_FLOAT(realWave);
    COM_END_CMD();
}

void Com_SystemExt_SetLampUsedTime(uint32_t time)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_LAMP_USED_TIME);
    COM_TX_32(time);
    COM_END_CMD();
}

void Com_SystemExt_SetLampOnOffTimes(uint16_t times)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_LAMP_ONOFF_TIMES);
    COM_TX_16(times);
    COM_END_CMD();
}

void Com_SystemExt_SetLampCount(uint8_t count)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_LAMP_COUNT);
    COM_TX_8(count);
    COM_END_CMD();
}

void Com_SystemExt_SetDACZeroVoltage(float voltage)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_DAC_ZEROVOLTAGE);
    COM_TX_FLOAT(voltage);
    COM_END_CMD();
}

void Com_SystemExt_SetUploadObject(uint8_t object)
{
    COM_BEGIN_CMD();
    COM_TX_8(man.machineAddress);
    COM_TX_8(PFC_CHECK_EXT_SYSTEM|0x80);
    COM_TX_8(PFC_CES_UPLOADOBJECT);
    COM_TX_8(object);
    COM_END_CMD();
}
