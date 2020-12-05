/*! 
*  \file hmi_driver.h
*  \brief 指令分类处理
*  \version 1.0
*  \date 2012-2018
*  \copyright 广州大彩光电科技有限公司
*/


#ifndef _COM_PROCESS_H_
#define _COM_PROCESS_H_

#include "ComDriver.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "ComQueue.h"
#include "ComUart.h"
#include "usart.h"

#define SYSTEM_COMMUNICATE_HEAD ':'//0x3A       //帧头
#define SYSTEM_COMMUNICATE_TAIL '!'//0x21       //帧尾
#define SYSTEM_COMMUNICATE_ACK '#' //0x23
#define SYSTEM_COMMUNICATE_NACK '$' //0x24

//基本通信帧功能码*******************************************
//通用功能码*************************************************
#define PFC_GLOBAL_DEVICE_ADDRESS                   0X00  //设备地址
#define PFC_GLOBAL_SOFTWARE_VERSION                 0X01  //软件版本
#define PFC_GLOBAL_HARDWARE_VERSION                 0X02  //硬件版本
#define PFC_GLOBAL_PRODUCT_DATE                     0X03  //出厂日期
#define PFC_GLOBAL_SERIAL_NUMBER                    0X04  //序列号
#define PFC_GLOBAL_MODEL                            0X05  //型号
#define PFC_GLOBAL_USED_TIME                        0X06  //总使用时间
#define PFC_GLOBAL_SYNC_TIME                        0X07  //同步时间
#define PFC_GLOBAL_INPUT                            0X08  //输入点
#define PFC_GLOBAL_OUTPUT                           0X09  //输出点
#define PFC_GLOBAL_TICK								              0X0A  //心跳包
//
#define PFC_GLOBAL_ERROR							              0X2D  //系统错误
#define PFC_GLOBAL_EXT_SYSTEM                       0X2E  //通用系统参数
#define PFC_GLOBAL_EXT_USER                         0X2F  //通用用户自定义参数

//检测器功能码***********************************************
#define PFC_CHECK_WAVE                              0X30  //波长
#define PFC_CHECK_WAVE2                             0X31  //波长2
#define PFC_CHECK_TIMECONST                         0X32  //时间常数
#define PFC_CHECK_RANGE                             0X33  //量程
#define PFC_CHECK_LAMP_TYPE                         0X34  //灯类型
#define PFC_CHECK_CHANNEL                           0X35  //通道
#define PFC_CHECK_LAMP                              0X36  //灯状态
#define PFC_CHECK_ZERO                              0X37  //基线归零
#define PFC_CHECK_HOME                              0X38  //回零点
#define PFC_CHECK_AU_UPLOAD_FREQ                    0X39  //AU值上传频率
#define PFC_CHECK_AU                                0X3A  //AU值
#define PFC_CHECK_WAVE_REF_SAM                      0X3B  //参比-样本值
#define PFC_CHECK_WAVE2_REF_SAM                     0X3C  //波长2参比-样本值
#define PFC_CHECK_SCAN_CMD                          0X3D  //波长扫描使能
#define PFC_CHECK_SCAN_AU                           0X3E  //波长扫描AU值
#define PFC_CHECK_SCAN_PARA                         0X3F  //波长扫描参数
#define PFC_CHECK_LAMP_USED_TIME                    0X40  //灯源使用时间
#define PFC_CHECK_START_STOP					              0X41  //启动/停止采集
//
#define PFC_CHECK_EXT_SYSTEM                        0X4E  //检测器系统参数
#define PFC_CHECK_EXT_USER                          0X4F  //检测器用户自定义参数

//输液泵功能码********************************************
#define PFC_PUMP_FLOW                               0X50  //流速
#define PFC_PUMP_FLOW_PERCENT                       0X51  //流速百分比
#define PFC_PUMP_PRESS_MIN                          0X52  //最小压力
#define PFC_PUMP_PRESS_MAX                          0X53  //最大压力
#define PFC_PUMP_PRESS_WARNING                      0X54  //预警压力
#define PFC_PUMP_START_STOP                         0X55  //启动/停止泵
#define PFC_PUMP_PAUSE_RESTORE                      0X56  //暂停
#define PFC_PUMP_PURGE                              0X57  //清洗
#define PFC_PUMP_PURGE_FLOW                         0X58  //清洗流速
#define PFC_PUMP_PURGE_TIME                         0X59  //清洗时间
#define PFC_PUMP_PRESS_ZERO                         0X5A  //压力清零,压力传感器电压归零
#define PFC_PUMP_PRESS_UPLOAD_FREQ                  0X5B  //压力上传频率
#define PFC_PUMP_FLOW_PRESS_COMPEN                  0X5C  //压力补偿
#define PFC_PUMP_MODE                               0X5D  //泵模式
#define PFC_PUMP_PRESS                              0X5E  //压力
//
#define PFC_PUMP_EXT_SYSTEM						              0X6E  //输液泵系统参数
#define PFC_PUMP_EXT_USER						                0X6F  //输液泵用户自定义参数

//系统扩展功能码*********************************************
//通用功能码*************************************************
#define PFC_GES_PRIORITY							              0X00  //权限级数
#define PFC_GES_RESET								                0X01  //重启
#define PFC_GES_RESTORE_DEFAULT						          0X02  //恢复出厂设置
#define PFC_GES_SAVE_AS_DEFAULT						          0X03  //保存为出厂设置
#define PFC_GES_SUPERPASSWORD						            0X04  //管理员密码
#define PFC_GES_SOFTWARE_VERSION                    0X05  //软件版本
#define PFC_GES_HARDWARE_VERSION                    0X06  //硬件版本
#define PFC_GES_PRODUCT_DATE                        0X07  //出厂日期
#define PFC_GES_SERIAL_NUMBER                       0X08  //序列号
#define PFC_GES_MODEL                               0X09  //型号
#define PFC_GES_MODEL_STRING                        0X0A  //型号字符串
#define PFC_GES_USED_TIME                           0X0B  //总使用时间
#define PFC_GES_ALL_DATA                            0X0C  //全部数据

//检测器功能码***********************************************
#define PFC_CES_COIL								                0X00  //电磁铁吸合/松开
#define PFC_CES_TIME_INTEGRAL						            0X01  //积分时间
#define PFC_CES_RANGE_INTEGRAL						          0X02  //积分量程
#define PFC_CES_AU_FACTOR						                0X03  //标定系数
#define PFC_CES_RASTER_STEP							            0X04  //光栅转动指定步数
#define PFC_CES_HOME_OFFSET							            0X05  //零点偏移
#define PFC_CES_WAVE_OFFSET							            0X06  //波长方向偏移
#define PFC_CES_WORKMODE						                0X07  //工作模式
#define PFC_CES_WAVECALIB_DATA						          0X08  //波长校正数据
#define PFC_CES_WAVECALIB_DATA_CMD					        0X09  //读写校正数据使能
#define PFC_CES_LAMP_USED_TIME                      0X0A  //灯源使用时间
#define PFC_CES_LAMP_ONOFF_TIMES                    0X0B  //灯源开关次数
#define PFC_CES_LAMP_COUNT                          0X0C  //换灯次数
#define PFC_CES_DAC_ZEROVOLTAGE                     0X0D  //DAC零点电压
#define PFC_CES_UPLOADOBJECT                        0X0E  //上传对象

//输液泵功能码********************************************
#define PFC_PES_WORKMODE							              0X00  //工作模式
#define PFC_PES_FLOWCALIB_DATA_CMD					        0X01  //写流速校正数据使能
#define PFC_PES_FLOWCALIB_DATA						          0X02  //流速校正数据
#define PFC_PES_FLOWCALIB_LOAD_FLOW					        0X03  //流速校正加载流速
#define PFC_PES_FLOWCALIB_LOAD_REALFLOW				      0X04  //流速校正加载实际流速
#define PFC_PES_FLOWCALIB_LOAD_PRESS				        0X05  //流速校正加载压力
#define PFC_PES_PRESSCALIB_DATA_CMD					        0X06  //写压力校正数据使能
#define PFC_PES_PRESSCALIB_DATA						          0X07  //压力校正参数
#define PFC_PES_PULSECOMPEN_DATA_CMD				        0X08  //写脉动补偿数据使能
#define PFC_PES_PULSECOMPEN_DATA					          0X09  //脉动补偿数据
#define PFC_PES_PULSECOMPEN_DEBUG_CMD				        0X0A  //脉动补偿调试使能
#define PFC_PES_PULSECOMPEN_GEAR   					        0X0B  //脉动补偿凸轮感应到
#define PFC_PES_PULSECOMPEN_GEAR_POS				        0X0C  //脉动补偿凸轮位置


//系统错误码********************************************
//通用错误码*************************************************

//检测器错误码*************************************************
#define PEC_CHECK_REF_LOW                           0x10  //检参比值能量低
#define PEC_CHECK_SAM_LOW                           0x11  //检样本值能量低
#define PEC_CHECK_REF2_LOW                          0x12  //检参比2值能量低
#define PEC_CHECK_SAM2_LOW                          0x13  //检样本2值能量低
#define PEC_CHECK_REF_OVER                          0x14  //检参比值能量溢出
#define PEC_CHECK_SAM_OVER                          0x15  //检样本值能量溢出
#define PEC_CHECK_REF2_OVER                         0x16  //检参比2值能量溢出
#define PEC_CHECK_SAM2_OVER                         0x17  //检样本2值能量溢出

//输液泵错误码*************************************************
#define PEC_PUMP_STOP                               0x10  //泵主动停止
#define PEC_PUMP_RUNNING                            0x11  //泵正在运行，控制权在设备端
#define PEC_PUMP_PRESS_LOW                          0x12  //泵低压报警
#define PEC_PUMP_PRESS_HIGH                         0x13  //泵超压报警


#define PTR2U16(PTR) ((((uint8 *)(PTR))[0]<<8)|((uint8 *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8 *)(PTR))[0]<<24)|(((uint8 *)(PTR))[1]<<16)|(((uint8 *)(PTR))[2]<<8)|((uint8 *)(PTR))[3])  //从缓冲区取32位数据

#pragma pack(push)
#pragma pack(1)                           //按字节对齐

typedef struct
{
    uint8    address;                    //目的地址  
    uint8    pfc;                        //功能码
    uint8    data[20];                   //产生消息的画面ID
}ComFrame_TypeDef;

#pragma pack(pop)


void ComProcessFrame( ComFrame_TypeDef *msg, uint16 size );
void ComProcessFrame_WriteGlobalExternSystem( ComFrame_TypeDef *msg, uint16 size );
void ComProcessFrame_ReadGlobalExternSystem( ComFrame_TypeDef *msg, uint16 size );
void ComProcessFrame_WriteCheckExternSystem( ComFrame_TypeDef *msg, uint16 size );
void ComProcessFrame_ReadCheckExternSystem( ComFrame_TypeDef *msg, uint16 size );

#endif
