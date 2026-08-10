#ifndef __LED_H__
#define __LED_H__

#include "gpio.h"
#include "freeRTOS.h"
#include "task.h"

#define LED_CLK_H()   HAL_GPIO_WritePin(LED_CLK_GPIO_Port,LED_CLK_Pin,GPIO_PIN_SET);
#define LED_CLK_L()   HAL_GPIO_WritePin(LED_CLK_GPIO_Port,LED_CLK_Pin,GPIO_PIN_RESET);
#define LED_SDI_H()   HAL_GPIO_WritePin(LED_SDI_GPIO_Port,LED_SDI_Pin,GPIO_PIN_SET);
#define LED_SDI_L()   HAL_GPIO_WritePin(LED_SDI_GPIO_Port,LED_SDI_Pin,GPIO_PIN_RESET);
#define LED_LE_H()    HAL_GPIO_WritePin(LED_LE_GPIO_Port,LED_LE_Pin,GPIO_PIN_SET);
#define LED_LE_L()    HAL_GPIO_WritePin(LED_LE_GPIO_Port,LED_LE_Pin,GPIO_PIN_RESET);
#define LED_OEA_H()   HAL_GPIO_WritePin(LED_OEA_GPIO_Port,LED_OEA_Pin,GPIO_PIN_SET);
#define LED_OEA_L()   HAL_GPIO_WritePin(LED_OEA_GPIO_Port,LED_OEA_Pin,GPIO_PIN_RESET);
#define LED_OEB_H()   HAL_GPIO_WritePin(LED_OEB_GPIO_Port,LED_OEB_Pin,GPIO_PIN_SET);
#define LED_OEB_L()   HAL_GPIO_WritePin(LED_OEB_GPIO_Port,LED_OEB_Pin,GPIO_PIN_RESET);

extern uint16_t Led_clk_h_s[3];
extern uint16_t Led_clk_h_g[10];
extern uint16_t Led_clk_m_s[10];
extern uint16_t Led_clk_m_g[10];

void Inf_led_start(void);
void Inf_led_stop(void);
void Inf_led_write(uint8_t p00,uint8_t p01,uint8_t p02,uint32_t data);
void Inf_led_set_clock(uint8_t h,uint8_t m);
#endif
