#ifndef __MIC_H__
#define __MIC_H__
#include "gpio.h"

typedef enum{
    MIC_ON=0,
    MIC_OFF,
}Mic_value;

Mic_value Inf_get_mic_value(void);

#endif
