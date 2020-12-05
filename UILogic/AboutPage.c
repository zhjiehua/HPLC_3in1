#include "pageCommon.h"
#include "management.h"
#include "miscellaneous.h"
#include "CPrintf.h"
#include "stdlib.h"
#include "hmi_driver.h"
#include "cmd_process.h"

#include "device_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ACTIVEMASK 0x0502
    
void AboutPageButtonProcess(uint16 control_id, uint8 state)
{
    uint16_t usefulDays;
	uint16_t validateCode;
	uint16_t temp;
    
	switch(control_id)
	{
    case ABOUTPAGE_RETURN_BUTTON:
			Detector_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_RETURN_BUTTON, state);
			PumpA_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_RETURN_BUTTON, state);
		break;
		default:
			cDebug("AboutPage BUTTON error!\n");
		break;
	}
}

void AboutPageEditProcess(uint16 control_id, uint8 *str)
{
	switch(control_id)
	{
		default:
			cDebug("AboutPage EDIT error!\n");
		break;
	}
}

#ifdef __cplusplus
}
#endif
