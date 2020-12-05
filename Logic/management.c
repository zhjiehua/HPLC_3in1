#include "management.h"
#include "miscellaneous.h"

#include "pageCommon.h"
#include "cmd_queue.h"
#include "hmi_driver.h"

#include "config.h"

#include "CPrintf.h"

#include "wdg.h"

Man_TypeDef man;

void InitMan(void)
{
    IWDG_Feed();
  
    cDebug("InitMan()...\r\n");
    
    
    cDebug("InitMan()...Finish\r\n");
}

