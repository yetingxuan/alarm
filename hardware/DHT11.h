#ifndef __DHT11_H__
#define __DHT11_H__

#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "DS_1302Z.h"
#include "DEBUG.h"

#define DHT11_DATA_H()    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port,DHT11_DATA_Pin,GPIO_PIN_SET)
#define DHT11_DATA_L()    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port,DHT11_DATA_Pin,GPIO_PIN_RESET)
#define DHT11_DATA_Read() HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port,DHT11_DATA_Pin)

void Inf_dht11_init(void);
void Inf_dht11_get_data(int8_t *temperature,int8_t*humidity);

#endif
