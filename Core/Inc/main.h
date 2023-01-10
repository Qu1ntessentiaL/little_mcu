#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "usart.h"
#include "gpio.h"

void Error_Handler(void);
void SystemClock_Config(void);

#define BUTTON_0_Pin GPIO_PIN_1
#define BUTTON_0_GPIO_Port GPIOA
#define BUTTON_T_Pin GPIO_PIN_2
#define BUTTON_T_GPIO_Port GPIOA
#define BUTTON_SUM_Pin GPIO_PIN_3
#define BUTTON_SUM_GPIO_Port GPIOA
#define BUTTON_M__Pin GPIO_PIN_4
#define BUTTON_M__GPIO_Port GPIOA
#define LIGHT_Pin GPIO_PIN_0
#define LIGHT_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
