#include "NV020D.h"
#include "FreeRTOS.h"
#include "task.h"
/**
 * 上电延时
 */
void Inf_nv020d_init(void)
{
    vTaskDelay(500);
}
/**
 * 发一个字节
 */
void Inf_nv020d_send_byte(uint8_t byte)
{
    NV020D_CLK_H();
    NV020D_CLK_L();
    if (byte&0x01)
    {
        NV020D_SDA_H();
    }
    else
    {
        NV020D_SDA_L();
    }
    vTaskDelay(4);
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte&(0x01<<i))
        {
            NV020D_SDA_H();
        }
        else
        {
            NV020D_SDA_L();
        }
        NV020D_CLK_L();
        Inf_Delay_us(400);
        NV020D_CLK_H();
        Inf_Delay_us(400);
    }
    NV020D_CLK_H();
}
/**
 * 双线模式
 */
// void Inf_nv020d_send_cmd(uint8_t cmd)
// {
//     uint8_t sum=0;
//     while (NV020D_BUSY_READ()==GPIO_PIN_RESET)
//     {
//         vTaskDelay(10);
//         /*等待忙*/
//     }
//     vTaskDelay(100);//每次都加实则只有忙结束的时候需要加
//     Inf_nv020d_send_byte(0XF1);
//     Inf_nv020d_send_byte(cmd);
//     Inf_nv020d_send_byte(0XF3);
//     sum=(uint8_t)(0XF1+cmd+0XF3);
// }



/**
 * 单线模式
 */
void Inf_nv020d_send_cmd(uint8_t cmd)
{
    NV020D_CTR_H();
    NV020D_CTR_L();
    vTaskDelay(4);
    for (uint8_t i = 0; i < 8; i++)
    {
        if (cmd&(0x01<<i))
        {
            NV020D_CTR_H();
            Inf_Delay_us(1200);
            NV020D_CTR_L();
            Inf_Delay_us(400);
        }
        else
        {
            NV020D_CTR_H();
            Inf_Delay_us(400);
            NV020D_CTR_L();
            Inf_Delay_us(1200);
        }
    }
    NV020D_CTR_H();
    vTaskDelay(100);
}

/**
 * 设置音量 0~7
 */
void Inf_nv020d_set_volume(uint8_t volume)
{
    if (volume>7)
    {
        debug_printf("volume error");
        return;
    }
    
    Inf_nv020d_send_cmd(0xE0+volume);
}
/**
 * 停止播放
 */
void Inf_nv020d_stop(void)
{
    Inf_nv020d_send_cmd(0xFE);
}
