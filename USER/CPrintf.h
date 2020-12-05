#ifndef _CPRINTF_H_
#define _CPRINTF_H_

#include "stdio.h"
#include "management.h"
#include "W5500.h"

//如果需要在上位机模拟STM32芯片功能，需要添加这个宏，否则需要注释这个宏
//#define STM32SIM

//如果需要在上位机调试STM32芯片功能（只有板，无电机和传感器等），需要添加这个宏，否则需要注释这个宏
//#define STM32DEBUG

//上位机和串口屏之间转发，主要用于调试串口屏，这时调试串口使用不了
#define USED_PC_X_LCD 0

//使用虚拟串口屏，即使用PC端的串口屏，这时调试串口改成LCD串口
#define USED_VIRTUALLCD	0 


#define _CDebug

#ifdef _CDebug
	#ifdef __cplusplus
		void MyCPrintf(const char *fmt, ...);
		#define cDebug(format, ...)     MyCPrintf(format, ##__VA_ARGS__)
		//#define cDebug(format, args...)    printf(format, ##args)
	#else
    
		#define cDebug printf
        
//#define cDebug(format, ...)    do{\
//                                if(man.comType){\
//                                    sprintf((char*)(man.cDebugBuffer), format, ##__VA_ARGS__);\
//                                    Write_SOCK_Data_Buffer(0, man.cDebugBuffer, strlen((const char*)man.cDebugBuffer));}\
//                                else{\
//                                    printf(format, ...);}\
//                                }while(0)
                                
        
	#endif
#else
	#ifdef __cplusplus
		#define cDebug(format, ...)
	#else
		#define cDebug
	#endif
#endif







#define USER_DEBUG_LEVEL 3
//#define USER_LOG_FILENAME	"log.txt"

#if (USER_DEBUG_LEVEL > 0)
#define USER_UsrLog(...)    cDebug(__VA_ARGS__);\
                            cDebug("\r\n");
#else
#define USER_UsrLog(...)
#endif

#if (USER_DEBUG_LEVEL > 1)

#define USER_ErrLog(...)    cDebug("ERROR: ");\
                            cDebug(__VA_ARGS__);\
                            cDebug("\r\n");
#else
#define USER_ErrLog(...)
#endif

#if (USER_DEBUG_LEVEL > 2)
#define USER_DbgLog(...)    cDebug("DEBUG : ");\
                            cDebug(__VA_ARGS__);\
                            cDebug("\r\n");
#else
#define USER_DbgLog(...)
#endif

//#if (USER_DEBUG_LEVEL > 0)
//#define USER_UsrLog(format, ...)    f_chdir((const TCHAR*)"0:/");\
//                            f_open(&(pProjectMan->file), (const TCHAR*)USER_LOG_FILENAME, FA_OPEN_ALWAYS|FA_WRITE);\
//														f_lseek(&(pProjectMan->file), pProjectMan->file.fsize);\
//														sprintf((char*)(pProjectMan->fsBuffer), "%s", "UsrLog:");\
//														sprintf((char*)(pProjectMan->fsBuffer+7), format, ##__VA_ARGS__);\
//														f_write(&(pProjectMan->file), pProjectMan->fsBuffer, strlen((char*)pProjectMan->fsBuffer), &pProjectMan->br);\
//														f_close(&(pProjectMan->file));
//#else
//#define USER_UsrLog(format, ...)
//#endif

//#if (USER_DEBUG_LEVEL > 1)

//#define USER_ErrLog(format, ...)    f_chdir((const TCHAR*)"0:/");\
//                            f_open(&(pProjectMan->file), (const TCHAR*)USER_LOG_FILENAME, FA_OPEN_ALWAYS|FA_WRITE);\
//														f_lseek(&(pProjectMan->file), pProjectMan->file.fsize);\
//														sprintf((char*)(pProjectMan->fsBuffer), "%s", "ErrorLog:");\
//														sprintf((char*)(pProjectMan->fsBuffer+9), format, ##__VA_ARGS__);\
//														f_write(&(pProjectMan->file), pProjectMan->fsBuffer, strlen((char*)pProjectMan->fsBuffer), &pProjectMan->br);\
//														f_close(&(pProjectMan->file));
//#else
//#define USER_ErrLog(format, ...)
//#endif

//#if (USER_DEBUG_LEVEL > 2)
//#define USER_DbgLog(format, ...)    f_chdir((const TCHAR*)"0:/");\
//                            f_open(&(pProjectMan->file), (const TCHAR*)USER_LOG_FILENAME, FA_OPEN_ALWAYS|FA_WRITE);\
//														f_lseek(&(pProjectMan->file), pProjectMan->file.fsize);\
//														sprintf((char*)(pProjectMan->fsBuffer), "%s", "DebugLog:");\
//														sprintf((char*)(pProjectMan->fsBuffer+9), format, ##__VA_ARGS__);\
//														f_write(&(pProjectMan->file), pProjectMan->fsBuffer, strlen((char*)pProjectMan->fsBuffer), &pProjectMan->br);\
//														f_close(&(pProjectMan->file));
//#else
//#define USER_DbgLog(format, ...)
//#endif

#endif
