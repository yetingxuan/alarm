#include "App_freeRTOS.h"
void task1(void*args);
TaskHandle_t task1_handle;
void task2(void*args);
TaskHandle_t task2_handle;
void App_freeRTOS_start(void)
{
    xTaskCreate(task1,"task1",128,NULL,1,&task1_handle);
    xTaskCreate(task2,"task2",128,NULL,1,&task2_handle);
    vTaskStartScheduler();
}

void task1(void*args)
{
    while(1)
    {
        debug_printf("task1");
        vTaskDelay(1000);
    }
}
void task2(void*args)
{
    while(1)
    {
        debug_printf("task2");
        vTaskDelay(1000);
    }
}