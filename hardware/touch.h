#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "gpio.h"

typedef enum
{
    TOUCH,
    NO_TOUCH
}Touch_value;

Touch_value Inf_get_touch_value(void);

#endif
