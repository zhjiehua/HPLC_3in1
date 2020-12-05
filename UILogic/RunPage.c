#include "pageCommon.h"
#include "management.h"
#include "CPrintf.h"
#include "StepMotor.h"
#include "IO.h"
#include "24cxx.h"

#include "hmi_driver.h"
#include "cmd_process.h"

#include "device_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

void RunPageButtonProcess(uint16 control_id, uint8  state)
{
	switch(control_id)
	{
		case RUNPAGE_LAMP_BUTTON:
			Detector_SetButtonValue(RUNPAGE_INDEX, DETECTOR_RUNPAGE_LAMP_BUTTON, state);
			//cDebug("DETECTOR_RUNPAGE_LAMP_BUTTON = %d\r\n", state);
		break;
		case RUNPAGE_CLEAR_BUTTON:
			Detector_SetButtonValue(RUNPAGE_INDEX, DETECTOR_RUNPAGE_CLEAR_BUTTON, state);
		break;
		
		case RUNPAGE_START_BUTTON:
			PumpA_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_START_BUTTON, state);
			PumpB_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_START_BUTTON, state);
		break;
		case RUNPAGE_STOP_BUTTON:
			PumpA_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_STOP_BUTTON, state);
			PumpB_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_STOP_BUTTON, state);
		break;
		case RUNPAGE_PURGEA_BUTTON:
			if(state)
				PumpA_SetButtonValue(PURGEPAGE_INDEX, PUMP_PURGEPAGE_PURGE_BUTTON, state);
			else
				PumpA_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_STOP_BUTTON, state);
		break;
		case RUNPAGE_PURGEB_BUTTON:
			if(state)
				PumpB_SetButtonValue(PURGEPAGE_INDEX, PUMP_PURGEPAGE_PURGE_BUTTON, state);
			else
				PumpB_SetButtonValue(RUNPAGE_INDEX, PUMP_RUNPAGE_STOP_BUTTON, state);
		break;
		case RUNPAGE_ABOUT_BUTTON:
			Detector_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_ABOUT_BUTTON, state);
			PumpA_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_ABOUT_BUTTON, state);
			PumpB_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_ABOUT_BUTTON, state);
		
			Detector_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_DEVICEINFO_BUTTON, state);
			PumpA_SetButtonValue(MENUPAGE_INDEX, MENUPAGE_DEVICEINFO_BUTTON, state);
		break;
		default:
			cDebug("RunPage BUTTON error!\n");
		break;
	} 
}

void RunPageEditProcess(uint16 control_id, uint8 *str)
{
	switch(control_id)
	{
		case RUNPAGE_WAVE_EDIT:
			Detector_SetTextValue(RUNPAGE_INDEX, DETECTOR_RUNPAGE_WAVE_EDIT, str);
		break;
		case RUNPAGE_AU_EDIT:
			Detector_SetTextValue(RUNPAGE_INDEX, DETECTOR_RUNPAGE_AU_EDIT, str);
		break;

		case RUNPAGE_FLOWA_EDIT:
			PumpA_SetTextValue(RUNPAGE_INDEX, PUMP_RUNPAGE_FLOW_EDIT, str);
		break;
		case RUNPAGE_FLOWB_EDIT:
			PumpB_SetTextValue(RUNPAGE_INDEX, PUMP_RUNPAGE_FLOW_EDIT, str);
		break;
		
		default:
			cDebug("RunPage EDIT error!\n");
		break;
	}
}

#ifdef __cplusplus
}
#endif
