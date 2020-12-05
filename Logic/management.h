
#ifndef __MANAGEMENT_H_
#define __MANAGEMENT_H_

#include "stdint.h"
#include "StepMotor.h"
#include "miscellaneous.h"

//相关信息
#define HARDWARE_VERSION    "3in1 V1.0"
#define SOFTWARE_VERSION    "V1.0.0"

//====================================
typedef struct
{
	uint8_t detectorReady;
	uint8_t pumpaReady;
	uint8_t pumpbReady;

	uint8_t lcdReady;
	uint8_t lcdBackLightFlag;
	uint8_t strLcd[40];//公共用

	struct date lcdDate;

	uint8_t factoryMode;
	
	uint8_t cDebugBuffer[100];
}Man_TypeDef;

extern Man_TypeDef man;

extern const uint8_t timeConstant[];
extern const float range[];

extern const uint8_t TimeConst[6][4];
extern const uint8_t Range[16][7];
extern const uint8_t Channel[2][3];
extern const uint8_t LampType[2][5];
extern const uint8_t Channel_En[2][7];
extern const uint8_t LampType_En[2][10];
    
extern const uint8_t ComType[2][5];
extern const uint8_t Language[2][5];
extern const uint8_t ComType_En[2][6];
extern const uint8_t Language_En[2][8];

extern const uint8_t Model[1][8];
extern const uint8_t RangeIntegral[8][4];

extern const uint8_t Wave2[2][6];
extern const uint8_t Absorb2[2][8];
extern const uint8_t Reference2[2][11];   
extern const uint8_t Sample2[2][8];    

void InitMan(void);
void InitUI(void);

#endif
