#ifndef __BOARD_H
#define __BOARD_H

#include "stm8l15x.h"

#define LED_PORT        GPIOD
#define LED_PIN         GPIO_Pin_4

#define BUTTON_PORT     GPIOB
#define BUTTON_PIN      GPIO_Pin_6
#define BUTTON_EXTI     EXTI_Pin_6

#define UART1           USART1
#define UART1_GPIO      GPIOC
#define UART1_CLK       CLK_Peripheral_USART1
#define UART1_RX        GPIO_Pin_2
#define UART1_TX        GPIO_Pin_3


typedef enum
{
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

void CLK_HSE_16MHz_Init(void);
void CLK_HSI_16MHz_Init(void);

void Delay_Init(void);
void TimingDelay_Decrement(void);
void delay_ms(__IO uint32_t msTime);

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

void Button_Init(ButtonMode_TypeDef Button_Mode);
uint8_t Button_GetState(void);

void UART1_Init(uint32_t USART_BaudRate,
                      USART_WordLength_TypeDef USART_WordLength,
                      USART_StopBits_TypeDef USART_StopBits,
                      USART_Parity_TypeDef USART_Parity,
                      USART_Mode_TypeDef USART_Mode);
					  
#endif /* __BOARD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
