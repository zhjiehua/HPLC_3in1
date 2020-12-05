#ifndef __MISCELLANEOUS_H_
#define __MISCELLANEOUS_H_

#include "stdint.h"

struct date
{
	int year;
	int month;
	int day;
};

void AddCRC16(uint8_t *buffer,uint16_t n,uint16_t *pcrc);
uint16_t CheckCRC16(uint8_t *buffer,uint16_t n);

void Hex2Ascii(uint8_t *outBuffer, uint8_t *inBuffer, uint8_t len);
void Ascii2Hex(uint8_t *outBuffer, uint8_t *inBuffer, uint8_t len);

void Sort(uint32_t *pBuffer, uint8_t count);
uint32_t GetAverage(uint32_t *pBuffer, uint8_t count, uint8_t discardCount);
uint32_t GetWeightedAverage(uint32_t *pBuffer, uint8_t len, uint8_t discardCount);
uint32_t OrderFilter(uint32_t *pData, uint8_t len, uint8_t order);

void SortFloat(float *pBuffer, uint8_t count);
float GetAverageFloat(float *pBuffer, uint8_t count, uint8_t discardCount);
float GetWeightedAverageFloat(float *pBuffer, uint8_t len, uint8_t discardCount);
float OrderFilterFloat(float *pData, uint8_t len, uint8_t order);

void SoftReset(void);
void GetChipID(uint32_t *chipID);
uint32_t RandomString2UInt32(uint8_t *str);
uint8_t BCDToInt(uint8_t bcd);
uint8_t IntToBCD(uint8_t i);

int dateDiff(struct date mindate,struct date maxdate);
void UsefulDaysCheck(void);

#endif
