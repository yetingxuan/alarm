#ifndef __APP_FREERTOS__
#define __APP_FREERTOS__
#include "FreeRTOS.h"
#include "task.h"
#include "DEBUG.h"
#include "KEY.h"
#include "MIC.h"
#include "DS_1302Z.h"
#include "DHT11.h"
#include "NV020D.h"
// #include "LED.h"
#include "App_show.h"
#include "App_switch.h"
#include "touch.h"

void App_freeRTOS_start(void);

#endif
