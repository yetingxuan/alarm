#ifndef APP__SHOW_H
#define APP__SHOW_H

#include "LED.h"
#include "DS_1302Z.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum{
    SHOW_MOOD_NORMAL,
    SHOW_MOOD_TIME_SET,
    SHOW_MOOD_ALARM_SET,
    SHOW_MOOD_VOLUME,
}Show_Mode_type;


void App_show_start(void);
void App_show_normal(Clock_Date_time_type*clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type);
void App_show_time_set(Clock_Date_time_type *clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type);
void App_show_alarm_set(Clock_Date_time_type *clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type);

#endif
