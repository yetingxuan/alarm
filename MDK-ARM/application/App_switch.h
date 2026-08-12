#ifndef __APP_SWITCH_H__
#define __APP_SWITCH_H__ 

#include "freeRTOS.h"
#include "task.h"
#include "KEY.h"
#include "LED.h"
#include "MIC.h"
#include "App_show.h"

void App_switch_key_process(void);
void App_switch_rotary_process(void);
void App_switch_time_set_process(void);
void App_switch_alarm_set_process(void);

#endif

