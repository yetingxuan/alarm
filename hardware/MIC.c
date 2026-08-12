#include "MIC.h"
Mic_value Inf_get_mic_value(void)
{
    if (HAL_GPIO_ReadPin(MIC_IN_GPIO_Port,MIC_IN_Pin)==GPIO_PIN_RESET)
    {
        return MIC_ON;
    }
    else
    {
        return MIC_OFF;
    }
    
}
