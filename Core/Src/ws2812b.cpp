#include "ws2812b.h"

static bool isInited = false;
static uint16_t ws2812bBuff[WS2812B_BUFF_SIZE];   // Array of data to be sent to leds.

void ws2812b::Init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pins : PA8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/*
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    DMA_InitTypeDef DMA_InitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_DeInit(GPIOA);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);

    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_Period = WS2812B_TIM_PERIOD;
    TIM_TimeBaseInitStruct.TIM_Prescaler = (uint16_t) (SystemCoreClock / WS2812B_TMI_CLOCK) - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    DMA_StructInit(
            &DMA_InitStruct);                                                                                     // Deinitialize DAM1 Channel 7 to their default reset values.
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &
            TIM1->CCR1;                     // Specifies Physical address of the peripheral in this case Timer
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) &ws2812bBuff;                // Specifies the buffer memory address
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                            // Data transfered from memory to peripheral
    DMA_InitStruct.DMA_BufferSize = WS2812B_BUFF_SIZE;                         // Specifies the buffer size
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;              // Do not incrament the peripheral address
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                       // Incrament the buffer index
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   // Specifies the peripheral data width 16bit
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;           // Specifies the memory data width
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                                 // Specifies the operation mode. Normal or Circular
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;                           // Specifies the software priority
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                  // memory to memory disable
    DMA_Init(WS2812B_DMA_CHANNEL,
             &DMA_InitStruct);                            // Initialize DAM1 Channel 2 to values specified in the DMA_InitStruct structure.

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
*/
    ClearAll();

    isInited = true;
}

bool ws2812b::Test() {
#if (WS2812B_TEST == 1)
    ws2812bSet(0, 0xff0000);
    for(int i = 1000000; i > 0; i--);
    ws2812bClear(0);
#endif
    return isInited;
}

void ws2812b::Clear(const int index) {
    Set(index, 0);
}

void ws2812b::ClearAll() {
    int32_t i, j, k = 0;

    for (i = 0; i < WS2812B_NUM; i++) {
        for (j = 0; j < WS2812B_PULSE_BIT_SIZE; j++) {
            ws2812bBuff[k++] = WS2812B_PWM_LOW_PULSE_WIDTH;
        }
    }

    memset(&ws2812bBuff[k], 0, WS2812B_BUFF_SIZE - k);
    ws2812bSend();
}

void ws2812b::Set(int index, int32_t color) {
    int32_t i, j;
    int32_t k = 0;
    int32_t clr = color;

    if (index >= WS2812B_NUM || (index < 0 && index != WS2812B_ALL_INDEX)) {
        return;
    }

    k = index * WS2812B_PULSE_BIT_SIZE;

    if (index != WS2812B_ALL_INDEX) {
        WS2812B_CREATE_PULSE_BUFF();
    } else {
        for (i = 0; i < WS2812B_NUM; i++)
            WS2812B_CREATE_PULSE_BUFF();
    }

    ws2812bSend();
}

void ws2812b::SetAll(const int32_t color) {
    Set(WS2812B_ALL_INDEX, color);
}

void ws2812b::Send() {
    //DMA_HandleTypeDef hdma1;
    //HAL_DMA_Start(&hdma1, (uint32_t) &ws2812bBuff, (uint32_t) &TIM1->CCR1, WS2812B_BUFF_SIZE);
    //DMA_SetCurrDataCounter(DMA1_Channel2, WS2812B_BUFF_SIZE);
    //DMA_Cmd(DMA1_Channel2, ENABLE);
    //TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
    //TIM_Cmd(TIM1, ENABLE);
}

void DMA1_Channel2_3_IRQHandler() {
    //DMA_ClearITPendingBit(DMA1_IT_TC2);        // clear DMA1 transfer complete interrupt flag
    //TIM_Cmd(TIM1, DISABLE);                    // disable the TIM and DMA channels
    //DMA_Cmd(DMA1_Channel2, DISABLE);
    //DMA_ClearFlag(DMA1_FLAG_TC2);              // clear DMA1 Channel 2 transfer complete flag
    //TIM_DMACmd(TIM1, TIM_DMA_CC1, DISABLE);    // disable the DMA requests
}