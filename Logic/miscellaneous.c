#include "miscellaneous.h"
#include "management.h"
#include "string.h"
#include "io.h"
#include "24cxx.h"
#include "CPrintf.h"
#include "ComProcess.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "hmi_user_uart.h"
#include "cmd_process.h"
#include "pageCommon.h"

#include "wdg.h"

/*! 
*  \brief  检查数据是否符合CRC16校验
*  \param buffer 待校验的数据
*  \param n 数据长度，包含CRC16
*  \param pcrc 校验码
*/
void AddCRC16(uint8_t *buffer,uint16_t n,uint16_t *pcrc)
{
    uint16_t i,j,carry_flag,a;

    for (i=0; i<n; i++)
    {
        *pcrc=*pcrc^buffer[i];
        for (j=0; j<8; j++)
        {
            a=*pcrc;
            carry_flag=a&0x0001;
            *pcrc=*pcrc>>1;
            if (carry_flag==1)
                *pcrc=*pcrc^0xa001;
        }
    }
}
/*! 
*  \brief  检查数据是否符合CRC16校验
*  \param buffer 待校验的数据，末尾存储CRC16
*  \param n 数据长度，包含CRC16
*  \return 校验通过返回1，否则返回0
*/
uint16_t CheckCRC16(uint8_t *buffer,uint16_t n)
{
    uint16_t crc0 = 0x0;
    uint16_t crc1 = 0xffff;

    if(n>=2)
    {
        crc0 = ((buffer[n-2]<<8)|buffer[n-1]);
        AddCRC16(buffer,n-2,&crc1);
    }

    return (crc0==crc1);
}

//len为HEX缓冲区长度，ASCII缓冲区长度为2*len+1，最后需要一个字符串结束符
void Hex2Ascii(uint8_t *outBuffer, uint8_t *inBuffer, uint8_t len)
{
	uint8_t i, temp;
	for(i=0;i<len;i++)
	{
		temp = (inBuffer[i]&0xF0)>>4;
		if(temp < 0x0A)
			outBuffer[2*i] = temp + 0x30;
		else
			outBuffer[2*i] = temp - 0x0A + 0x41;
		
		temp = inBuffer[i]&0x0F;
		if(temp < 0x0A)
			outBuffer[2*i+1] = temp + 0x30;
		else
			outBuffer[2*i+1] = temp - 0x0A + 0x41;
	}
	outBuffer[2*i] = '\0';
}

//长度len为HEX缓冲区的长度，也是ASCII缓冲区长度/2
void Ascii2Hex(uint8_t *outBuffer, uint8_t *inBuffer, uint8_t len)
{
	uint8_t i, temp;
	for(i=0;i<len;i++)
	{
		temp = inBuffer[2*i];
		if(temp < 0x40)
			outBuffer[i] = (temp-0x30)<<4;
		else
			outBuffer[i] = (temp-0x41+0x0A)<<4;
		
		temp = inBuffer[2*i+1];
		if(temp < 0x40)
			outBuffer[i] |= (temp-0x30)&0x0F;
		else
			outBuffer[i] |= (temp-0x41+0x0A)&0x0F;
	}
}

void Sort(uint32_t *pBuffer, uint8_t count)
{
	uint8_t i, j;
	uint32_t temp;
	for(i=0;i<count-1;i++)
	{
		for(j=i+1;j<count;j++)
		{
			if(pBuffer[i] > pBuffer[j])
			{
				temp = pBuffer[j];
				pBuffer[j] = pBuffer[i];
				pBuffer[i] = temp;
			}
		}
	}
}

uint32_t GetAverage(uint32_t *pBuffer, uint8_t count, uint8_t discardCount)
{
	uint8_t i;
	uint32_t average = 0;

	for(i=discardCount;i<(count-discardCount);i++)
		average += pBuffer[i];

	average /= (count-2*discardCount);
	return average;
}

uint32_t GetWeightedAverage(uint32_t *pBuffer, uint8_t len, uint8_t discardCount)
{
	uint64_t average = 0;
	uint16_t averageCnt = 0;
    uint8_t startIndex = discardCount;
	//uint8_t startIndex = len/4;
	//uint8_t startIndex = 0;
    uint8_t i, j;
    uint32_t temp;

	//排序
	for(i = 0; i < len-1; i++)
	{
		for(j = i+1; j < len; j++)
		{
			if(pBuffer[i] < pBuffer[j])
			{
				temp = pBuffer[i];
				pBuffer[i] = pBuffer[j];
				pBuffer[j] = temp;
			}
		}
	}

	//加权平均
	for(i = startIndex; i < len - startIndex; i++)
	{
		if(i <= len/2)
		{
			average += pBuffer[i]*(i - startIndex + 1);
			averageCnt += (i - startIndex + 1);
		}
		else
		{
			average += pBuffer[i]*(len - startIndex - i);
			averageCnt += (len - startIndex - i);
		}
	}
	average /= averageCnt;

	return (uint32_t)average;
}

uint32_t OrderFilter(uint32_t *pData, uint8_t len, uint8_t order)
{
	uint64_t average = 0;
	uint16_t averageCnt = 0;
    uint8_t i;
	for(i=0;i<len;i++)
	{
		average += (pData[i] * (i + 1));
		averageCnt += (i + 1);
	}
	average /= averageCnt;
	return (uint32_t)average;
}

void SortFloat(float *pBuffer, uint8_t count)
{
	uint8_t i, j;
	float temp;
	for(i=0;i<count-1;i++)
	{
		for(j=i+1;j<count;j++)
		{
			if(pBuffer[i] > pBuffer[j])
			{
				temp = pBuffer[j];
				pBuffer[j] = pBuffer[i];
				pBuffer[i] = temp;
			}
		}
	}
}

float GetAverageFloat(float *pBuffer, uint8_t count, uint8_t discardCount)
{
	uint8_t i;
	float average = 0;

	for(i=discardCount;i<(count-discardCount);i++)
		average += pBuffer[i];

	average /= (count-2*discardCount);
	return average;
}

uint8_t weightedTable[] = {1, 2, 4, 8, 16, 32, 64};

float GetWeightedAverageFloat(float *pBuffer, uint8_t len, uint8_t discardCount)
{
	float average = 0;
	uint16_t averageCnt = 0;
	uint8_t startIndex = len/4;
	//uint8_t startIndex = 0;
    uint8_t i, j;
//    uint8_t weightedIndex;
    float temp;

	//排序
	for(i = 0; i < len-1; i++)
	{
		for(j = i+1; j < len; j++)
		{
			if(pBuffer[i] < pBuffer[j])
			{
				temp = pBuffer[i];
				pBuffer[i] = pBuffer[j];
				pBuffer[j] = temp;
			}
		}
	}

	//加权平均
	for(i = startIndex; i < len - startIndex; i++)
	{
		if(i <= len/2)
		{
			average += pBuffer[i]*(i - startIndex + 1);
			averageCnt += (i - startIndex + 1);
		}
		else
		{
			average += pBuffer[i]*(len - startIndex - i);
			averageCnt += (len - startIndex - i);
		}
	}
	average /= averageCnt;

//    weightedIndex = 0;
//    for(i = startIndex; i < len - startIndex; i++)
//	{
//		if(i < len/2)
//		{
//			average += pBuffer[i]*weightedTable[weightedIndex];
//			averageCnt += weightedTable[weightedIndex];
//            weightedIndex++;
//		}
//        else
//        {
//            average += pBuffer[i]*weightedTable[weightedIndex];
//			averageCnt += weightedTable[weightedIndex];
//            weightedIndex--;
//        }
//	}
//    average /= averageCnt;
    
	return average;
}

float OrderFilterFloat(float *pData, uint8_t len, uint8_t order)
{
	float average = 0;
	uint16_t averageCnt = 0;
    uint8_t i;
	for(i=0;i<len;i++)
	{
		average += (pData[i] * (i + 1));
		averageCnt += (i + 1);
	}
	average /= averageCnt;
	return average;
}


/**
 * @brief  Reset the mcu by software
 *
 * @param  none
 *
 * @note   use the 3.5 version of the firmware library. 
 */
void SoftReset(void)
{
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}

void GetChipID(uint32_t *chipID)
{
//#ifdef STM32SIM
	chipID[0] = *(__IO uint32_t*)(0X1FFFF7F0); //高字节
	chipID[1] = *(__IO uint32_t*)(0X1FFFF7EC);
	chipID[2] = *(__IO uint32_t*)(0X1FFFF7E8); //低字节
//#else
//	chipID[0] = 0X0FFFF7F0;
//	chipID[1] = 0X0FFFF7EC;
//	chipID[2] = 0X1FFFF7E8;
//#endif
	//STM32_FLASH_SIZE = *(__IO uint32_t*)(0X1FFFF7E0);
}

uint32_t RandomString2UInt32(uint8_t *str)
{
    uint8_t strLen = strlen((const char*)str);
    uint8_t i, j = 0;
    uint32_t ret = 0;
    uint8_t *p = (uint8_t*)&ret;
    
    for(i=0;i<strLen;i++)
    {
        p[j] ^= str[i];
        j++;
        if(j >= 4)
            j = 0;
    }
    
    return ret;
}

uint8_t BCDToInt(uint8_t bcd) //BCD转10进制
{
	return (0xff & (bcd>>4))*10 +(0xf & bcd);
}

uint8_t IntToBCD(uint8_t i) //10进制转BCD
{
	return (((i/10)<<4)+((i%10)&0x0f));
}
