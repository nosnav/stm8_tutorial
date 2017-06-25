#include "board.h"

#define TIM4_PERIOD       124
__IO uint32_t TimingDelay;

void CLK_HSE_16MHz_Init(void)
{
    /* Select HSE as system clock source */
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    /* system clock prescaler: 1*/
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    //while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE);
}

void CLK_HSI_16MHz_Init(void)
{
    /* Select HSE as system clock source */
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    /* system clock prescaler: 1*/
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    //while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
}

void Delay_Init(void)
{
    /* Enable TIM4 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    
    /* TIM4 configuration:
    - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
    clock used is 16 MHz / 128 = 125 000 Hz
    - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
    - In this example we need to generate a time base equal to 1 ms
    so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

    /* Time base configuration */
    TIM4_TimeBaseInit(TIM4_Prescaler_128, TIM4_PERIOD);
    /* Clear TIM4 update flag */
    TIM4_ClearFlag(TIM4_FLAG_Update);
    /* Enable update interrupt */
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    /* enable interrupts */
    enableInterrupts();

    /* Enable TIM4 */
    TIM4_Cmd(ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_ms(__IO uint32_t msTime)
{
    TimingDelay = msTime;
    while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

void LED_Init(void)
{
    GPIO_Init(LED_PORT, LED_PIN, GPIO_Mode_Out_PP_High_Fast);
}
void LED_On(void)
{
    LED_PORT->ODR &= (uint8_t)(~LED_PIN);
}

void LED_Off(void)
{
    LED_PORT->ODR |= LED_PIN;
}

void LED_Toggle(void)
{
    LED_PORT->ODR ^= LED_PIN;
}


void Button_Init(ButtonMode_TypeDef Button_Mode)
{
    if (Button_Mode == BUTTON_MODE_EXTI)
    { /* Pin configured in input floating mode with interrupt enabled
        --> connected to EXTIx Interrupt, where x:0..7 */
        GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_In_PU_IT);
        EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)BUTTON_EXTI, EXTI_Trigger_Falling);
    }
    else
    {/* Pin configured in input floating mode with interrupt disabled */
        GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_In_FL_No_IT);
    }
}

uint8_t Button_GetState(void)
{
    return GPIO_ReadInputDataBit(BUTTON_PORT, (GPIO_Pin_TypeDef)BUTTON_PIN);
}

void UART1_Init(uint32_t USART_BaudRate,
                      USART_WordLength_TypeDef USART_WordLength,
                      USART_StopBits_TypeDef USART_StopBits,
                      USART_Parity_TypeDef USART_Parity,
                      USART_Mode_TypeDef USART_Mode)
{
    /* Enable USART clock */
    CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)CLK_Peripheral_USART1, ENABLE);

    /* Configure USART Tx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(UART1_GPIO, UART1_RX, ENABLE);

    /* Configure USART Rx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(UART1_GPIO, UART1_RX, ENABLE);

    /* USART configuration */
    USART_Init(UART1, USART_BaudRate,
             USART_WordLength,
             USART_StopBits,
             USART_Parity,
             USART_Mode);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
