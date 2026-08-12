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
/**
 * 读取全部时间
 */
void datatime_get(Clock_Date_time_type *data_time)
{
    uint8_t year_data= Inf_DS1302Z_Read_Byte(DS1302Z_YEAR_REG);
    uint8_t month_data=Inf_DS1302Z_Read_Byte(DS1302Z_MONTH_REG);
    uint8_t day_data=Inf_DS1302Z_Read_Byte(DS1302Z_DAY_REG);
    uint8_t week_data=Inf_DS1302Z_Read_Byte(DS1302Z_WEEK_REG);
    uint8_t hour_data=Inf_DS1302Z_Read_Byte(DS1302Z_HOUR_REG);
    uint8_t minute_data=Inf_DS1302Z_Read_Byte(DS1302Z_MINUTE_REG);
    data_time->year=(year_data>>4)*10+(year_data&0x0F)+2000;
    data_time->month=(month_data>>4)*10+(month_data&0x0F);
    data_time->day=(day_data>>4)*10+(day_data&0x0F);
    data_time->week=(week_data&0x0F);
    if (hour_data&0x80)
    {
        data_time->is_12_flag=1;
        if (hour_data&0x20)
        {
            data_time->is_pm=1;
        }
        else
        {
            data_time->is_pm=0;
        }
        if (hour_data&0x10)
        {
            data_time->hour=10+(hour_data&0x0F);
        }
        else
        {
            data_time->hour=(hour_data&0x0F);
        }
    }
    else
    {
        data_time->is_12_flag=0;
        if (hour_data&0x20)
        {
            data_time->hour=20+(hour_data&0x0F);
        }
        else if (hour_data&0x10)
        {
            data_time->hour=10+(hour_data&0x0F);
        }
        else
        {
            data_time->hour=(hour_data&0x0F);
        }
    }
    //统一保存24小时制
    if (data_time->is_12_flag==1)
    {
        if (data_time->is_pm==1)
        {
            data_time->hour+=12;
        }
        data_time->is_12_flag=0;
    }
    

    data_time->min=(minute_data>>4)*10+(minute_data&0x0F);
}
/**
 * 写入时间
 */
void dataTime_set(Clock_Date_time_type *data_time)
{
    Inf_DS1302Z_Write_Byte(DS1302Z_CONTROL_REG,0x00);
    Inf_DS1302Z_Write_Byte(DS1302Z_YEAR_REG,((data_time->year%100)/10)<<4|data_time->year%10);
    Inf_DS1302Z_Write_Byte(DS1302Z_MONTH_REG,(data_time->month/10)<<4|data_time->month%10);
    Inf_DS1302Z_Write_Byte(DS1302Z_DAY_REG,(data_time->day/10)<<4|data_time->day%10);
    //在switch中都用24小时制写入的时候都用12小时制
    if (data_time->hour>12)
    {
        Inf_DS1302Z_Write_Byte(DS1302Z_HOUR_REG,0xa0|(data_time->hour-12)/10<<4|(data_time->hour-12)%10);
    }
    else
    {
        Inf_DS1302Z_Write_Byte(DS1302Z_HOUR_REG,0xa0|(data_time->hour/10)<<4|data_time->hour%10);
    }

    Inf_DS1302Z_Write_Byte(DS1302Z_MINUTE_REG,(data_time->min/10)<<4|data_time->min%10);
    //秒默认都给0最高位为启震位
    Inf_DS1302Z_Write_Byte(DS1302Z_SECOND_REG,0x00);
    //周数
    uint16_t y=data_time->year;
    uint8_t m=data_time->month;
    uint8_t d=data_time->day;
    uint8_t c=y/100;
    if (m==1||m==2)
    {
        y--;
        m+=12;
    }
    int16_t w=y+y/4+c/4-2*c+26*(m+1)/10+d-1;
    while(w<0)
    {
        w+=7;
    }
    w=w%7;
    if (w==0)
    {
        w=7;
    }
    
}
