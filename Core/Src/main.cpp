#include "dma.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "ws2812b.h"

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(100);
    }
}