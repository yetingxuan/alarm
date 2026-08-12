#include "App_freeRTOS.h"
/**
 * 采集任务
 */
void task_collect(void *pvParameters);
#define COLLECT_TASK_STACK_SIZE 128
#define COLLECT_TASK_PRIORITY 3
TaskHandle_t collect_task_handle;
Clock_Date_time_type clock_data_time = {0};
int8_t temperature, humidity;
/**
 * 展示任务
 */
void task_show(void *pvParameters);
#define SHOW_TASK_STACK_SIZE 128
#define SHOW_TASK_PRIORITY 1
TaskHandle_t show_task_handle;
Show_Mode_type show_mode = SHOW_MOOD_NORMAL;
SHOW_Time_Set show_time_set = TIME_SET_YEAR;
Show_alarm_Set show_alarm_set = ALARM_SET_HOUR;
Show_type_struct show_type = {.is_12_hour = 0, .is_pm = 0, .is_celsius = 0, .is_alarm_1 = 0, .is_alarm_2 = 0, .is_alarm_five_day = 0, .is_charing = 0};
/**
 * 触发任务
 */
void task_switch(void *pvParameters);
#define SWITCH_TASK_STACK_SIZE 128
#define SWITCH_TASK_PRIORITY 2
TaskHandle_t switch_task_handle;
uint8_t led_en = 1;
Alarm_Time_Type alarm_time1 = {0};
Alarm_Time_Type alarm_time2 = {0};
uint8_t touch_count = 0;
/**
 * 闹钟任务
 */
void task_alarm(void *pvParameters);
#define ALARM_TASK_STACK_SIZE 128
#define ALARM_TASK_PRIORITY 4
TaskHandle_t alarm_task_handle;
/**
 * 调节音量任务
 */
void task_volume(void *pvParameters);
#define VOLUME_TASK_STACK_SIZE 128
#define VOLUME_TASK_PRIORITY 4
TaskHandle_t volume_task_handle;
int8_t nv02d_volume=5;
void App_freeRTOS_start(void)
{
    xTaskCreate(task_collect, "collect", COLLECT_TASK_STACK_SIZE, NULL, COLLECT_TASK_PRIORITY, &collect_task_handle);
    xTaskCreate(task_show, "show", SHOW_TASK_STACK_SIZE, NULL, SHOW_TASK_PRIORITY, &show_task_handle);
    xTaskCreate(task_switch, "switch", SWITCH_TASK_STACK_SIZE, NULL, SWITCH_TASK_PRIORITY, &switch_task_handle);
    xTaskCreate(task_alarm, "alarm", ALARM_TASK_STACK_SIZE, NULL, ALARM_TASK_PRIORITY, &alarm_task_handle);
    xTaskCreate(task_volume, "volume", VOLUME_TASK_STACK_SIZE, NULL, VOLUME_TASK_PRIORITY, &volume_task_handle);
    vTaskStartScheduler();
}

void task_collect(void *pvParameters)
{
    Inf_dht11_init();
    uint8_t count = 9;
    while (1)
    {
        count++;
        if (count == 10)
        {
            count = 0;
            Inf_dht11_get_data(&temperature, &humidity);
        }
        if (show_mode != SHOW_MOOD_TIME_SET)
        {
            datatime_get(&clock_data_time);
        }
        vTaskDelay(1000);
    }
}
void task_show(void *pvParameters)
{
    App_show_start();
    while (1)
    {
        if (led_en)
        {
            if (show_mode == SHOW_MOOD_NORMAL||show_mode==SHOW_MOOD_VOLUME)
            {
                App_show_normal(&clock_data_time, temperature, humidity, &show_type);
            }
            else if (show_mode == SHOW_MOOD_TIME_SET)
            {
                App_show_time_set(&clock_data_time, temperature, humidity, &show_type);
            }
            else if (show_mode == SHOW_MOOD_ALARM_SET)
            {
                App_show_alarm_set(&clock_data_time, temperature, humidity, &show_type);
            }
        }
        else
        {
            Inf_led_stop();
        }
    }
}
void task_switch(void *pvParameters)
{
    while (1)
    {
        if (show_mode == SHOW_MOOD_NORMAL)
        {
            App_switch_key_process();
        }
        else if (show_mode == SHOW_MOOD_TIME_SET)
        {
            App_switch_time_set_process();
        }
        else if (show_mode == SHOW_MOOD_ALARM_SET)
        {
            App_switch_alarm_set_process();
        }
        Touch_value touch_value = Inf_get_touch_value();
        if (touch_value == TOUCH)
        {
            touch_count++;
            touch_count %= 5;
        }
        App_switch_rotary_process();
        vTaskDelay(50);
    }
}
void task_alarm(void *pvParameters)
{
    Inf_nv020d_init();
    uint8_t count_alarm = 0;
    uint8_t alarm_touch_count = 0;
    while (1)
    {
        if (show_type.is_alarm_five_day == 1 && clock_data_time.week >= 6)
        {
            /* code */
        }
        else
        {
            if (show_mode == SHOW_MOOD_NORMAL)
            {
                if (show_type.is_alarm_1 || show_type.is_alarm_2)
                {
                    if (show_type.is_alarm_1)
                    {
                        // 开启闹钟
                        if (alarm_time1.hour == clock_data_time.hour && alarm_time1.min == clock_data_time.min)
                        {
                            // 时间对上
                            if (alarm_time1.is_on == 0)
                            {
                                // 闹钟没响过
                                alarm_time1.is_on = 1;
                                Inf_nv020d_send_cmd(0x00);
                                alarm_touch_count = touch_count;
                            }
                            else if (alarm_time1.is_on == 1 && alarm_time1.is_done == 0)
                            {
                                // 闹钟第一次且正在响
                                count_alarm++;
                                if (count_alarm >= 5)
                                {
                                    Inf_nv020d_send_cmd(0x00);
                                    count_alarm = 0;
                                }
                                if (alarm_touch_count != touch_count)
                                {
                                    Inf_nv020d_send_cmd(0xfe);
                                    alarm_time1.is_done = 1;
                                }
                            }
                            else if (alarm_time1.is_on == 1 && alarm_time1.is_done == 1)
                            {
                                // 闹钟被关过
                            }
                        }
                        else
                        {
                            // 时间不对应如果是自动关的说明响了一分钟   全关
                            if (alarm_time1.is_on == 1)
                            {
                                alarm_time1.is_on = 0;
                                alarm_time1.is_done = 0;
                                Inf_nv020d_send_cmd(0xfe);
                            }
                        }
                    }
                    else if (show_type.is_alarm_2)
                    {
                        // 开启闹钟
                        if (alarm_time2.hour == clock_data_time.hour && alarm_time2.min == clock_data_time.min)
                        {
                            // 时间对上
                            if (alarm_time2.is_on == 0)
                            {
                                // 闹钟没响过
                                alarm_time2.is_on = 1;
                                Inf_nv020d_send_cmd(0x00);
                                alarm_touch_count = touch_count;
                            }
                            else if (alarm_time2.is_on == 1 && alarm_time2.is_done == 0)
                            {
                                // 闹钟第一次且正在响
                                count_alarm++;
                                if (count_alarm >= 5)
                                {
                                    Inf_nv020d_send_cmd(0x00);
                                    count_alarm = 0;
                                }
                                if (alarm_touch_count != touch_count)
                                {
                                    Inf_nv020d_send_cmd(0xfe);
                                    alarm_time2.is_done = 1;
                                }
                            }
                            else if (alarm_time2.is_on == 1 && alarm_time2.is_done == 1)
                            {
                                // 闹钟关过
                            }
                        }
                        else
                        {
                            // 时间不对应如果是自动关的说明响了一分钟   全关
                            if (alarm_time2.is_on == 1)
                            {
                                alarm_time2.is_on = 0;
                                alarm_time2.is_done = 0;
                                Inf_nv020d_send_cmd(0xfe);
                            }
                        }
                    }
                }
            }
        }
        vTaskDelay(1000);
    }
}
void task_volume(void *pvParameters)
{
    Inf_nv020d_init();
    Inf_nv020d_send_cmd(0xe0+nv02d_volume);
    uint8_t count_volume=99;
    while (1)
    {
        if (show_mode==SHOW_MOOD_VOLUME)
        {
            count_volume++;
            if (count_volume>=100)
            {
                Inf_nv020d_send_cmd(0x00);
                count_volume=0;
            }
            uint8_t key_value= Inf_get_key_value();
            switch (key_value)
            {
            case KEY_UP:
                nv02d_volume++;
                nv02d_volume%=7;
                Inf_nv020d_send_cmd(0xe0+nv02d_volume);
                break;
            case KEY_DOWN:
                nv02d_volume--;
                if (nv02d_volume<0)
                {
                    nv02d_volume=0;
                }
                Inf_nv020d_send_cmd(0xe0+nv02d_volume);
                break;
            case KEY_ALARM_SET:
                Inf_nv020d_send_cmd(0xfe);
                show_mode=SHOW_MOOD_NORMAL;
                count_volume=99;
                break;
            default:
                break;
            }
            
        }
        vTaskDelay(50);
    }
    
}
