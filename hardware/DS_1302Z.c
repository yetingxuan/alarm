#include "DS_1302Z.h"
/**
 * 延时函数微秒级
 */
void Inf_Delay_us(uint32_t us)
{
    uint32_t count=10*us;
    while (count--)
    {
        __NOP();
    }
}

uint8_t Inf_DS1302Z_Read_Byte(uint8_t reg_addr)
{
    reg_addr |= 0x01;

    uint8_t data = 0;
    DS_RST_L();
    DS_CLK_L();
    DS_RST_H();
    Inf_Delay_us(5);
    // 写命令
    for (uint8_t i = 0; i < 8; i++)
    {
        if (reg_addr & (0x01 << i))
        {
            DS_IO_H();
        }
        else
        {
            DS_IO_L();
        }
        DS_CLK_H();
        Inf_Delay_us(1);
        DS_CLK_L();
    }
    // 读数据
    DS_IO_H();
    for (uint8_t i = 0; i < 8; i++)
    {
        data |= (DS_IO_READ() << i);
        if (i < 7)
        {
            DS_CLK_H();
            DS_CLK_L();
        }
    }
    DS_RST_L();
    return data;
}

void Inf_DS1302Z_Write_Byte(uint8_t reg_addr, uint8_t data)
{
    reg_addr &= 0xFE;

    DS_RST_L();
    DS_CLK_L();
    DS_RST_H();
    Inf_Delay_us(5);
    for (uint8_t i = 0; i < 8; i++)
    {
        if (reg_addr & (0x01 << i))
        {
            DS_IO_H();
        }
        else
        {
            DS_IO_L();
        }
        DS_CLK_H();
        Inf_Delay_us(1);
        DS_CLK_L();
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (0x01 << i))
        {
            DS_IO_H();
        }
        else
        {
            DS_IO_L();
        }
        DS_CLK_H();
        Inf_Delay_us(1);
        DS_CLK_L();
    }
}
