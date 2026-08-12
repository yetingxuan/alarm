#ifndef __DS_1302Z_H__
#define __DS_1302Z_H__

#include "gpio.h"

#define DS1302Z_SECOND_REG  0x80
#define DS1302Z_MINUTE_REG  0x82
#define DS1302Z_HOUR_REG    0x84
#define DS1302Z_DAY_REG     0x86
#define DS1302Z_MONTH_REG   0x88
#define DS1302Z_WEEK_REG    0x8A
#define DS1302Z_YEAR_REG    0x8C

#define DS1302Z_CONTROL_REG  0x8E
#define DS1302Z_READ_REG    0x01
#define DS1302Z_WRITE_REG   0x00

#define DS_CLK_H()  HAL_GPIO_WritePin(DS_CLK_GPIO_Port,DS_CLK_Pin,GPIO_PIN_SET)
#define DS_CLK_L() HAL_GPIO_WritePin(DS_CLK_GPIO_Port,DS_CLK_Pin,GPIO_PIN_RESET)
#define DS_IO_H() HAL_GPIO_WritePin(DS_IO_GPIO_Port,DS_IO_Pin,GPIO_PIN_SET)
#define DS_IO_L() HAL_GPIO_WritePin(DS_IO_GPIO_Port,DS_IO_Pin,GPIO_PIN_RESET)
#define DS_IO_READ() HAL_GPIO_ReadPin(DS_IO_GPIO_Port,DS_IO_Pin)
#define DS_RST_H() HAL_GPIO_WritePin(DS_RST_GPIO_Port,DS_RST_Pin,GPIO_PIN_SET)
#define DS_RST_L() HAL_GPIO_WritePin(DS_RST_GPIO_Port,DS_RST_Pin,GPIO_PIN_RESET)

typedef struct{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    int8_t hour;
    int8_t min;
    uint8_t sec;
    uint8_t week;
    uint8_t is_pm;
    uint8_t is_12_flag;
}Clock_Date_time_type;
typedef struct {
    int8_t hour;
    int8_t min;
    uint8_t is_on;
    uint8_t is_done;
}Alarm_Time_Type;


void Inf_Delay_us(uint32_t us);
uint8_t Inf_DS1302Z_Read_Byte(uint8_t reg_addr);
void Inf_DS1302Z_Write_Byte(uint8_t reg_addr, uint8_t data);
void datatime_get(Clock_Date_time_type *data_time);
void dataTime_set(Clock_Date_time_type *data_time);
#endif

