#ifndef WS2812B_H
#define WS2812B_H

#include <cstring>
#include "stm32f0xx.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_rcc_ex.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_tim_ex.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_dma_ex.h"

#define WS2812B_TEST                     0
#define WS2812B_GPIO_PERIF               RCC_AHBPeriph_GPIOA
#define WS2812B_TIM_PERIF                RCC_APB2Periph_TIM1
#define WS2812B_TIM                      TIM1
#define WS2812B_DMA                      RCC_AHBPeriph_DMA1
#define WS2812B_DMA_CHANNEL              DMA1_Channel2
#define WS2812B_DMA_NVIC_IRQChannel      DMA1_Channel2_3_IRQn
#define WS2812B_GPIO_PORT                GPIOA
#define WS2812B_GPIO_PIN                 GPIO_Pin_8
#define WS2812B_TMI_CLOCK                24000000
#define WS2812B_TIM_PERIOD               29

#define WS2812B_NUM                      13
#define WS2812B_PULSE_BIT_SIZE           24
#define WS2812B_BUFF_SIZE                (24 * (WS2812B_NUM) + 43)   // Buffer size needs to be the number of LEDs times 24 bits plus 43 trailing bit
#define WS2812B_NVIC_PRI                 10
#define WS2812B_DMA_PeripheralBaseAddr   (uint32_t)&TIM1->CCR1;

#define WS2812B_PWM_HIGH_PULSE_WIDTH     19                    // Duty cycle of pwm signal for a logical 1 to be read by the ws2812 chip
#define WS2812B_PWM_LOW_PULSE_WIDTH      9                     // Duty cycle of pwm signal for a logical 0 to be read by the ws2812 chip
#define WS2812B_ALL_INDEX                -1

#define WS2812B_CREATE_PULSE_BUFF() do { \
        clr = color; \
        for (j = 0; j < WS2812B_PULSE_BIT_SIZE; j++) { \
            ws2812bBuff[k++] = clr & 0x800000 ? WS2812B_PWM_HIGH_PULSE_WIDTH : WS2812B_PWM_LOW_PULSE_WIDTH; \
            clr <<= 1; \
        } \
    } while(0)

class ws2812b {
public:
    static void Init();

    static bool Test();

    static void Clear(int index);

    static void ClearAll();

    static void Set(int index, int32_t color);

    static void SetAll(int32_t color);

    static void Send();
};

#endif //WS2812B_H
