#ifndef __NV020D_H__
#define __NV020D_H__

#include "gpio.h"
#include "DS_1302Z.h"
#include "DEBUG.h"
//双线模式
#define NV020D_CLK_H() HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_SET)
#define NV020D_CLK_L() HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_RESET)
#define NV020D_SDA_H() HAL_GPIO_WritePin(NVD_SDA_GPIO_Port,NVD_SDA_Pin,GPIO_PIN_SET)
#define NV020D_SDA_L() HAL_GPIO_WritePin(NVD_SDA_GPIO_Port,NVD_SDA_Pin,GPIO_PIN_RESET)
#define NV020D_BUSY_READ() HAL_GPIO_ReadPin(NVD_BUSY_GPIO_Port,NVD_BUSY_Pin)
//单线模式
#define NV020D_CTR_H()  HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_SET)
#define NV020D_CTR_L()  HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_RESET)

void Inf_nv020d_init(void);
void Inf_nv020d_send_cmd(uint8_t cmd);
void Inf_nv020d_set_volume(uint8_t volume);

#endif
