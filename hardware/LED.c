#include "LED.h"
//小时十位0~2
uint16_t Led_clk_h_s[3]={0X0000,0X0006,0X005B};
//小时个位0~9
uint16_t Led_clk_h_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};
//分钟十位0~5
uint16_t Led_clk_m_s[10]={0X003F,0x0006,0x005B,0x004F,0x0066,0x006D,0x007C,0x0007,0x007F,0x006F};
//分钟个位0~9
uint16_t Led_clk_m_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};
//温度十位
uint16_t led_th_s[10]={0X003F,0x0006,0x005B,0x004F,0x0066,0x006D,0x007C,0x0007,0x007F,0x006F};
//湿度个位
uint16_t led_th_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};
//温度十位
uint16_t led_hu_s[10]={0X003F,0x0006,0x005B,0x004F,0x0066,0x006D,0x007C,0x0007,0x007F,0x006F};
//湿度个位
uint16_t led_hu_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};

extern uint8_t touch_count;
/**
 * 总开关开启
 */
void Inf_led_start(void)
{
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_RESET);
}
/**
 * 总开关关闭
 */
void Inf_led_stop(void)
{
    // HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_SET);
    LED_OEA_H();
    LED_OEB_H();
}
/**
 * 电源控制和控制LED
 */
void Inf_led_write(uint8_t p00,uint8_t p01,uint8_t p02,uint32_t data)
{

    for (uint8_t i = 0; i < 32; i++)
    {
        if(data&(0x80000000>>i))
        {
            LED_SDI_H();
        }
        else
        {
            LED_SDI_L();
        }
        LED_CLK_H();
        LED_CLK_L();
    }
    LED_LE_H();
    LED_LE_L();
    HAL_GPIO_WritePin(VCCP00_GPIO_Port,VCCP00_Pin,(GPIO_PinState)p00);
    HAL_GPIO_WritePin(VCCP01_GPIO_Port,VCCP01_Pin,(GPIO_PinState)p01);
    HAL_GPIO_WritePin(VCCP02_GPIO_Port,VCCP02_Pin,(GPIO_PinState)p02);
    LED_OEA_L();
    LED_OEB_L();
}
/**
 * 快速控制亮灭达到同时显示效果 时间
 */
void Inf_led_set_clock(uint8_t h,uint8_t m,uint8_t clock_flag)
{
    uint8_t h_shi=h/10;
    uint8_t h_ge=h%10;
    if (clock_flag)
    {
        Inf_led_write(0,1,1,Led_clk_h_s[h_shi]+Led_clk_h_g[h_ge]+0x0020);
    }
    else
    {
        Inf_led_write(0,1,1,Led_clk_h_s[h_shi]+Led_clk_h_g[h_ge]);
    }
    vTaskDelay(5);
    uint8_t m_shi=m/10;
    uint8_t m_ge=m%10;
    Inf_led_write(1,0,1,Led_clk_m_s[m_shi]+Led_clk_m_g[m_ge]);
    vTaskDelay(5);
}
/**
 * 快速显示时间+温度+湿度
 */
void Inf_led_set_all(uint8_t h,uint8_t m,uint8_t clock_flag,int8_t temperature,int8_t humidity,Show_type_struct*show_type)
{
    //vccp00
    uint16_t data_right=led_hu_s[humidity/10]+led_hu_g[humidity%10]+0x4000;
    uint16_t data_left=0;
    uint16_t am_pm_data=0;
    uint32_t vccp02_data=0x380000;
    if (show_type->is_12_hour)
    {
        if (show_type->is_pm)
        {
            am_pm_data=0X8000;
        }
        else
        {
            am_pm_data=0X4000;
        }
    }
    if (show_type->is_alarm_five_day)
    {
        data_right+=0x8000;
    }
    if (clock_flag)
    {
        if (h>99)
        {
            data_left=am_pm_data+0x20;
        }
        else
        {
            data_left=Led_clk_h_s[h/10]+Led_clk_h_g[h%10]+0x0020+am_pm_data;
        }
    }
    else
    {
        if (h>99)
        {
            data_left=am_pm_data;
        }
        else
        {
            data_left=Led_clk_h_s[h/10]+Led_clk_h_g[h%10]+am_pm_data;
        }
    }
    Inf_led_write(0,1,1,(data_right<<16)|+data_left);
    vTaskDelay(5-touch_count);
    //vccp01
    int8_t temp_now=temperature;
    if (show_type->is_celsius==0)
    {
        temp_now=(temp_now*9/5)+32;
    }
    else
    {
        if (temp_now<0)
        {
            temp_now=-temp_now;
            vccp02_data+=0x020000;
        }
        if (temp_now>99)
        {
            temp_now-=100;
            vccp02_data+=0x050000;
        }
    }
    if (show_type->is_celsius)
    {
        data_right=led_th_s[temp_now/10]+led_th_g[temp_now%10]+0x4000;
    }
    else
    {
        
        data_right=led_th_s[temp_now/10]+led_th_g[temp_now%10]+0x8000;
    }
    if (m>99)
    {
        data_left=0;
    }
    else
    {
        data_left=Led_clk_m_s[m/10]+Led_clk_m_g[m%10];
    }
    uint16_t alarm_data=0;
    if (show_type->is_alarm_1)
    {
        alarm_data+=0x4000;
    }
    if (show_type->is_alarm_2)
    {
        alarm_data+=0x8000;
    }
    data_left+=alarm_data;
    //亮度调节
    HAL_GPIO_WritePin(VCCP00_GPIO_Port,VCCP00_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);
    Inf_led_write(1,0,1,(data_right<<16)|+data_left);
    vTaskDelay(5-touch_count);
    HAL_GPIO_WritePin(VCCP01_GPIO_Port,VCCP01_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);
    Inf_led_write(1,1,0,vccp02_data);
    vTaskDelay(5-touch_count);
    HAL_GPIO_WritePin(VCCP02_GPIO_Port,VCCP02_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);

    // 充电标识
    if (show_type->is_charing)
    {
        HAL_GPIO_WritePin(P03_GPIO_Port,P03_Pin,GPIO_PIN_RESET);
    }
}
/**
 *  时间设置时候展示时钟数字
 */
void Inf_led_time_set_show_num(uint16_t num1,uint16_t num2,uint8_t point)
{
    uint16_t vcc_p00_data=0;
    uint16_t vcc_p01_data=0;
    uint8_t point_data=0;
    if (point)
    {
        point_data=0x20;
    }
    if (num1>99)
    {
        vcc_p00_data=point_data+0;
    }
    else
    {
        vcc_p00_data=Led_clk_h_s[num1/10]+Led_clk_h_g[num1%10]+point_data;
    }
    if (num2>99)
    {
        vcc_p01_data=0;
    }
    else
    {
        vcc_p01_data=Led_clk_m_s[num2/10]+Led_clk_m_g[num2%10];
    }
    Inf_led_write(0,1,1,vcc_p00_data);
    vTaskDelay(5);
    Inf_led_write(1,0,1,vcc_p01_data);
    vTaskDelay(5);
}
/**
 * 设置时间时显示
 */
void Inf_led_time_set(SHOW_Time_Set time_set,uint16_t value)
{
    static uint8_t led_flag=0;
    static uint32_t time_set_last_time=0;
    if (xTaskGetTickCount()-time_set_last_time>500)
    {
        led_flag=!led_flag;
        time_set_last_time=xTaskGetTickCount();
    }
    switch (time_set)
    {
    case TIME_SET_YEAR:
        if (led_flag)
        {
            Inf_led_time_set_show_num(value/100,value%100,0);
        }
        else
        {
            Inf_led_time_set_show_num(value/100,0xFF,0);
        }
        break;
    case TIME_SET_MONTH:
        if (led_flag)
        {
            Inf_led_time_set_show_num(value,0xff,0);
        }
        else
        {
            Inf_led_time_set_show_num(0xff,0xff,0);
        }
        break;
    case TIME_SET_DAY:
        if (led_flag)
        {
            Inf_led_time_set_show_num(0xff,value,0);
        }
        else
        {
            Inf_led_time_set_show_num(0xff,0xff,0);
        }
        break;
    case TIME_SET_HOUR:
        if (led_flag)
        {
            Inf_led_time_set_show_num(value,0xff,1);
        }
        else
        {
            Inf_led_time_set_show_num(0xff,0xff,1);
        }
        break;
    case TIME_SET_MIN:
        if (led_flag)
        {
            Inf_led_time_set_show_num(0xff,value,1);
        }
        else
        {
            Inf_led_time_set_show_num(0xff,0xff,1);
        }
        break;
    default:
        break;
    }
}
