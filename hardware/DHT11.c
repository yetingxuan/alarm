#include "DHT11.h"
uint8_t data[5];
/**
 * 上电延时
 */
void Inf_dht11_init(void)
{
    DHT11_DATA_H();
    vTaskDelay(1000);
}

/**
 * 获取温湿度数据   温度有正负
 */
void Inf_dht11_get_data(int8_t *temperature,int8_t*humidity)
{
    int temp=0,hum=0;
    DHT11_DATA_L();
    vTaskDelay(20);
    DHT11_DATA_H();
    /**
     * 超时逻辑
     */
    uint32_t count=0xffffff;
    while (DHT11_DATA_Read()==GPIO_PIN_SET&&count--)
    {
        /* 等待响应信号开始 */
    }
    while (DHT11_DATA_Read()==GPIO_PIN_RESET&&count--)
    {
        /* 等待响应信号结束 */
    }
    while (DHT11_DATA_Read()==GPIO_PIN_SET&&count--)
    {
        /* 等待DHT11给的缓冲时间 */
    }
    if (count==0)
    {
        debug_printf("DHT11响应超时");
        return;
    }
    
    for(uint8_t i = 0; i < 5; i++)
    {
        data[i]=0;
        for (uint8_t j = 0; j < 8; j++)
        {
            while (DHT11_DATA_Read()==GPIO_PIN_RESET)
            {
                /* 等数据位的前面固定的低电平结束 */
            }
            Inf_Delay_us(40);
            if (DHT11_DATA_Read()==GPIO_PIN_SET)
            {
                data[i]|=0x80>>j;
                while (DHT11_DATA_Read()==GPIO_PIN_SET)
                {
                    /* 等待长时间高电平结束与短时间高电平（0的情况）时间对齐 */
                }
            }
            else
            {
                /*不管移位出来就是0*/
            }
        }
    }
    //效验码
    uint32_t sum=data[0]+data[1]+data[2]+data[3];
    if ((uint8_t)sum==data[4])
    {
        hum=data[0];
        if (data[3]&0x80)
        {
            temp=-data[2];
        }
        else
        {
            temp=data[2];
        }
        *humidity=hum;
        *temperature=temp;
    }
    else
    {
        debug_printf("DHT11数据效验失败");
    }
}

