/*! 
 *  \file hmi_driver.h
 *  \brief 串口屏驱动文件
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright 广州大彩光电科技有限公司
 */

#ifndef _DEVICE_DRIVER_
#define _DEVICE_DRIVER_

#define DEVICE_CMD_HEAD 0XEE  //帧头
#define DEVICE_CMD_TAIL 0XFFFCFFFF //帧尾

#include "Types.h"

void MsgTransmit(uint8_t device, void *msg, uint16 size);

void DetectorSendChar(uchar t);
void Detector_SetScreen(uint16 screen_id);
void Detector_SetButtonValue(uint16 screen_id,uint16 control_id,uchar value);
void Detector_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str);

void PumpASendChar(uchar t);
void PumpA_SetScreen(uint16 screen_id);
void PumpA_SetButtonValue(uint16 screen_id,uint16 control_id,uchar value);
void PumpA_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str);

void PumpBSendChar(uchar t);
void PumpB_SetScreen(uint16 screen_id);
void PumpB_SetButtonValue(uint16 screen_id,uint16 control_id,uchar value);
void PumpB_SetTextValue(uint16 screen_id,uint16 control_id,uchar *str);

#endif
