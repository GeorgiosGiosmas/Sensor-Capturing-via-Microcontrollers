/*
 * msp.c
 *
 *  Created on: Nov 18, 2025
 *      Author: gigio
 */
#include "main_custom.h"

void HAL_MspInit(void)
{

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Enable necessary IRQs
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(SysTick_IRQn, 3, 0);

	HAL_NVIC_EnableIRQ(MemoryManagement_IRQn);
	HAL_NVIC_EnableIRQ(BusFault_IRQn);
	HAL_NVIC_EnableIRQ(UsageFault_IRQn);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	UNUSED(hadc);
	GPIO_InitTypeDef gpio_analog;

	memset(&gpio_analog, 0, sizeof(gpio_analog));

	// Enable the clocks.
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_analog.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	gpio_analog.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_analog.Pull = GPIO_NOPULL;
	gpio_analog.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpio_analog);

}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	UNUSED(hi2c);
	GPIO_InitTypeDef gpio_i2c;

	memset(&gpio_i2c, 0, sizeof(gpio_i2c));

	// Enable the I2C and GPIO PORT B clock.
	__HAL_RCC_I2C2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_i2c.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	gpio_i2c.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_i2c.Mode = GPIO_MODE_AF_OD;
	gpio_i2c.Pull = GPIO_NOPULL;
	gpio_i2c.Alternate = GPIO_AF4_I2C2;

	HAL_GPIO_Init(GPIOB,  &gpio_i2c);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	GPIO_InitTypeDef gpio_uart;

	memset(&gpio_uart, 0, sizeof(gpio_uart));

	// Enable the UART and GPIO PORTB clock.
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_uart.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Alternate = GPIO_AF7_USART1;
	gpio_uart.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOB, &gpio_uart);

	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);

	// Enable clock in TIM2.
	__HAL_RCC_TIM2_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

