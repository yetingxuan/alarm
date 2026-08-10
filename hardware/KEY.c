#include "KEY.h"
#include "gpio.h"
/**
 * 按键
 */
//短按按键
uint8_t Inf_get_key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==RESET)
    {
        vTaskDelay(10);
        if (HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==RESET)
        {
            while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==RESET)
            {
                vTaskDelay(1);
            }
            return 1;
        }
    }
    return 0;
}
Key_type_value Inf_get_key_value(void)
{
    if(Inf_get_key_press(UP_GPIO_Port,UP_Pin))
    {
        return KEY_UP;
    }
    else if(Inf_get_key_press(DOWN_GPIO_Port,DOWN_Pin))
    {
        return KEY_DOWN;
    }
    else if (Inf_get_key_press(ALARM_EN_GPIO_Port,ALARM_EN_Pin))
    {
        return KEY_ALARM_EN;
    }
    else if (Inf_get_key_press(ALARM_5_GPIO_Port,ALARM_5_Pin))
    {
        return KEY_ALARM_5;
    }
    //可长按按键
    if (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==RESET)
    {
        vTaskDelay(10);
        if (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==RESET)
        {
            TickType_t start_tick;
            start_tick=xTaskGetTickCount();
            while (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==RESET)
            {
                vTaskDelay(1);
            }
            TickType_t end_tick;
            end_tick=xTaskGetTickCount();
            if (end_tick-start_tick>=3000)
            {
                return KEY_TIME_SET_LONG;
            }
            else
            {
                return KEY_TIME_SET;
            }
        }
    }
    if (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==RESET)
    {
        vTaskDelay(10);
        if (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==RESET)
        {
            TickType_t start_tick;
            start_tick=xTaskGetTickCount();
            while (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==RESET)
            {
                vTaskDelay(1);
            }
            TickType_t end_tick;
            end_tick=xTaskGetTickCount();
            if (end_tick-start_tick>=3000)
            {
                return KEY_ALARM_SET_LONG;
            }
            else
            {
                return KEY_ALARM_SET;
            }
        }
    }
    return KEY_NONE;
}
/**
 * 按键长按
 */
LED_ON_Type_value Inf_get_led_on_value(void)
{
    if (HAL_GPIO_ReadPin(LED_ON_GPIO_Port,LED_ON_Pin)==RESET)
    {
        return LED_ON;
    }
    return LED_OFF;
    
}
/**
 * 声控/常亮
 */
Light_Type_value Inf_get_light_value(void)
{
    if (HAL_GPIO_ReadPin(LIGHT_GPIO_Port,LIGHT_Pin)==RESET)
    {
        return LIGHT_ON;
    }
    return LIGHT_OFF;
}
/**
 * 触摸按键
 */
Touch_value Inf_get_touch_value(void)
{
    if (HAL_GPIO_ReadPin(TOUCH_GPIO_Port,TOUCH_Pin)==RESET)
    {
        return TOUCH_NONE;
    }
    return TOUCH_UP;
}
