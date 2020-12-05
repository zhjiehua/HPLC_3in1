#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FIRMWARE_VERSION 1.0.0          //
    
#define CRC16_ENABLE 1            // 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)
#define COM_CMD_MAX_SIZE 64           // 单条指令大小，根据需要调整，尽量设置大一些
    
#define COM_QUEUE_MAX_SIZE 1024//512        // 指令接收缓冲区大小，根据需要调整，尽量设置大一些   

#ifdef __cplusplus
}
#endif

#endif
