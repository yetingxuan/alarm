#include "LED.h"
//小时十位0~2
uint16_t Led_clk_h_s[3]={0X0020,0X0026,0X007B};
//小时个位0~9
uint16_t Led_clk_h_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};
//分钟十位0~5
uint16_t Led_clk_m_s[10]={0X003F,0x0006,0x005B,0x004F,0x0066,0x006D,0x007D,0x0007,0x007F,0x006F};
//分钟个位0~9
uint16_t Led_clk_m_g[10]={0X1F80,0X0300,0X2D80,0X2780,0X3300,0X3680,0X3E00,0x0380,0X3F80,0X3380};
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
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_SET);
}
/**
 * 电源控制和控制LED
 */
void Inf_led_write(uint8_t p00,uint8_t p01,uint8_t p02,uint32_t data)
{
    HAL_GPIO_WritePin(VCCP00_GPIO_Port,VCCP00_Pin,(GPIO_PinState)p00);
    HAL_GPIO_WritePin(VCCP01_GPIO_Port,VCCP01_Pin,(GPIO_PinState)p01);
    HAL_GPIO_WritePin(VCCP02_GPIO_Port,VCCP02_Pin,(GPIO_PinState)p02);

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
    LED_OEA_L();
    LED_OEB_L();
}
/**
 * 快速控制亮灭达到同时显示效果
 */
void Inf_led_set_clock(uint8_t h,uint8_t m)
{
    uint8_t h_shi=h/10;
    uint8_t h_ge=h%10;
    Inf_led_write(0,1,1,Led_clk_h_s[h_shi]+Led_clk_h_g[h_ge]);
    vTaskDelay(5);
    uint8_t m_shi=m/10;
    uint8_t m_ge=m%10;
    Inf_led_write(1,0,1,Led_clk_m_s[m_shi]+Led_clk_m_g[m_ge]);
    vTaskDelay(5);

}
