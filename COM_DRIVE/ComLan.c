#include "management.h"
#include "ComLAN.h"
#include <string.h>
#include "ComQueue.h"

#include "CPrintf.h"

/*******************************************************************************
* 函数名  : Load_Net_Parameters
* 描述    : 装载网络参数
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 网关、掩码、物理地址、本机IP地址、端口号、目的IP地址、目的端口号、端口工作模式
*******************************************************************************/
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = man.localIP[0];//加载网关参数
	Gateway_IP[1] = man.localIP[1];
	Gateway_IP[2] = man.localIP[2];
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//加载子网掩码
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

    {
        uint32_t mac = man.chipID[0]^man.chipID[1]^man.chipID[2];
        Phy_Addr[0]=(uint8_t)((mac&0x000000FF)>>0);//加载物理地址
        Phy_Addr[1]=(uint8_t)((mac&0x0000FF00)>>8);
        Phy_Addr[2]=(uint8_t)((mac&0x00FF0000)>>16);
        Phy_Addr[3]=(uint8_t)((mac&0xFF000000)>>24);
        Phy_Addr[4]=0x00;
        Phy_Addr[5]=0x02;
        
        cDebug("MAC Address:%02X %02X %02X %02X %02X %02X\r\n", 
            Phy_Addr[0], Phy_Addr[1], Phy_Addr[2], Phy_Addr[3], Phy_Addr[4], Phy_Addr[5]);
    }
//	Phy_Addr[0]=0x0c;//加载物理地址
//	Phy_Addr[1]=0x29;
//	Phy_Addr[2]=0xab;
//	Phy_Addr[3]=0x7c;
//	Phy_Addr[4]=0x00;
//	Phy_Addr[5]=0x01;

	IP_Addr[0]=man.localIP[0];//加载本机IP地址
	IP_Addr[1]=man.localIP[1];
	IP_Addr[2]=man.localIP[2];
	IP_Addr[3]=man.localIP[3];

	S0_Port[0] = (uint8_t)((man.localPort&0xFF00)>>8);//高位在前
	S0_Port[1] = (uint8_t)((man.localPort&0x00FF)>>0);

    //S0_Mode=UDP_MODE;//加载端口0的工作模式,UDP模式
    S0_Mode=TCP_SERVER;//加载端口0的工作模式,TCP服务器模式
    //S0_Mode=TCP_CLIENT;//加载端口0的工作模式,TCP客户端模式
 
    
    if(S0_Mode == TCP_CLIENT)
    {
        S0_DIP[0]=man.remoteIP[0];//加载端口0的目的IP地址
        S0_DIP[1]=man.remoteIP[1];
        S0_DIP[2]=man.remoteIP[2];
        S0_DIP[3]=man.remoteIP[3];
        
        S0_DPort[0] = (uint8_t)((man.remotePort&0xFF00)>>8);//高位在前
        S0_DPort[1] = (uint8_t)((man.remotePort&0x00FF)>>0);
    }



    //	S0_DIP[0]=192;//加载端口0的目的IP地址
    //	S0_DIP[1]=168;
    //	S0_DIP[2]=1;
    //	S0_DIP[3]=190;
    //	
    //	S0_DPort[0] = 0x17;//加载端口0的目的端口号6000
    //	S0_DPort[1] = 0x70;

    UDP_DIPR[0] = man.remoteIP[0];	//UDP(广播)模式,目的主机IP地址
	UDP_DIPR[1] = man.remoteIP[1];
	UDP_DIPR[2] = man.remoteIP[2];
	UDP_DIPR[3] = man.remoteIP[3];

	UDP_DPORT[0] = (uint8_t)((man.remotePort&0xFF00)>>8);	//UDP(广播)模式,目的主机端口号
	UDP_DPORT[1] = (uint8_t)((man.remotePort&0x00FF)>>0);


    //	UDP_DIPR[0] = 192;	//UDP(广播)模式,目的主机IP地址
    //	UDP_DIPR[1] = 168;
    //	UDP_DIPR[2] = 1;
    //	UDP_DIPR[3] = 190;
    //
    //	UDP_DPORT[0] = 0x17;	//UDP(广播)模式,目的主机端口号
    //	UDP_DPORT[1] = 0x70;
}

/*******************************************************************************
* 函数名  : W5500_Socket_Set
* 描述    : W5500端口初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
*			从端口状态字节Socket_State可以判断端口的工作情况
*******************************************************************************/
void W5500_Socket_Set(void)
{
	if(S0_State==0)//端口0初始化配置
	{
		if(S0_Mode==TCP_SERVER)//TCP服务器模式 
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP客户端模式 
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else//UDP模式 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}

/*******************************************************************************
* 函数名  : Process_Socket_Data
* 描述    : W5500接收并发送接收到的数据
* 输入    : s:端口号
* 输出    : 无
* 返回值  : 无
* 说明    : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
*			然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
*			处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
*			发送数据。
*******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
	unsigned short size, i;
	size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
	
    if(S0_Mode==UDP_MODE)
    {
        UDP_DIPR[0] = Rx_Buffer[0];
        UDP_DIPR[1] = Rx_Buffer[1];
        UDP_DIPR[2] = Rx_Buffer[2];
        UDP_DIPR[3] = Rx_Buffer[3];

        UDP_DPORT[0] = Rx_Buffer[4];
        UDP_DPORT[1] = Rx_Buffer[5];
        
        //cDebug("remoteIP is %d.%d.%d.%d:%d\r\n", UDP_DIPR[0], UDP_DIPR[1], UDP_DIPR[2], UDP_DIPR[3], UDP_DPORT[0]*256+UDP_DPORT[1]);
        
        size -= 8;
        
    //	memcpy(Tx_Buffer, Rx_Buffer+8, size);			
    //	Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
        for(i=0;i<size;i++)
            queue_push(&comQueue, Rx_Buffer[8+i]);
    }
    else
    {
        for(i=0;i<size;i++)
            queue_push(&comQueue, Rx_Buffer[i]);
    }
}

/*******************************************************************************
* 函数名  : ComLAN_Init
* 描述    : W5500初始货配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void ComLan_Init(void)
{
    cDebug("ComLan_Init()...\r\n");
    
    SPI_Configuration();		//W5500 SPI初始化配置(STM32 SPI2)
	W5500_GPIO_Configuration();	//W5500 GPIO初始化配置
    
    Load_Net_Parameters();		//装载网络参数	
	W5500_Hardware_Reset();		//硬件复位W5500
    
	W5500_Init();		//初始化W5500寄存器函数
	Detect_Gateway();	//检查网关服务器 
	Socket_Init(0);		//指定Socket(0~7)初始化,初始化端口0
    
    cDebug("ComLan_Init()...Finish\r\n");
}

void ComLan_Loop(void)
{
    W5500_Socket_Set();//W5500端口初始化配置

    if(W5500_Interrupt)//处理W5500中断		
    {
        W5500_Interrupt_Process();//W5500中断处理程序框架
    }
    if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
    {
        S0_Data&=~S_RECEIVE;
        Process_Socket_Data(0);//W5500接收并发送接收到的数据
    }
}

