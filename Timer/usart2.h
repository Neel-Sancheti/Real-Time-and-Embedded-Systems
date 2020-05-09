#ifndef __STM32L476G_DISCOVERY_USART2_H
#define __STM32L476G_DISCOVERY_USART2_H

#include "stm32l476xx.h"

void USART2_Init(void);
void USART2_Write(void);
void USART2_Read(void);
void USART2_IRQ_Handler(void);

#endif
