#ifndef __KEY_H__
#define __KEY_H__ 

#include "gpio.h"
#include "DEBUG.h"
#include "freeRTOS.h"
#include "task.h"

typedef enum{
    KEY_NONE=0,
    KEY_TIME_SET,
    KEY_TIME_SET_LONG,
    KEY_UP,
    KEY_DOWN,
    KEY_ALARM_SET,
    KEY_ALARM_SET_LONG,
    KEY_ALARM_EN,
    KEY_ALARM_5,
}Key_type_value;
typedef enum{
    LED_OFF=0,
    LED_ON,
}LED_ON_Type_value;
typedef enum{
    LIGHT_ON=0,
    LIGHT_OFF,
}Light_Type_value;
typedef enum{
    TOUCH_NONE=0,
    TOUCH_UP,
}Touch_value;

Key_type_value Inf_get_key_value(void);
LED_ON_Type_value Inf_get_led_on_value(void);
Light_Type_value Inf_get_light_value(void);
Touch_value Inf_get_touch_value(void);
#endif
