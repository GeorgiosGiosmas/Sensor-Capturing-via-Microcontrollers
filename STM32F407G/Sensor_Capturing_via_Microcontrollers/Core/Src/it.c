/*
 * it.c
 *
 *  Created on: Nov 18, 2025
 *      Author: gigio
 */
#include "it.h"

extern UART_HandleTypeDef usart1;
extern TIM_HandleTypeDef timer;

void TIM1_UP_TIM10_IRQHandler(void)
{
	// Increase tick.
	uwTick += uwTickFreq;
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&usart1);
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer);
}

void HardFault_Handler(void)
{
	// Halt execution, when HardFault error occurs.
	for(;;);
}

void MemManage_Handler(void)
{
	// Halt execution, when MemManage error occurs.
	for(;;);
}

void BusFault_Handler(void)
{
	// Halt execution, when BusFault error occurs.
	for(;;);
}

void UsageFault_Handler(void)
{
	// Halt execution, when UsageFault error occurs.
	for(;;);
}

