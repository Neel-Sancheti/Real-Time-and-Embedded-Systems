#include "led_init.h"

void LED_Init(void)
{	
	// LED Red = PB2
	// LED Green = PE8
	
	//Enable the Peripheral Clock of Port B and Port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; 		//Set bit 1 :: Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; 		//Set bit 4 :: Port E
	
	//Configure the Mode Register	
	//Make PB2 and PE8 as output (01)
	GPIOB->MODER &= ~GPIO_MODER_MODER2_1;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;

	GPIOE->MODER &= ~GPIO_MODER_MODER8_1;
	GPIOE->MODER |= GPIO_MODER_MODER8_0;
	
	//Set Output Speed as Low
	//Make PB2 and PE8 as low speed pins (00)
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2_0;
	
	GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR8_1;
	GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR8_0;
}

void Red_LED_on(void)
{
	//Set the PB2 pin
	GPIOB->ODR |= GPIO_ODR_ODR_2;
}

void Red_LED_off(void)
{
	//Reset the PB2 pin
	GPIOB->ODR &= ~GPIO_ODR_ODR_2;
}

void Green_LED_on(void)
{
	//Set the PE8 pin
	GPIOE->ODR |= GPIO_ODR_ODR_8;
}

void Green_LED_off(void)
{
	//Reset the PE8 pin
	GPIOE->ODR &= ~GPIO_ODR_ODR_8;
}
