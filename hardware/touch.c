#include "touch.h"
uint8_t touch_value = 2;
Touch_value Inf_get_touch_value(void)
{
    if (touch_value == 2)
    {
        touch_value = (uint8_t)HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin);
        return NO_TOUCH;
    }

    if (touch_value != (uint8_t)HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin))
    {
        touch_value = (uint8_t)HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin);

        return TOUCH;
    }

    return NO_TOUCH;
}
