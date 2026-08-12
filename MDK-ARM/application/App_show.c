#include "App_show.h"
uint8_t clock_flag=0;
uint32_t clock_last_time;
extern SHOW_Time_Set show_time_set;
extern Show_alarm_Set show_alarm_set;
extern Alarm_Time_Type alarm_time1;
extern Alarm_Time_Type alarm_time2;
/**
 * 初始化灯
 */
void App_show_start(void)
{
    Inf_led_start();
    clock_last_time=xTaskGetTickCount();
}
/**
 * 常规显示
 */
void App_show_normal(Clock_Date_time_type*clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type)
{
    //时钟的点
    if (xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag=!clock_flag;
        clock_last_time=xTaskGetTickCount();
    }
    uint8_t hour=0;
    if (clock_data_time->is_pm&&clock_data_time->is_12_flag)
    {
        hour=clock_data_time->hour+12;
    }
    else
    {
        hour=clock_data_time->hour;
    }
    if (show_type->is_12_hour)
    {
        if (hour>=12)
        {
            hour-=12;
            show_type->is_pm=1;
        }
        else
        {
            show_type->is_pm=0;
        }
    }
    
    Inf_led_set_all(hour,clock_data_time->min,clock_flag,temperature,humidity,show_type);
}
/**
 * 设置时显示
 */
void App_show_time_set(Clock_Date_time_type *clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type)
{
    if (xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag=!clock_flag;
        clock_last_time=xTaskGetTickCount();
    }
    switch (show_time_set)
    {
    case TIME_SET_YEAR:
        // Inf_led_time_set(TIME_SET_YEAR,clock_data_time->year);
        if (clock_flag)
        {
            Inf_led_set_all(clock_data_time->year/100,clock_data_time->year%100,0,temperature,humidity,show_type);
        }
        else
        {
            Inf_led_set_all(clock_data_time->year/100,0xff,0,temperature,humidity,show_type);
        }
        break;
    case TIME_SET_MONTH:
        // 
        if (clock_flag)
        {
            Inf_led_set_all(clock_data_time->month,0xff,0,temperature,humidity,show_type);
        }
        else
        {
            Inf_led_set_all(0xff,0xff,0,temperature,humidity,show_type);
        }
        break;
    case TIME_SET_DAY:
        // Inf_led_time_set(TIME_SET_DAY,clock_data_time->day);
        if (clock_flag)
        {
            Inf_led_set_all(0xff,clock_data_time->day,0,temperature,humidity,show_type);
        }
        else
        {
            Inf_led_set_all(0xff,0xff,0,temperature,humidity,show_type);
        }
        break;
    case TIME_SET_HOUR:
        // Inf_led_time_set(TIME_SET_HOUR,clock_data_time->hour);
        if (clock_flag)
        {
            Inf_led_set_all(clock_data_time->hour,0xff,1,temperature,humidity,show_type);
        }
        else
        {
            Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        }
        break;
    case TIME_SET_MIN:
        // Inf_led_time_set(TIME_SET_MIN,clock_data_time->min);
        if (clock_flag)
        {
            Inf_led_set_all(0xff,clock_data_time->min,1,temperature,humidity,show_type);
        }
        else
        {
            Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        }
        break;
    default:
        break;
    }
}

/**
 * 闹钟设置时显示
 */
void App_show_alarm_set(Clock_Date_time_type *clock_data_time,int8_t temperature,int8_t humidity,Show_type_struct*show_type)
{
    if (xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag=!clock_flag;
        clock_last_time=xTaskGetTickCount();
    }
    switch (show_alarm_set)
    {
    case ALARM_SET_HOUR:
        if (clock_flag)
        {
            if (show_type->is_alarm_1)
            {
                Inf_led_set_all(alarm_time1.hour,0xff,1,temperature,humidity,show_type);
            }
            else if (show_type->is_alarm_2)
            {
                Inf_led_set_all(alarm_time2.hour,0xff,1,temperature,humidity,show_type);
            }
        }
        else
        {
            Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        }
        break;
    case ALARM_SET_MIN:
        if (clock_flag)
        {
            if (show_type->is_alarm_1)
            {
                Inf_led_set_all(0xff,alarm_time1.min,1,temperature,humidity,show_type);
            }
            else if (show_type->is_alarm_2)
            {
                Inf_led_set_all(0xff,alarm_time2.min,1,temperature,humidity,show_type);
            }
        }
        else
        {
            Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        }
        break;
    default:
        break;
    }
}
