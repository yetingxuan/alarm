#include "App_switch.h"
extern Show_type_struct show_type;
extern SHOW_Time_Set show_time_set;
extern Show_alarm_Set show_alarm_set;
extern Clock_Date_time_type clock_data_time;
extern uint8_t led_en;
uint8_t switch_alarm_en=0;
extern Show_Mode_type show_mode;
extern Alarm_Time_Type alarm_time1;
extern Alarm_Time_Type alarm_time2;
/**
 * 按键开关
 */
void App_switch_key_process(void)
{
    Key_type_value key_value=Inf_get_key_value();
    switch (key_value)
    {
    case KEY_TIME_SET: 
        show_type.is_12_hour=(show_type.is_12_hour+1)%2;
        break;
    case KEY_TIME_SET_LONG:
        show_mode=SHOW_MOOD_TIME_SET;
        //切换24小时
        show_type.is_12_hour=0;
        if (clock_data_time.is_12_flag==1&&clock_data_time.is_pm==1)
        {
            clock_data_time.hour+=12;
            clock_data_time.is_12_flag=0;
        }
        break;
    case KEY_UP:
        show_type.is_celsius=(show_type.is_celsius+1)%2;
        break;
    case  KEY_DOWN:
        show_mode=SHOW_MOOD_VOLUME;
        break;
    case KEY_ALARM_SET:
        show_mode=SHOW_MOOD_VOLUME;
        break;
    case KEY_ALARM_SET_LONG:
    //也统一24小时制
        show_mode=SHOW_MOOD_ALARM_SET;
        show_type.is_12_hour=0;
        show_type.is_alarm_1=1;
        show_type.is_alarm_2=0;
        break;
    case KEY_ALARM_EN:
        switch_alarm_en++;
        switch_alarm_en%=6;
        if (switch_alarm_en%2==0)
        {
            show_type.is_alarm_1=0;
            show_type.is_alarm_2=0;
        }
        else if (switch_alarm_en==1)
        {
            show_type.is_alarm_1=1;
        }
        else if (switch_alarm_en==3)
        {
            show_type.is_alarm_2=1;
        }
        else if (switch_alarm_en==5)
        {
            show_type.is_alarm_1=1;
            show_type.is_alarm_2=1;
        }
        break;
    case KEY_ALARM_5:
        show_type.is_alarm_five_day=(show_type.is_alarm_five_day+1)%2;
        break;
    default:
        break;
    }
}
/**
 * 拨动开关
 */
void App_switch_rotary_process(void)
{
    //总开关
    LED_ON_Type_value led_on_value= Inf_get_led_on_value();
    if (led_on_value==LED_ON)
    {
        led_en=1;
    }
    else
    {
        led_en=0;
    }
    //声控
    Light_Type_value Light_Type_value=Inf_get_light_value();
    static uint8_t sound_control_flag;
    static uint32_t sound_control_last_time;
    
    Mic_value mic_value=Inf_get_mic_value();
    if (mic_value==MIC_ON)
    {
        sound_control_flag=1;
        sound_control_last_time=xTaskGetTickCount();
    }
    if (led_on_value==LED_ON)
    {
        if (Light_Type_value==LIGHT_ON)
        {
            led_en=1;
        }
        else if (sound_control_flag==1)
        {
            if (xTaskGetTickCount()-sound_control_last_time<=5000)
            {
                led_en=1;
            }
            else
            {
                led_en=0;
            }
        }
        else
        {
            led_en=0;
        }
    }
    else
    {
        led_en=0;
    }
}
/**
 * 时间设置时的按键逻辑
 */
void App_switch_time_set_process(void)
{
    Key_type_value key_value=Inf_get_key_value();
    switch (key_value)
    {
    case KEY_UP:
        if (show_time_set==TIME_SET_YEAR)
        {
            clock_data_time.year++;
            if (clock_data_time.year>2099)
            {
                clock_data_time.year=2000;
            }
        }
            else if (show_time_set==TIME_SET_MONTH) 
            {
                clock_data_time.month++;
                if (clock_data_time.month>12)
                {
                     clock_data_time.month=1;
                }
            }
            else if (show_time_set==TIME_SET_DAY)
            {
                clock_data_time.day++;
                if (clock_data_time.day>31)
                {
                    clock_data_time.day=1;
                }
            }
            else if (show_time_set==TIME_SET_HOUR)
            {
                clock_data_time.hour++;
                if (show_type.is_12_hour==1)
                {
                    if (clock_data_time.hour>12)
                    {
                        clock_data_time.hour=0;
                    }
                }
                else
                {
                    if (clock_data_time.hour>23)
                    {
                        clock_data_time.hour=0;
                    }
                }
            }
            else if (show_time_set==TIME_SET_MIN)
            {
                clock_data_time.min++;
                if (clock_data_time.min>59)
                {
                    clock_data_time.min=0;
                }
                
            }
        
        break;
    case KEY_DOWN:
        if (show_time_set == TIME_SET_YEAR)
        {
            clock_data_time.year--;
            if (clock_data_time.year < 2000)
            {
                clock_data_time.year = 2099;
            }
        }
        else if (show_time_set == TIME_SET_MONTH)
        {
            clock_data_time.month--;
            if (clock_data_time.month < 1)
            {
                clock_data_time.month = 12;
            }
        }
        else if (show_time_set == TIME_SET_DAY)
        {
            clock_data_time.day--;
            if (clock_data_time.day < 1)
            {
                clock_data_time.day = 31;
            }
        }
        else if (show_time_set == TIME_SET_HOUR)
        {
            clock_data_time.hour--;
            if (clock_data_time.hour < 0)
            {
                clock_data_time.hour = 23;
            }
        }
        else if (show_time_set == TIME_SET_MIN)
        {
            clock_data_time.min--;
            if (clock_data_time.min < 0)
            {
                clock_data_time.min = 59;
            }
        }
        break;
    case KEY_TIME_SET:
        show_time_set++;
        if (show_time_set == TIME_SET_END)
        {
            show_mode = SHOW_MOOD_NORMAL;
            clock_data_time.is_12_flag=show_type.is_12_hour;
            clock_data_time.is_pm=show_type.is_pm;
            dataTime_set(&clock_data_time);
            show_time_set = TIME_SET_YEAR;
        }
        break;
    default:
        break;
    }
}
/**
 * 闹钟设置按键效果
 */
void App_switch_alarm_set_process(void)
{
    Key_type_value key_value=Inf_get_key_value();
    switch (key_value)
    {
    case KEY_UP:
        if (show_alarm_set==ALARM_SET_HOUR)
        {
            if (show_type.is_alarm_1==1)
            {
                alarm_time1.hour++;
                alarm_time1.hour%=24;
            }
            else if (show_type.is_alarm_2==1)
            {
                alarm_time2.hour++;
                alarm_time2.hour%=24;
            }
        }
        else if (show_alarm_set==ALARM_SET_MIN)
        {
            if (show_type.is_alarm_1==1)
            {
                alarm_time1.min++;
                alarm_time1.min%=60;
            }
            else if (show_type.is_alarm_2==1)
            {
                alarm_time2.min++;
                alarm_time2.min%=60;
            }
        }
        break;
    case KEY_DOWN:
        if (show_alarm_set==ALARM_SET_HOUR)
        {
            if (show_type.is_alarm_1==1)
            {
                alarm_time1.hour--;
                if (alarm_time1.hour<0)
                {
                    alarm_time1.hour=23;
                }
            }
            else if (show_type.is_alarm_2==1)
            {
                alarm_time2.hour--;
                if (alarm_time2.hour<0)
                {
                    alarm_time2.hour=23;
                }
            }
        }
        else if (show_alarm_set==ALARM_SET_MIN)
        {
            if (show_type.is_alarm_1==1)
            {
                alarm_time1.min--;
                if (alarm_time1.min<0)
                {
                    alarm_time1.min=59;
                }
            }
            else if (show_type.is_alarm_2==1)
            {
                alarm_time2.min--;
                if (alarm_time2.min<0)
                {
                    alarm_time2.min=59;
                }
            }
        }
        break;
    case KEY_ALARM_SET:
        show_alarm_set++;
        if (show_alarm_set==ALARM_SET_END)
        {
            if (switch_alarm_en%2==0)
            {
                if (show_type.is_alarm_1==1)
                {
                    switch_alarm_en=1;
                }
                else if (show_type.is_alarm_2==1)
                {
                    switch_alarm_en=3;
                }
            }
            else if (switch_alarm_en==1)
            {
                if (show_type.is_alarm_2==1)
                {
                    switch_alarm_en=5;
                    show_type.is_alarm_1=1;
                }
            }
            else if (switch_alarm_en==3)
            {
                if (show_type.is_alarm_1==1)
                {
                    switch_alarm_en=5;
                    show_type.is_alarm_2=1;
                }
            }
            show_mode=SHOW_MOOD_NORMAL;
            show_alarm_set=ALARM_SET_HOUR;
        }
        
        break;
    case KEY_ALARM_EN:
        if (show_type.is_alarm_1==1)
        {
            show_type.is_alarm_1=0;
            show_type.is_alarm_2=1;
        }
        else if (show_type.is_alarm_2==1)
        {
            show_type.is_alarm_2=0;
            show_type.is_alarm_1=1;
        }
        
        break;
    default:
        break;
    }
}
